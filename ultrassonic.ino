// Definição dos Pinos e Constantes (facilita a manutenção)
#define PIN_TRIGGER   7
#define PIN_ECHO      8
#define PIN_LED_RED   12
#define PIN_LED_GREEN 11
#define PIN_BUZZER    10

const float DISTANCIA_ALERTA = 15.0; // Distância limite em cm
const int NUM_LEITURAS = 5;          // Quantidade de amostras para a média

void setup () {
    Serial.begin(9600);
    
    // Configuração dos pinos do sensor ultrassônico
    pinMode(PIN_TRIGGER, OUTPUT);
    pinMode(PIN_ECHO, INPUT);
    
    // Configuração dos pinos de alerta
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);
}

// Função nativa para ler a distância sem biblioteca
float medirDistanciaCm() {
    // Garante que o pino trigger inicie em nível baixo (desligado)
    digitalWrite(PIN_TRIGGER, LOW);
    delayMicroseconds(2);
    
    // Envia um pulso alto de 10 microssegundos para acionar o disparo sonoro
    digitalWrite(PIN_TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIGGER, LOW);
    
    // Mede o tempo de resposta no pino Echo com timeout de 30ms (~5 metros)
    long duracao = pulseIn(PIN_ECHO, HIGH, 30000);
    
    // Se a duração for 0, o som não retornou (fora do alcance)
    if (duracao == 0) {
        return -1.0; 
    }
    
    // Distância = (Tempo * Velocidade do Som no ar (0.0343 cm/µs)) / 2 (ida e volta)
    return (duracao * 0.0343) / 2.0;
}

void loop () {
    float distancia_total = 0.0;
    int leituras_validas = 0;
    
    // Calcula a média filtrando falhas do sensor
    for (int i = 0; i < NUM_LEITURAS; i++){
      float leitura = medirDistanciaCm();
      
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