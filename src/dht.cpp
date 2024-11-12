#include "dhtt.h"
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Defina os pinos e tipo do sensor
#define DHTPIN_OPERACAO 4 // Pino para o sensor da sala de operação
#define DHTPIN_CONTROLE 2 // Pino para o sensor da sala de controle
#define DHTTYPE DHT22     // Tipo de sensor, DHT22 ou DHT11

// Instancia os sensores DHT para cada sala
DHT dhtControle(DHTPIN_CONTROLE, DHTTYPE);
DHT dhtOperacao(DHTPIN_OPERACAO, DHTTYPE);

// Variáveis globais para armazenar os dados lidos
float humidityControle = 0;
float temperatureControle = 0;
float humidityOperacao = 0;
float temperatureOperacao = 0;

// Variáveis para controle de tempo
unsigned long previousMillis = 0;
const long interval = 2000; // Intervalo de 2 segundos

// Função de configuração
void setupDHT()
{
    Serial.begin(115200);
    dhtControle.begin();
    dhtOperacao.begin();
    Serial.println("Sensores DHT inicializados para salas de controle e operação.");
}

// Função de loop para leitura dos sensores
void loopDHT()
{
    unsigned long currentMillis = millis();

    // Verifica se o intervalo de tempo já passou
    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis; // Atualiza o tempo

        // Leitura do sensor da sala de controle
        float humidityControle = dhtControle.readHumidity();
        float temperatureControle = dhtControle.readTemperature();

        // Verifica se a leitura é válida
        if (isnan(humidityControle) || isnan(temperatureControle))
        {
            Serial.println("Erro ao ler o sensor DHT da Sala de Controle!");
        }
        else
        {
            Serial.print("Sala de Controle - Umidade: ");
            Serial.print(humidityControle);
            Serial.print("% Temperatura: ");
            Serial.print(temperatureControle);
            Serial.println("°C");
        }

        // Leitura do sensor da sala de operação
        float humidityOperacao = dhtOperacao.readHumidity();
        float temperatureOperacao = dhtOperacao.readTemperature();
        if (isnan(humidityOperacao) || isnan(temperatureOperacao))

        {
            Serial.println("Erro ao ler o sensor DHT da Sala de Operação!");
        }
        else
        {
            Serial.print("Sala de Operação - Umidade: ");
            Serial.print(humidityOperacao);
            Serial.print("% Temperatura: ");
            Serial.print(temperatureOperacao);
            Serial.println("°C");
        }
    }
}