/*
*Autor: Erivelto Clénio da Costa e Silva
*Data de Inicio: 04-07-2022
*Data de Ultima Actualização:05/07/2022
*Para: Salomão e Colegas
*Instituto Médio: 
*Descrição: Sistema de Irrigação Inteligente
*Objectivo: Fazer um sistema que permite monitorar o nivel de humidadeAmbiente de solo de uma plantação,
*bem como controlar a temperatura, humidadeAmbiente ambiente e o nível e intensidade de chuvas.
*Microcontrolador: ESP32
*Sensores: DHT11(Temperatura e humidadeAmbiente), humidadeAmbiente de solo, Chuva
*Atuadores: Bomba
*/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPLbTdntheI"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "2NmrFW53-PVDn9C2UAtVG8m5WXRsCnmV"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#define DHTTYPE DHT11

#define BOMBA 2
#define DHTPIN 15
#define SENSOR_SOLO 34
#define SENSOR_CHUVA 35

#include "DHT.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "OTLEVIRE";
char pass[] = "123456789";

BlynkTimer timer;
DHT dht(DHTPIN, DHTTYPE);


//Objectivo: Receber o estado do botão e ligar a bomba caso receba 1,
//           e desligar a bomba caso recebem 0.
BLYNK_WRITE(V0)
{
  // Recebe o valor do pin V0 para uma variavel.
  int value = param.asInt();
  if(value)
    ligarBomba();
  else
    desligarBomba();
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

//Objectivo: Função chamada a casa 2segundo que envia os dados ao Blynk
void enviarDadosParaBlynk()
{
  //V0--bomba(int)
  //V4--Humid. Solo(int)
  //V5--Temperatura(float)
  //V6--Humid.Ambiente(float)
  //V7--Clima(string)
  
  float humidadeAmbiente = dht.readHumidity();
  float temperatura = dht.readTemperature();

  // Verifica se os dados dos sensores são váidos.
  if (isnan(humidadeAmbiente) || isnan(temperatura)) {
    temperatura = humidadeAmbiente=1;
  }

  int humidadeSolo = map(analogRead(SENSOR_SOLO), 0, 4095, 0, 100);
  int chuva = 100- map(analogRead(SENSOR_CHUVA), 0, 4095, 0, 100); 

  String estadoChuva = "";
  if(chuva>70)
    estadoChuva="Chuva Forte-";
  else if(chuva>50)
    estadoChuva="Chuva Normal-";
  else if(chuva>20)
    estadoChuva = "Serenando-";
  else
    estadoChuva = "Sem Chuva-";

  estadoChuva += (String) chuva;
  estadoChuva += "%";

  //Parte que envia os dados ao Blynk
  Blynk.virtualWrite(V4, humidadeSolo);
  Blynk.virtualWrite(V5, temperatura);
  Blynk.virtualWrite(V6, humidadeAmbiente);
  Blynk.virtualWrite(V7, estadoChuva);
}

void setup()
{
  pinMode(BOMBA, OUTPUT);
  digitalWrite(BOMBA, LOW);
  dht.begin();
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  // Chama a função para enviar os dados no Blynk a cada 2 segundos
  timer.setInterval(2000L, enviarDadosParaBlynk);
}

void loop()
{
  Blynk.run();
  timer.run();
}

void ligarBomba()
{
  digitalWrite(BOMBA, HIGH);  
}

void desligarBomba()
{
  digitalWrite(BOMBA, LOW);  
}
