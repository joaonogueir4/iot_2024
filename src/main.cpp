/*************************
 * Projeto IOT com ESP32
 * Escola SENAI de Informática
 * Curso de Apredizagem industrial
 * Integrador de soluções em nuvem
 * Data: 01/08/2024
 *
 * Autor: Joao Nogueira
 *
 * Descrição: Projeto de automação utilizando ESP32
 * com conexão WiFi e MQTT.
 *
 * versão: 0.9
 *************************/

#include <Arduino.h>
#include <ArduinoJson.h>
#include "iot.h"
#include "saidas.h"
#include "entradas.h"
#include "tempo.h"
#include "atuadores.h"
#include "funcoes.h"
#include "umidificador.h"
#include "motorDC.h"
#include "atuadores.h"
#include "dhtt.h"
// Definição dos tópicos de publicação
#define mqtt_pub_topic1 "projeto/dados"

float distanciaMedida = 0;

// Variáveis globais
unsigned long tempoAnteriorIntervaloPublicacao = 0;
const unsigned long intervaloPublicao = 1000;

// Prototipo das funcoes

void setup()
{
  Serial.begin(115200);
  


  setup_wifi();
  setup_time();
  inicializa_entradas();
  inicializa_saidas();
  randomSeed(esp_random());
  setupUmidificador();
  inicializaSensorTinta();
  MotorSetup();
  Servosetup();
  setupDHT();
   
}

void loop()
{

  atualiza_time();
  atualiza_saidas();
  atualiza_botoes();
  atualiza_mqtt();
  randomiza_senha();
  loopUmidificador();
  medirNivelTinta();
  Motorloop();
  Servoloop();
  loopDHT();
  
  

  JsonDocument doc;
  String json;
  bool mensagemPendente = false;

  if (millis() - tempoAnteriorIntervaloPublicacao >= intervaloPublicao)
  {
    tempoAnteriorIntervaloPublicacao = millis();

    // Adicionando dados comuns ao JSON
    doc["timeStamp"] = timeStamp();
    doc["LedSinal"] = estadoLed;
    doc["UmidificadorState"] = umidificadorLigado; // Estado do umidificador
    doc["nivelTinta"] = round(distanciaMedida * 10) / 10.0;
    doc["motorDC"] = motorLigado;                  // Estado do motor DC
    doc["portaAberta"] = portaAberta;              // Estado da porta
    doc["UmidadeSalaDeControle"] = humidityControle;
    doc["TemperaturaSalaDeControle"] = temperatureControle;
    doc["UmidadeOperação"] = humidityOperacao;
    doc["TemperaturaOperacao"] = temperatureOperacao;

    mensagemPendente = true;
  }

  // Se houver uma mensagem pendente, serialize e publique no MQTT
  if (mensagemPendente)
  {
    String json;
    serializeJsonPretty(doc, json);
    publica_mqtt(mqtt_pub_topic1, json); // Publica no MQTT
    mensagemPendente = false;            // Reseta a flag de mensagem pendente
  }
}

// Variáveis e funções relacionadas à senha
int senha;
const unsigned long intervaloNormal = 30000;
const unsigned long intervaloEstendido = 90000;
unsigned long tempoInicialResetSenha = 0;
unsigned long intervaloResetSenha = 0;

int randomiza_senha()
{
  unsigned long tempoAtual = millis();

  if (tempoAtual - tempoInicialResetSenha >= intervaloResetSenha)
  {

    if (intervaloResetSenha != intervaloNormal)
      intervaloResetSenha = intervaloNormal;
    tempoInicialResetSenha = tempoAtual;
    senha = random(1000, 9999);
    Serial.printf("Nova Senha: %d\n", senha);
  }
  return senha;
}

void tempoSenhaEstendido()
{
  if (intervaloResetSenha != intervaloEstendido)
  {
    unsigned long tempoAtual = millis();
    tempoInicialResetSenha = tempoAtual;
    intervaloResetSenha = intervaloEstendido;
    Serial.println("Senha estendida por 90 segundos");
  }
}