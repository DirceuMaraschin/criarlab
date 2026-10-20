#include <HCSR04.h>

// Definição dos Pinos e Constantes (facilita a manutenção)
#define PIN_TRIGGER   7
#define PIN_ECHO      8
#define PIN_LED_RED   12
#define PIN_LED_GREEN 11
#define PIN_BUZZER    10

const float DISTANCIA_ALERTA = 15.0; // Distância limite em cm
const int NUM_LEITURAS = 5;          // Quantidade de amostras para a média

UltraSonicDistanceSensor distanceSensor(PIN_TRIGGER, PIN_ECHO);   

void setup () {
    Serial.begin(9600);
    
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);
}

void loop () {
    float distancia_total = 0.0;
    int leituras_validas = 0;
    
    // Calcula a média filtrando falhas do sensor
    for (int i = 0; i < NUM_LEITURAS; i++){
      float leitura = distanceSensor.measureDistanceCm();
      
      // Só contabiliza a leitura se for válida (maior que zero)
      if (leitura > 0) {
        distancia_total += leitura;
        leituras_validas++;
      }
      delay(20); 
    }

    // Só executa a lógica se pelo menos 1 leitura foi bem-sucedida
    if (leituras_validas > 0) {
      float distancia_media = distancia_total / leituras_validas;

      Serial.print(distancia_media);
      Serial.println(" cm");
      
      if (distancia_media < DISTANCIA_ALERTA) {
        digitalWrite(PIN_LED_RED, HIGH);
        digitalWrite(PIN_LED_GREEN, LOW);
        digitalWrite(PIN_BUZZER, HIGH);
      } else {
        digitalWrite(PIN_LED_RED, LOW);
        digitalWrite(PIN_LED_GREEN, HIGH);
        digitalWrite(PIN_BUZZER, LOW);
      } 
    } else {
      Serial.println("Erro: Objeto muito longe ou muito perto");
      digitalWrite(PIN_BUZZER, LOW); // Prevenção: desliga o alarme no erro
    }
}