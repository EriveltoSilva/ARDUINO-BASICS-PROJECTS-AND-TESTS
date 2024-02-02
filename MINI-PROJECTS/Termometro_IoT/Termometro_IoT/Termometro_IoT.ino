/**************************************************
    AUTHOR....: Erivelto Silva(Otlevire).
    FOR.......: Filipe Lopes. 
    CREATED AT:14-01-2024.
    Principais Componentes usados:
      *** 1-ESP32 (1)
      *** 2-Sensor de Temperatura e Humidade DHT11 (1)
      *** 3-LCD com I2C (1)
*************************************************/

///////////// PIN DEFINITIONS /////////////////////
#define BLYNK_TEMPLATE_ID "TMPL2Ztxrhb6i"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "GpP_EfpIfB-pgqjVzbo_10dvyALpAZtO"
#define BLYNK_PRINT Serial

#define LED 2                                 /////
#define DHTPIN 15                             /////
#define DHTTYPE DHT11                         /////
///////////////////////////////////////////////////

////////////////// Libraries Used  ////////////////
#include <WiFi.h>               /////
#include <Wire.h>               /////
#include "DHT.h"                /////
#include <WiFiClient.h>         /////
#include <BlynkSimpleEsp32.h>   /////
#include <LiquidCrystal_I2C.h>  /////
///////////////////////////////////////////////////

char ssid[] = "TERMOMETRO";
char pass[] = "123456789";
float temperatura = 0, humidade = 0;

//////////////////// OBJECTS ////////////////////////
BlynkTimer timer;                               /////
DHT dht(DHTPIN, DHTTYPE);                       /////
LiquidCrystal_I2C lcd(0x27, 16, 2);             /////
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////
void sendDataToBlynk() {
  lerSensores();
  printDataLCD();
  Blynk.virtualWrite(V3, humidade);
  Blynk.virtualWrite(V4, temperatura);
  digitalWrite(LED, !digitalRead(LED));
}


/////////////////////////////////////////////////////
void setup() {
  initSetup();
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(2000L, sendDataToBlynk);
}

/////////////////////////////////////////////////////
void loop() {
  Blynk.run();
  timer.run();
}

/////////////////////////////////////////////////////
void initSetup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("#   PROJECTO   #");
  lcd.setCursor(0, 1);
  lcd.print("#    TDAUTO    #");
}

/////////////////////////////////////////////////////
void lerSensores() {
  humidade = dht.readHumidity();
  temperatura = dht.readTemperature();
  if (isnan(humidade) || isnan(temperatura)) {
    Serial.println(F("Falha ao ler o sensor!"));
    temperatura = humidade = 0;
  }
}


void printDataLCD() {
  lcd.init();
  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.print("TEMPERAT:");
  lcd.print(temperatura);
  lcd.print("*C");

  lcd.setCursor(0, 1);
  lcd.print("HUMIDADE:");
  lcd.print(humidade);
  lcd.print("%");
}