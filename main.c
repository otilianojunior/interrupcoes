#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pico/bootrom.h"
#include "interrupcoes.pio.h"

// =============================================
//          DEFINIÇÕES DE HARDWARE
// =============================================
#define LED_RGB_VERDE_PIN 11
#define LED_RGB_AZUL_PIN 12
#define LED_RGB_VERMELHO_PIN 13
#define BOTAO_A_PIN 5
#define BOTAO_B_PIN 6
#define NUM_LEDS 25
#define WS2812_PIN 7
#define TEMPO_DEBOUNCE_MS 400
#define BRILHO_LED 51

// =============================================
//          ESTRUTURAS DE DADOS
// =============================================
typedef struct {
    double verde;
    double vermelho;
    double azul;
} CorLed;

typedef CorLed MatrizLed[5][5];

// =============================================
//          FUNÇÕES
// =============================================
uint32_t cor_para_grb(double vermelho, double verde, double azul);
void atualizar_matriz_led(MatrizLed config, PIO pio, uint sm);
void exibir_numero(int numero, PIO pio, uint sm);
void interrupcao_botoes(uint gpio, uint32_t events);

// =============================================
//          VARIÁVEIS GLOBAIS
// =============================================
volatile uint32_t ultimo_pressionamento_a = 0;
volatile uint32_t ultimo_pressionamento_b = 0;
volatile uint32_t agora = 0;
volatile int numero_atual = 0;

PIO pio_global;
uint sm_global;

// =============================================
//          CONVERSÃO DE CORES
// =============================================
/*
 * Converte as intensidades das cores (valores de 0 a 1) para um número de 32 bits
 * no formato GRB, que é o esperado pelos LEDs WS2812.
 */
uint32_t cor_para_grb(double verde, double vermelho, double azul) {
    return ((uint8_t)(verde * 255) << 16) |
           ((uint8_t)(vermelho * 255) << 8) |
           ((uint8_t)(azul * 255));
}

// =============================================
//          CONTROLE DA MATRIZ LED
// =============================================
void atualizar_matriz_led(MatrizLed config, PIO pio, uint sm) {
    for (int linha = 4; linha >= 0; linha--) {
        bool reverso = (linha % 2 == 0);

        for (int coluna = (reverso ? 4 : 0);
             reverso ? (coluna >= 0) : (coluna < 5);
             reverso ? coluna-- : coluna++) {

            uint32_t cor = cor_para_grb(
                config[linha][coluna].vermelho,
                config[linha][coluna].verde,
                config[linha][coluna].azul);
            pio_sm_put_blocking(pio, sm, cor);
        }
    }
}

// =============================================
//          EXIBIÇÃO DE NÚMEROS
// =============================================
/* 
 * Cada dígito é representado por uma matriz 5x5 onde:
 * 1 indica o LED que deve ser aceso e 0 o LED apagado.
 */
const uint8_t padroes_digitos[10][5][5] = {
    [0] = {{0, 1, 1, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 1, 1, 0}},
    [1] = {{0, 0, 1, 0, 0}, {0, 1, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 1, 1, 1, 0}},
    [2] = {{0, 1, 1, 1, 0}, {0, 0, 0, 1, 0}, {0, 1, 1, 1, 0}, {0, 1, 0, 0, 0}, {0, 1, 1, 1, 0}},
    [3] = {{0, 1, 1, 1, 0}, {0, 0, 0, 1, 0}, {0, 0, 1, 1, 0}, {0, 0, 0, 1, 0}, {0, 1, 1, 1, 0}},
    [4] = {{0, 1, 0, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 1, 0}, {0, 0, 0, 1, 0}},
    [5] = {{0, 1, 1, 1, 0}, {0, 1, 0, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 1, 0}, {0, 1, 1, 1, 0}},
    [6] = {{0, 1, 1, 1, 0}, {0, 1, 0, 0, 0}, {0, 1, 1, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 1, 1, 0}},
    [7] = {{0, 1, 1, 1, 0}, {0, 1, 0, 1, 0}, {0, 0, 0, 1, 0}, {0, 0, 0, 1, 0}, {0, 0, 0, 1, 0}},
    [8] = {{0, 1, 1, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 1, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 1, 1, 0}},
    [9] = {{0, 1, 1, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 1, 0}, {0, 0, 0, 1, 0}}
};

void exibir_numero(int numero, PIO pio, uint sm) {
    MatrizLed matriz = {0};

    for (int linha = 0; linha < 5; linha++) {
        for (int coluna = 0; coluna < 5; coluna++) {
            if (padroes_digitos[numero][linha][coluna]) {
                // Acende apenas o canal vermelho
                matriz[linha][coluna] = (CorLed){BRILHO_LED / 255.0, 0, 0};
            } else {
                matriz[linha][coluna] = (CorLed){0, 0, 0};
            }
        }
    }
    atualizar_matriz_led(matriz, pio, sm);
}

// =============================================
//          INTERRUPÇÕES DE BOTÕES
// =============================================
void interrupcao_botoes(uint gpio, uint32_t events) {
    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());

    if (gpio == BOTAO_A_PIN && (tempo_atual - ultimo_pressionamento_a) > TEMPO_DEBOUNCE_MS) {
        if (numero_atual < 9) {
            numero_atual++;
            printf("Incremento: %d\n", numero_atual);
        }
        ultimo_pressionamento_a = tempo_atual;
    } 
    
    if (gpio == BOTAO_B_PIN && (tempo_atual - ultimo_pressionamento_b) > TEMPO_DEBOUNCE_MS) {
        if (numero_atual > 0) {
            numero_atual--;
            printf("Decremento: %d\n", numero_atual);
        }
        ultimo_pressionamento_b = tempo_atual;
    }
    // A atualização da matriz é feita no loop principal para evitar conflitos
}

// =============================================
//          CONFIGURAÇÃO INICIAL
// =============================================
void inicializar_hardware(PIO pio, uint *sm) {
    set_sys_clock_khz(128000, false);
    stdio_init_all();

    // Configuração WS2812
    uint offset = pio_add_program(pio, &ws2812_program);
    *sm = pio_claim_unused_sm(pio, true);
    ws2812_program_init(pio, *sm, offset, WS2812_PIN, 800000, false);

    pio_global = pio;
    sm_global = *sm;

    // Configuração LED RGB
    const uint pins_rgb[] = {LED_RGB_VERMELHO_PIN, LED_RGB_VERDE_PIN, LED_RGB_AZUL_PIN};
    for (int i = 0; i < 3; i++) {
        gpio_init(pins_rgb[i]);
        gpio_set_dir(pins_rgb[i], GPIO_OUT);
    }

    // Configuração Botões com resistor de pull-up
    const uint pins_botoes[] = {BOTAO_A_PIN, BOTAO_B_PIN};
    for (int i = 0; i < 2; i++) {
        gpio_init(pins_botoes[i]);
        gpio_set_dir(pins_botoes[i], GPIO_IN);
        gpio_pull_up(pins_botoes[i]);
    }

    // Configuração de interrupções para os botões
    gpio_set_irq_enabled_with_callback(BOTAO_A_PIN, GPIO_IRQ_EDGE_FALL, true, (gpio_irq_callback_t)interrupcao_botoes);
    gpio_set_irq_enabled_with_callback(BOTAO_B_PIN, GPIO_IRQ_EDGE_FALL, true, (gpio_irq_callback_t)interrupcao_botoes);
}

// =============================================
//          LOOP PRINCIPAL
// =============================================
int main() {
    stdio_init_all();
    printf("Iniciando...\n");
    
    PIO pio = pio0;
    uint sm = 0;
    inicializar_hardware(pio, &sm);

    absolute_time_t ultimo_piscar = get_absolute_time();

    while (true) {
        agora = to_ms_since_boot(get_absolute_time());

        // Piscar o LED vermelho do LED RGB (5Hz)
        if (absolute_time_diff_us(ultimo_piscar, get_absolute_time()) > 100000) {
            gpio_put(LED_RGB_VERMELHO_PIN, !gpio_get(LED_RGB_VERMELHO_PIN));
            ultimo_piscar = get_absolute_time();
        }

        // Atualiza a matriz WS2812 com o número atual
        exibir_numero(numero_atual, pio, sm);

        sleep_ms(10);
    }
    
    return 0;
}
