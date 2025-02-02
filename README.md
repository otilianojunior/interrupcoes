<p align="center">
  <img src="assets/logo.jpeg" alt="Logo do Projeto" width="50%">
</p>

---

# 💡 Projeto: Interrupções com RP2040 e BitDogLab

## 📋 Descrição Geral

Este projeto tem como objetivo consolidar a compreensão dos conceitos relacionados ao uso de **interrupções** no microcontrolador **RP2040**, explorando as funcionalidades da placa de desenvolvimento **BitDogLab**. Ao longo do desenvolvimento, você aplicará técnicas essenciais como:

- **Interrupções via IRQ**
- **Debouncing** dos botões
- Controle de **LEDs comuns** (como o LED RGB)
- Controle de **LEDs endereçáveis WS2812**
- Utilização dos resistores **pull-up internos**

A proposta integra hardware e software para criar uma aplicação interativa e funcional.

---

## 🎯 Objetivos

- **Interrupções:** Compreender e aplicar o conceito de interrupções em microcontroladores.
- **Debouncing:** Corrigir o fenômeno do bouncing nos botões por meio de tratamento via software.
- **Controle de LEDs:** Gerenciar tanto LEDs comuns quanto LEDs endereçáveis para exibição de informações.
- **Uso de Pull-up:** Explorar o uso dos resistores pull-up internos nos botões.
- **Integração Hardware/Software:** Desenvolver um projeto que combine de forma harmônica entradas (botões) e saídas (LEDs e matriz de exibição).

---

## 🛠 Componentes Utilizados

- **Matriz 5x5 de LEDs WS2812 (endereçáveis):**
  - **GPIO:** Conectada à GPIO 7.
  - **Função:** Exibe números de 0 a 9 com efeitos visuais.
- **LED RGB Comum:**
  - **GPIOs:** Conectado às GPIOs 11 (verde), 12 (azul) e 13 (vermelho).
  - **Função:** O LED vermelho deve piscar continuamente 5 vezes por segundo.
- **Botão A:**
  - **GPIO:** Conectado à GPIO 5.
  - **Função:** Incrementa o número exibido na matriz a cada pressionamento.
- **Botão B:**
  - **GPIO:** Conectado à GPIO 6.
  - **Função:** Decrementa o número exibido na matriz a cada pressionamento.

---

## 🗂 Estrutura do Projeto

```plaintext
Interrupcoes/
├── assets
│   ├── logo.jpeg
│   └── placa.gif
│   └── wokwi.gif
├── wokwi
│   ├── diagram.json
│   └── wokwi.toml
├── .gitignore
├── CMakeLists.txt
├── LICENSE
├── main.c
├── pico_sdk_import.cmake
└── README.md
```

---

## 🚀 Funcionalidades do Projeto

1. **LED RGB – Piscar Vermelho:**

   - O LED vermelho pisca continuamente com uma frequência de 5 vezes por segundo, servindo como indicação visual do funcionamento.

2. **Interrupções e Debouncing nos Botões:**

   - **Botão A:** Incrementa o valor exibido na matriz de LEDs sempre que pressionado.
   - **Botão B:** Decrementa o valor exibido na matriz de LEDs sempre que pressionado.
   - **Debouncing:** Implementado via software para eliminar o efeito do bouncing, garantindo leituras precisas.

3. **Exibição na Matriz WS2812:**

   - Os LEDs endereçáveis exibem números de 0 a 9 com efeitos visuais.
   - O formato pode ser fixo (ex.: estilo digital com segmentos iluminados) ou criativo, desde que o número seja claramente identificado.

4. **Demonstração:**
   - A simulação foi feita na pasta wokwi, após compilar o projeto pode ser executado o arquivo diagram.json para para visualizar o comportamento do projeto de interrupções.

> _Observação:_ O diagrama original da matriz de LEDs foi adaptado a partir do repositório do professor [Wilton Lacerda Silva](https://github.com/wiltonlacerda) e modificado para esta atividade.

---

## 🔧 Requisitos Técnicos

- **Interrupções:** Utilização de rotinas de interrupção (IRQ) para tratar as entradas dos botões.
- **Debouncing:** Implementação obrigatória para o tratamento do bouncing dos botões.
- **Controle de LEDs:** Domínio na manipulação dos diferentes tipos de LEDs.
- **Código Organizado:** O código deve ser bem estruturado e comentado, facilitando a manutenção e o entendimento.

---

## ⚙️ Instalação e Execução

### 1. Configuração do Ambiente

- Certifique-se de que o [Pico SDK](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf) esteja instalado e configurado corretamente no seu sistema.
- Verifique se todas as dependências necessárias estão instaladas.

### 2. Clonando o Repositório

Clone o repositório para sua máquina:

```bash
git clone https://github.com/otilianojunior/interrupcoes.git
```

### 3. Compilação e Envio do Código

No VS Code com o ambiente do Pico SDK configurado, execute os comandos abaixo:

```bash
mkdir build
cd build
cmake ..
make
```

Após a compilação, copie o arquivo `.uf2` gerado para o Raspberry Pi Pico (certifique-se de que a placa esteja no modo bootloader).

### 4. Testes

- **Simulação no Wokwi:**  
 Utilizando a plataforma [Wokwi](https://wokwi.com/projects/420549512857480193) é possível testar o código.

  <p align="center">
  <img src="assets/wokwi.gif" alt="Simulação no Wokwi">
</p>

---

- **Execução na placa RP2040 - BitDogLab:**  
  <p align="center">
  <img src="assets/placa.gif" alt="Execução na Placa RP2040 - BitDogLab">
</p>

---

## 📁 Entregáveis

- Código-fonte presente neste repositório.
- Vídeo demonstrativo: [Vídeo](https://drive.google.com/file/d/10bey8MsrKD87hNsXohAC1H6DWAOKdtKM/view?usp=sharing)

---

## ✅ Conclusão

Este projeto é uma excelente oportunidade para praticar conceitos fundamentais em sistemas embarcados, como o uso de interrupções, debouncing e controle de LEDs. Ao integrar hardware e software de maneira organizada e comentada, você desenvolverá habilidades essenciais para a criação de aplicações interativas com o microcontrolador RP2040.

---

_Desenvolvido por Otiliano Junior_

---
