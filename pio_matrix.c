#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "hardware/i2c.h"
#include "libs/font.h"
#include "libs/numbers.h"
#include "ssd1306.h"

#include "pio_matrix.pio.h"

// Definição dos pinos e configurações do display e botões
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define DISPLAY_ADDR 0x3C
#define MATRIX_SIZE 25
#define GREEN_LED 11
#define BLUE_LED 12
#define BUTTON_A 5
#define BUTTON_B 6
#define LED_MATRIX_PIN 7

// Instância do PIO e variáveis globais para controle de LEDs e display
PIO pio_instance = pio0;
uint32_t led_color;
uint sm_instance;
uint program_offset;
double red_intensity = 0.0, green_intensity = 0.0, blue_intensity = 0.0;

ssd1306_t display;
static volatile uint32_t last_button_event = 0;

// Converte valores de intensidade de cor (0.0 a 1.0) para um formato de 32 bits
uint32_t convert_rgb(double blue, double red, double green)
{
  uint8_t r = red * 255;
  uint8_t g = green * 255;
  uint8_t b = blue * 255;
  return (g << 24) | (r << 16) | (b << 8);
}

// Atualiza a matriz de LEDs com um padrão de exibição
void desenha_pio(const double *pattern, uint32_t led_value, PIO pio, uint sm, double red, double green, double blue)
{
  // Mapeamento dos pixels para a matriz física de LEDs
  int pixel_map[MATRIX_SIZE] = {
      24, 23, 22, 21, 20,
      15, 16, 17, 18, 19,
      14, 13, 12, 11, 10,
      5, 6, 7, 8, 9,
      4, 3, 2, 1, 0};

  // Atualiza os LEDs conforme o padrão recebido
  for (int i = 0; i < MATRIX_SIZE; i++)
  {
    int physical_index = pixel_map[i];
    led_value = convert_rgb(pattern[physical_index], red, green);
    pio_sm_put_blocking(pio, sm, led_value);
  }
}

// Função de interrupção para manipular os botões com debounce
void handle_button_interrupt(uint gpio, uint32_t events)
{
  uint32_t current_time = to_us_since_boot(get_absolute_time());

  // Implementação de debounce para evitar leituras repetidas rápidas
  if (current_time - last_button_event > 200000)
  {
    last_button_event = current_time;

    if (gpio == BUTTON_A)
    {
      gpio_put(GREEN_LED, !gpio_get(GREEN_LED));

      // Atualiza o display OLED com o estado do LED verde
      ssd1306_fill(&display, false);
      ssd1306_draw_string(&display, gpio_get(GREEN_LED) ? "Green LED On" : "Green LED Off", 20, 30);
      ssd1306_send_data(&display);
      printf("Green LED %s\n", gpio_get(GREEN_LED) ? "On" : "Off");
    }

    if (gpio == BUTTON_B)
    {
      gpio_put(BLUE_LED, !gpio_get(BLUE_LED));

      // Atualiza o display OLED com o estado do LED azul
      ssd1306_fill(&display, false);
      ssd1306_draw_string(&display, gpio_get(BLUE_LED) ? "Blue LED On" : "Blue LED Off", 20, 30);
      ssd1306_send_data(&display);
      printf("Blue LED %s\n", gpio_get(BLUE_LED) ? "On" : "Off");
    }
  }
}

int main()
{
  // Configuração do clock do sistema
  if (!set_sys_clock_khz(128000, false))
  {
    printf("Failed to set clock speed\n");
    return 1;
  }

  stdio_init_all();
  printf("Starting PIO communication...\n");

  // Configuração do PIO para a matriz de LEDs
  program_offset = pio_add_program(pio_instance, &pio_matrix_program);
  sm_instance = pio_claim_unused_sm(pio_instance, true);
  pio_matrix_program_init(pio_instance, sm_instance, program_offset, LED_MATRIX_PIN);

  // Configuração dos LEDs
  gpio_init(GREEN_LED);
  gpio_set_dir(GREEN_LED, GPIO_OUT);
  gpio_put(GREEN_LED, false);

  gpio_init(BLUE_LED);
  gpio_set_dir(BLUE_LED, GPIO_OUT);
  gpio_put(BLUE_LED, false);

  // Configuração dos botões
  gpio_init(BUTTON_A);
  gpio_set_dir(BUTTON_A, GPIO_IN);
  gpio_pull_up(BUTTON_A);

  gpio_init(BUTTON_B);
  gpio_set_dir(BUTTON_B, GPIO_IN);
  gpio_pull_up(BUTTON_B);

  // Inicializa o barramento I2C e o display OLED
  i2c_init(I2C_PORT, 400 * 1000);
  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
  gpio_pull_up(I2C_SDA);
  gpio_pull_up(I2C_SCL);

  ssd1306_init(&display, WIDTH, HEIGHT, false, DISPLAY_ADDR, I2C_PORT);
  ssd1306_config(&display);
  ssd1306_send_data(&display);
  ssd1306_fill(&display, false);
  ssd1306_send_data(&display);

  // Configuração das interrupções para os botões
  gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &handle_button_interrupt);
  gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &handle_button_interrupt);

  // Loop principal aguardando entrada via USB
  while (true)
  {
    if (stdio_usb_connected())
    {
      char input_char;
      if (scanf("%c", &input_char) == 1)
      {
        printf("Received: %c\n", input_char);

        // Exibe o caractere recebido no display OLED
        ssd1306_fill(&display, false);
        ssd1306_draw_string(&display, &input_char, 30, 30);
        ssd1306_send_data(&display);

        // Se o caractere recebido for um número, atualiza a matriz de LEDs
        if (input_char >= '0' && input_char <= '9')
        {
          int number = input_char - '0';
          desenha_pio(numbers[number], led_color, pio_instance, sm_instance, red_intensity, green_intensity, blue_intensity);
        }
      }
    }
  }
}
