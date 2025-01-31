 #include "pico/stdlib.h"
#include "hardware/gpio.h"

// Definindo os pinos do teclado matricial
#define ROWS 4
#define COLS 4
int rowPins[ROWS] = {2, 3, 4, 5}; // Linhas do teclado
int colPins[COLS] = {6, 7, 8, 9}; // Colunas do teclado

// Mapeamento das teclas do teclado matricial
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Definindo os pinos dos LEDs
#define LED_VERMELHO 10
#define LED_VERDE 11
#define LED_AZUL 12

// Função para inicializar os pinos do teclado e LEDs
void setup() {
    // Inicializando pinos de linhas do teclado
    for (int i = 0; i < ROWS; i++) {
        gpio_init(rowPins[i]);
        gpio_set_dir(rowPins[i], GPIO_IN);
        gpio_pull_up(rowPins[i]); // Configura as linhas como entradas com pull-up
    }

    // Inicializando pinos de colunas do teclado
    for (int j = 0; j < COLS; j++) {
        gpio_init(colPins[j]);
        gpio_set_dir(colPins[j], GPIO_OUT);
        gpio_put(colPins[j], 1); // Colunas em nível alto
    }

    // Inicializando LEDs
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_init(LED_AZUL);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
}

// Função para verificar se uma tecla foi pressionada
char scanKeypad() {
    for (int j = 0; j < COLS; j++) {
        gpio_put(colPins[j], 0); // Coloca a coluna em LOW

        for (int i = 0; i < ROWS; i++) {
            if (!gpio_get(rowPins[i])) { // Se a linha estiver em LOW, a tecla foi pressionada
                while (!gpio_get(rowPins[i])); // Aguarda a tecla ser solta
                return keys[i][j]; // Retorna a tecla pressionada
            }
        }

        gpio_put(colPins[j], 1); // Coloca a coluna de volta em HIGH
    }
    return 0; // Retorna 0 se nenhuma tecla for pressionada
}

int main() {
    setup(); // Configura os pinos

    while (true) {
        char key = scanKeypad(); // Verifica se alguma tecla foi pressionada

        // Apaga os LEDs antes de acender o correto
        gpio_put(LED_VERMELHO, 0);
        gpio_put(LED_VERDE, 0);
        gpio_put(LED_AZUL, 0);

        // Ação dos LEDs com base nas teclas pressionadas
        if (key == '1' || key == '4' || key == '7' || key == '*') {
            gpio_put(LED_VERMELHO, 1); // Acende o LED vermelho
        } else if (key == '2' || key == '5' || key == '8' || key == '0') {
            gpio_put(LED_VERDE, 1); // Acende o LED verde
        } else if (key == '3' || key == '6' || key == '9' || key == '#') {
            gpio_put(LED_AZUL, 1); // Acende o LED azul
        }

        // Nenhum LED acende para as teclas A, B, C ou D
        sleep_ms(100); // Delay para evitar leituras rápidas demais
    }

    return 0;
}