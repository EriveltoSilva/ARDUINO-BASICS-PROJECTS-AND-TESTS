/**************************************************
    AUTHOR....: Erivelto Silva(Otlevire).
    FOR.......: Abel & Sousa. 
    CREATED AT:16-12-2023.
    Principais Componentes usados:
      *** 1-ArduinoUno (1)
      *** 2-Sensor de Tensão ZMPT101B (2)
      *** 3-Sensor de Tensão Continua DC 0-25V (1)
      *** 4-Sensor de Corrente ACS712 30A (1)
      *** 5-Sensor de Temperatura e Humidade DHT11 (1)
      *** 6-LCD com I2C (1)
*************************************************/

////////////////// Libraries Used  ////////////////
#include "DHT.h"                /////
#include "EmonLib.h"            /////
#include <SoftwareSerial.h>     /////
#include <LiquidCrystal_I2C.h>  /////
///////////////////////////////////////////////////

///////////// PIN DEFINITIONS /////////////////////
#define TIME_START_GERADOR 3000               /////
#define PIN_VOLTAGE_BATERIA A0                /////
#define PIN_VOLTAGE_GERADOR A1                /////
#define PIN_VOLTAGE_REDE A2                   /////
#define PIN_CURRENT A3                        /////
#define DHTPIN 4                              /////
#define LIGAR_GERADOR 6                       /////
#define DESLIGAR_GERADOR 5                    /////
#define BTN_GERADOR 7                         /////
#define LED 13                                /////
#define DHTTYPE DHT11                         /////
///////////////////////////////////////////////////

String dados = "";
bool umaVez = false;
char statusModo = 'M';
byte numeroTentativas = 0;
float tensaoRede = 0, potenciaRede = 0;
float tensaoGerador = 0, potenciaGerador = 0;
float tensaoBateria = 0, corrente = 0;
float temperatura = 0, humidade = 0;
unsigned long int timeDelay = 0, tempoLigarGerador = 0;
bool flagBotao = false;
bool btnGeradorEstadoAnterior= false;

//////////////////// OBJECTS ////////////////////////
EnergyMonitor emonRede;                         /////
DHT dht(DHTPIN, DHTTYPE);                       /////
EnergyMonitor emonGerador;                      /////
SoftwareSerial esp32(2, 3);  // RX, TX          /////
LiquidCrystal_I2C lcd(0x3F, 20, 4);             /////
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////
void piscar();
void analise();
void lerSensores();
void receiveData();
void ligarGerador();
void printDataLCD();
bool haEnergiaRede();
void desligarGerador();
bool haEnergiaGerador();
/////////////////////////////////////////////////////

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(LIGAR_GERADOR, OUTPUT);
  pinMode(DESLIGAR_GERADOR, OUTPUT);
  pinMode(PIN_VOLTAGE_BATERIA, INPUT);
  pinMode(BTN_GERADOR, INPUT_PULLUP);

  digitalWrite(LED, LOW);
  digitalWrite(LIGAR_GERADOR, HIGH);
  digitalWrite(DESLIGAR_GERADOR, HIGH);
  btnGeradorEstadoAnterior = digitalRead(BTN_GERADOR);
  
  delay(200);

  Serial.begin(9600);
  delay(1000);
  esp32.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("#**SMART GERADOR**#");
  lcd.setCursor(0, 1);
  lcd.print("BY:ABEL & SOUSA");
  lcd.setCursor(-4, 2);
  lcd.print("UNIV. METODISTA");
  lcd.setCursor(-4, 3);
  lcd.print("  PROJECTO FINAL   ");


  dht.begin();
  delay(500);


  //Voltage:input pin, calibration, phase_shift
  emonRede.voltage(PIN_VOLTAGE_REDE, 222.5, 1.7);
  //Current: input pin, calibration..
  emonRede.current(PIN_CURRENT, 0.9);

  //Voltage:input pin, calibration, phase_shift
  emonGerador.voltage(PIN_VOLTAGE_GERADOR, 325, 1.7);

  Serial.println("SISTEMA LIGADO. TUDO PRONTO!");
  delay(3000);
}

void loop() {
  receiveData();
  if (millis() - timeDelay > 1500) {
    timeDelay = millis();
    lerSensores();
    printDataLCD();
    analise();
    digitalWrite(LED, !digitalRead(LED));
  }

  if (digitalRead(BTN_GERADOR) != btnGeradorEstadoAnterior) {
    btnGeradorEstadoAnterior = digitalRead(BTN_GERADOR);    
    if (statusModo == 'M') {
      if (haEnergiaGerador())
        desligarGerador();
      else
        ligarGerador();
    }
  }
  
  delay(2);
}

void lerSensores() {
  humidade = dht.readHumidity();
  temperatura = dht.readTemperature();

  float tensaoEntrada = (analogRead(PIN_VOLTAGE_BATERIA) * 5.0) / 1023.0;
  tensaoBateria = tensaoEntrada * 5;

  if (isnan(humidade) || isnan(temperatura)) {
    Serial.println(F("Falha ao ler o sensor!"));
    temperatura = humidade = 0;
  }


  emonRede.calcVI(20, 2000);
  emonGerador.calcVI(20, 2000);


  tensaoRede = emonRede.Vrms;
  tensaoGerador = emonGerador.Vrms;

  corrente = emonRede.Irms;
  if (tensaoRede < 50) tensaoRede = 0;
  if (tensaoGerador < 50) tensaoGerador = 0;
  if( tensaoRede==0 && tensaoGerador==0) corrente=0;

  potenciaRede = tensaoRede * corrente;
  potenciaGerador = (haEnergiaRede()) ? 0 : tensaoGerador * corrente;

  Serial.println("Temperatura:" + String(temperatura) + "*C");
  Serial.println("Humidade:" + String(humidade) + "%");
  Serial.println("Tensão DC da Bateria:" + String(tensaoBateria) + "V");
  Serial.println("Tensão da Rede:" + String(tensaoRede) + "V");
  Serial.println("Tensão do Gerador:" + String(tensaoGerador) + "V");
  Serial.println("Corrente Medida:" + String(corrente) + "A");
  Serial.println("----------------------------------------------------");

  dados = "D*" + String(temperatura) + "*" + String(humidade) + "*" + String(corrente) + "*";
  dados += String(tensaoRede) + "*" + String(potenciaRede) + "*";
  dados += String(tensaoGerador) + "*" + String(potenciaGerador) + "*" + String(tensaoBateria) + "*";
  dados += String(statusModo) + "*" + String((tensaoGerador > 100) ? "1" : "0") + "*";
}

void piscar() {
  for (byte i = 0; i < 10; i++) {
    digitalWrite(LED, !digitalRead(LED));
    delay(75);
  }
}

void ligarGerador() {
  digitalWrite(LIGAR_GERADOR, HIGH);
  digitalWrite(LIGAR_GERADOR, LOW);
  delay(TIME_START_GERADOR);
  digitalWrite(LIGAR_GERADOR, HIGH);
}

void desligarGerador() {
  digitalWrite(DESLIGAR_GERADOR, HIGH);
  digitalWrite(DESLIGAR_GERADOR, LOW);
  delay(TIME_START_GERADOR);
  digitalWrite(DESLIGAR_GERADOR, HIGH);
}

void receiveData() {
  if (esp32.available()) {
    while (esp32.available()) {
      char rx = (char)esp32.read();
      switch (rx) {
        case 'M': statusModo = (statusModo == 'M') ? 'A' : 'M'; break;
        case 'D':
          Serial.println("ENVIANDO DADOS");
          esp32.println(dados);
          piscar();
          break;
        case 'G':
          if (statusModo == 'M') {
            if (haEnergiaGerador()) {
              umaVez = 1;
              desligarGerador();
            } else
              ligarGerador();
          }
          break;
      }
    }
  }

  if (Serial.available()) {
    while (Serial.available()) {
      char rx = (char)Serial.read();
      switch (rx) {
        case 'M': statusModo = (statusModo == 'M') ? 'A' : 'M'; break;
        case 'D':
          Serial.println(dados);
          piscar();
          break;
        case 'G': ligarGerador(); break;
        case 'g': desligarGerador(); break;
      }
    }
  }
}

bool haEnergiaRede() {
  return (tensaoRede > 150);
}

bool haEnergiaGerador() {
  return (tensaoGerador > 150);
}

//////////////////////////////////////////////////////////
void printDataLCD() {
  static byte flag = 0;

  lcd.init();
  lcd.clear();
  lcd.setCursor(0, 0);

  switch (flag) {
    case 0:
      lcd.print("TEMPERAT:");
      lcd.print(temperatura);
      lcd.print("*C");

      lcd.setCursor(0, 1);
      lcd.print("HUMIDADE:");
      lcd.print(humidade);
      lcd.print("%");

      lcd.setCursor(-4, 2);
      lcd.print("CORRENTE:");
      lcd.print(corrente);
      lcd.print("A");

      lcd.setCursor(-4, 3);
      lcd.print("VOLT.BAT:");
      lcd.print(tensaoBateria);
      lcd.print("V");
      break;

    case 1:
      lcd.print("VOLT.REDE:");
      lcd.print(tensaoRede);
      lcd.print("V");

      lcd.setCursor(0, 1);
      lcd.print("POTE.REDE:");
      lcd.print(potenciaRede);
      lcd.print("W");

      lcd.setCursor(-4, 2);
      lcd.print("VOLT.GER :");
      lcd.print(tensaoGerador);
      lcd.print("V");

      lcd.setCursor(-4, 3);
      lcd.print("POTEN.GER:");
      lcd.print(potenciaGerador);
      lcd.print("W");

      break;

    case 2:

      lcd.print("ENER.REDE: ");
      lcd.print((haEnergiaRede()) ? "ON" : "OFF");

      lcd.setCursor(0, 1);
      lcd.print("GERADOR: ");
      lcd.print((haEnergiaGerador()) ? "ON" : "OFF");

      lcd.setCursor(-4, 2);
      lcd.print("MODO:");
      lcd.print((statusModo == 'M') ? "MANUAL" : "AUTOMATICO");
      break;
  }

  if (++flag > 2) flag = 0;
}

void analise() {
  if (statusModo == 'A') {
    if (!haEnergiaRede() && umaVez == 0) {
      umaVez = 1;
      tempoLigarGerador = millis();
      Serial.println("Timer ligado! Cinco segundos para ativar!");
    } else if (haEnergiaRede() && umaVez == 1) {
      umaVez = 0;
      desligarGerador();
      Serial.println("Gerador Desligado- uma vez agora e 0");
    }

    if (!haEnergiaRede() && !haEnergiaGerador() && (numeroTentativas < 5) && ((millis() - tempoLigarGerador) > 5000)) {
      numeroTentativas++;
      umaVez = 0;
      ligarGerador();
      Serial.print("Ligando Gerador- uma vez agora e 0, nTentativas:");
      Serial.println(numeroTentativas);
    }

    else if (!haEnergiaRede() && haEnergiaGerador()) {
      umaVez = 1;
      numeroTentativas = 0;
      Serial.print("Zerando nTentativas- uma vez agora e 1, nTentativas:");
      Serial.println(numeroTentativas);
    } else if (haEnergiaRede()) {
      umaVez = 0;
      numeroTentativas = 0;
      Serial.print("Zerando nTentativas- uma vez agora e 0, nTentativas:");
      Serial.println(numeroTentativas);
    }
  }
}
