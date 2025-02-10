## 🔥 Projeto: Introdução às Interfaces de Comunicação Serial com RP2040 UART, SPI e I2C 

## 📌 Descrição do Projeto

Este projeto tem como objetivo consolidar os conceitos de comunicação serial e controle de hardware utilizando a placa de desenvolvimento BitDogLab com o microcontrolador RP2040. O projeto envolve a manipulação de LEDs comuns, LEDs endereçáveis WS2812, botões de acionamento, interrupções, e um display SSD1306, além da comunicação serial via UART e I2C.

## Objetivos

- Compreender o funcionamento e a aplicação de comunicação serial em microcontroladores.
- Aplicar os conhecimentos adquiridos sobre UART e I2C na prática.
- Manipular e controlar LEDs comuns e LEDs endereçáveis WS2812.
- Fixar o estudo do uso de botões de acionamento, interrupções e Debounce.
- Desenvolver um projeto funcional que combine hardware e software.

## 🛠️ Componentes Utilizados

- **Matriz 5x5 de LEDs WS2812**: Conectada à GPIO 7.
- **LED RGB**: Conectado às GPIOs 11 (Verde), 12 (Azul) e 13 (Vermelho).
- **Botão A**: Conectado à GPIO 5.
- **Botão B**: Conectado à GPIO 6.
- **Display SSD1306**: Conectado via I2C (GPIO 14 e GPIO 15).

## Funcionalidades do Projeto

### 1. Modificação da Biblioteca `font.h`

- Adicionar caracteres minúsculos e maiúsculos à biblioteca `font.h`.

### 2. Entrada de Caracteres via PC

- Utilize o Serial Monitor do VS Code para digitar os caracteres.
- Cada caractere digitado no Serial Monitor deve ser exibido no display SSD1306. **Observação**: Apenas um caractere será enviado de cada vez, não é necessário suportar o envio de strings completas.
- Quando um número entre 0 e 9 for digitado, um símbolo correspondente ao número deve ser exibido na matriz 5x5 WS2812.

### 3. Interação com o Botão A

- Pressionar o botão A deve alternar o estado do LED RGB Verde (ligado/desligado).
- A operação deve ser registrada de duas formas:
  - Uma mensagem informativa sobre o estado do LED deve ser exibida no display SSD1306.
  - Um texto descritivo sobre a operação deve ser enviado ao Serial Monitor.

### 4. Interação com o Botão B

- Pressionar o botão B deve alternar o estado do LED RGB Azul (ligado/desligado).
- A operação deve ser registrada de duas formas:
  - Uma mensagem informativa sobre o estado do LED deve ser exibida no display SSD1306.
  - Um texto descritivo sobre a operação deve ser enviado ao Serial Monitor.

## 📋 Requisitos do Projeto

1. **Uso de Interrupções**: Todas as funcionalidades relacionadas aos botões devem ser implementadas utilizando rotinas de interrupção (IRQ).
2. **Debouncing**: É obrigatório implementar o tratamento do bouncing dos botões via software.
3. **Controle de LEDs**: O projeto deve incluir o uso de LEDs comuns e LEDs WS2812, demonstrando o domínio de diferentes tipos de controle.
4. **Utilização do Display 128x64**: A utilização de fontes maiúsculas e minúsculas demonstrará o domínio do uso de bibliotecas, o entendimento do princípio de funcionamento do display, bem como a utilização do protocolo I2C.
5. **Envio de Informação pela UART**: Visa observar a compreensão sobre a comunicação serial via UART.
6. **Organização do Código**: O código deve estar bem estruturado e comentado para facilitar o entendimento.

## 🛠️ Configurando o Projeto no VS Code

1. **Clonar o Repositório**:
```
git clone https://github.com/deciofrancis/EmbarcaTechSerialCommunication.git
cd EmbarcaTechSerialCommunication
```

2. **Pra o primeiro projeto - Abra o VS Code** e **importe o projeto**:
   - Vá até a **Extensão Raspberry Pi Pico**.
   - Selecione **Import Project**.
   - Escolha a pasta do repositório clonado.
   - Clique em **Import**.

3. **Compilar o código**:
   - Utilize a opção de **Build** da extensão.

4. **Rodar no BitDogLab**:
   - Na extensão do VSCode Raspberry PI PICO Project.
   - Clique em **Run Project(USB)** para enviar o arquivo para a BitDogLab.

## Desenvolvedor

[Décio Francis](https://www.linkedin.com/in/deciofrancis/)

## Vídeo de Demonstração

[Assista ao vídeo de demonstração aqui]()