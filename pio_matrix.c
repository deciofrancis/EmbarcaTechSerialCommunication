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

int main()
{

  stdio_init_all();

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

    while (true)
  {
  }
}
