/*
**dois semaforos
**três ultrassonicos
**lcd 16x02
**um motor
**botao modo manual
*/

////////////////////////// INCLUSÃO ///////////////////////////////
#include <Wire.h>
#include <NewPing.h>
#include <LiquidCrystal_I2C.h>

#define MAX_DISTANCE_FRONT 30
#define MAX_DISTANCE_BACK 30
#define TIME_TO_CLOSE_BRIDGE 5000

#define LED 13
#define TRIGGER_FRENTE_1 4
#define ECHO_FRENTE_1 5

#define TRIGGER_FRENTE_2 6
#define ECHO_FRENTE_2 7

#define TRIGGER_TRAS 8
#define ECHO_TRAS 9

#define BUZZER 10
#define MOTOR_BRIDGE_OPEN 11
#define MOTOR_BRIDGE_CLOSE 12

//Funciones
#define led_verde_1 22
#define led_amarelo_1 23
#define led_vermelho_1 24

#define led_verde_2 25
#define led_amarelo_2 26
#define led_vermelho_2 27

#define SWITCH_MODE 28
#define BUTTON_OPEN_CLOSE 29

#define LIMIT_BRIDGE_OPEN 31
#define LIMIT_BRIDGE_CLOSE 32

//Recursos
char mode = 'A';
char modeAnt = 'A';

bool flagBarco = false;
bool flagAlert = false;
bool flagAbrindo = false;
bool flagPassando = false;
bool flagFechando = false;

int distanciaFrente1 = 0;
int distanciaFrente2 = 0;
int distanciaTras = 0;
String distanciaBarco = "";
String estadoSemaforo = "";
unsigned long int tempoDelay = 0, tempoBarco = 0;

//********************************  *************************************
LiquidCrystal_I2C lcd(0x27, 16, 2);  //Set the LCD address to 0x27 for a 16x02
NewPing ultraFrente1(TRIGGER_FRENTE_1, ECHO_FRENTE_1, MAX_DISTANCE_FRONT);
NewPing ultraFrente2(TRIGGER_FRENTE_2, ECHO_FRENTE_2, MAX_DISTANCE_FRONT);
NewPing ultraTras(TRIGGER_TRAS, ECHO_TRAS, MAX_DISTANCE_BACK);

void modeRed();
void modeGreen();
void modeYellow();
void modeOff();
void lerSensores();
void turnOnAlarm();
void turnOffAlarm();

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, HIGH);

  pinMode(MOTOR_BRIDGE_CLOSE, OUTPUT);
  digitalWrite(MOTOR_BRIDGE_CLOSE, LOW);

  pinMode(MOTOR_BRIDGE_OPEN, OUTPUT);
  digitalWrite(MOTOR_BRIDGE_OPEN, LOW);

  pinMode(SWITCH_MODE, INPUT_PULLUP);
  pinMode(BUTTON_OPEN_CLOSE, INPUT_PULLUP);
  pinMode(LIMIT_BRIDGE_OPEN, INPUT_PULLUP);
  pinMode(LIMIT_BRIDGE_CLOSE, INPUT_PULLUP);


  //Semaforo 1
  pinMode(led_verde_1, OUTPUT);
  pinMode(led_amarelo_1, OUTPUT);
  pinMode(led_vermelho_1, OUTPUT);
  digitalWrite(led_verde_1, HIGH);
  digitalWrite(led_amarelo_1, LOW);
  digitalWrite(led_vermelho_1, LOW);

  //Semaforo 2
  pinMode(led_verde_2, OUTPUT);
  pinMode(led_amarelo_2, OUTPUT);
  pinMode(led_vermelho_2, OUTPUT);
  digitalWrite(led_verde_2, HIGH);
  digitalWrite(led_amarelo_2, LOW);
  digitalWrite(led_vermelho_2, LOW);
  delay(200);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" UNIV.METODISTA");
  lcd.setCursor(0, 1);
  lcd.print(" PROJECTO FINAL");
  Serial.begin(9600);
  delay(2500);
  digitalWrite(BUZZER, LOW);

  modeGreen();
}


void loop() {
  verifyMode();
  verifyClosing();
  readButton();

  if (millis() - tempoDelay > 500) {
    tempoDelay = millis();
    lerSensores();
    printDataLCD(lcd);
    digitalWrite(LED, !digitalRead(LED));
  }

  if (mode == 'A') {
    if (flagAbrindo && flagBarco) {
      if (!isBridgeClose()) {
        modeRed();
        openBridge();
        turnOnAlarm();
        Serial.println("Abrinco, ha barco");
        printManualLCD(lcd, "     PONTE      ", "    ABRINDO     ");
        while (!isBridgeClose())
          ;
        stopBridge();
        flagAbrindo = false;
        Serial.println("Solte o botão");
        Serial.println("## ponte aberta ##");
        printManualLCD(lcd, "     PONTE      ", "### ABERTA ###");
        turnOffAlarm();
      }
    }
  }

  if ((millis() - tempoBarco > TIME_TO_CLOSE_BRIDGE) && flagPassando) {
    flagBarco = false;
    flagPassando = false;
    flagFechando = true;
    Serial.println("--> PONTE FECHANDO");
    if (mode == 'A') {
      modeRed();
      closeBridge();
      turnOnAlarm();
      Serial.println("FECHANDO...");
      printManualLCD(lcd, "     PONTE      ", "   FECHANDO     ");
      while (!isBridgeOpen());
      stopBridge();
      flagBarco = false;
      flagPassando = false;
      modeGreen();
      Serial.println("Solte o botão");
      printManualLCD(lcd, "     PONTE      ", "### FECHADA ###");
      turnOffAlarm();
      delay(2000);
    }
  }
  delay(10);
}



void lerSensores() {
  distanciaFrente1 = ultraFrente1.ping_cm();
  distanciaFrente2 = ultraFrente2.ping_cm();
  distanciaTras = ultraTras.ping_cm();

  Serial.println("DistanciaFrente1:" + String(distanciaFrente1));
  Serial.println("DistanciaFrente2:" + String(distanciaFrente2));
  Serial.println("DistanciaTras:" + String(distanciaTras));
  Serial.println("----------------------------------------------\n");

  if (((distanciaFrente1 > 15) || (distanciaFrente2 > 15)) && mode=='A') {
    flagBarco = true;
    flagAlert = true;
    distanciaBarco = (distanciaFrente1 > 5) ? "NAVIO Á:" + String(distanciaFrente1) : "NAVIO Á:" + String(distanciaFrente2);
    modeAlert();
  } else if (((distanciaFrente1 > 5) || (distanciaFrente2 > 5)) && mode=='A') {
    flagBarco = true;
    flagAlert = false;
    distanciaBarco = (distanciaFrente1 > 5) ? "NAVIO Á:" + String(distanciaFrente1) : "NAVIO Á:" + String(distanciaFrente2);
    flagAbrindo = true;

  } 
  else if((distanciaFrente1 < 2) || (distanciaFrente2 < 2) && digitalRead(LIMIT_BRIDGE_CLOSE) && mode=='A')
  {
    if(flagAlert)
    {
      flagAlert = false;
      flagBarco = false;
      modeGreen();
    }
  }
  
  Serial.println("LIMIT_CLOSE:"+String(digitalRead(LIMIT_BRIDGE_CLOSE)));
  Serial.println("LIMIT_OPEN:"+String(digitalRead(LIMIT_BRIDGE_OPEN)));

  if (distanciaTras > 2 && flagBarco && mode=='A') {
    distanciaBarco = "NAVIO PASSANDO  ";
    modeRed();
    flagPassando = true;
    tempoBarco = millis();
  }
}


//////////////////////////////////////////////////////////
void printDataLCD(LiquidCrystal_I2C lcd) {
  lcd.init();
  lcd.clear();
  lcd.setCursor(0, 0);
  if (flagBarco) {
    lcd.print("NAVIO DETECTADO");
    lcd.setCursor(0, 1);
    lcd.print(distanciaBarco);
    lcd.print("cm");
  } else {
    lcd.print("#   SEMAFORO   #");
    lcd.setCursor(0, 1);
    lcd.print(estadoSemaforo);
    Serial.println(estadoSemaforo);
  }

  lcd.setCursor(15, 1);
  lcd.print(mode);
}


void printManualLCD(LiquidCrystal_I2C lcd, String linha1, String linha2) {
  lcd.init();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(linha1);
  lcd.setCursor(0, 1);
  lcd.print(linha2);
}

bool isBridgeClose() {
  return (!digitalRead(LIMIT_BRIDGE_CLOSE));
}

bool isBridgeOpen() {
  return (!digitalRead(LIMIT_BRIDGE_OPEN));
}


void openBridge() {
  if (!isBridgeClose())
    digitalWrite(MOTOR_BRIDGE_CLOSE, HIGH);
  else {
    Serial.println("JÁ ESTÁ FECHADA!");
    stopBridge();
  }
}

void closeBridge() {
  if (!isBridgeOpen())
    digitalWrite(MOTOR_BRIDGE_OPEN, HIGH);
  else {
    Serial.println("JÁ ESTÁ ABERTA!");
    stopBridge();
  }
}

void readButton() {
  if (mode == 'M') {
    if (!digitalRead(BUTTON_OPEN_CLOSE)) {
      if (isBridgeClose()) {
        modeRed();
        closeBridge();
        turnOnAlarm();
        Serial.println("FECHANDO...");
        printManualLCD(lcd, "     PONTE      ", "   FECHANDO     ");
        while (!digitalRead(BUTTON_OPEN_CLOSE) && !isBridgeOpen())
          ;
        stopBridge();
        flagBarco = false;
        flagPassando = false;
        modeGreen();
        Serial.println("Solte o botão");
        printManualLCD(lcd, "     PONTE      ", "### FECHADA ###");
        while (!digitalRead(BUTTON_OPEN_CLOSE))
          ;
        turnOffAlarm();
        delay(2000);
      } else {
        modeRed();
        openBridge();
        turnOnAlarm();
        Serial.println("ABRINDO...");
        printManualLCD(lcd, "     PONTE      ", "    ABRINDO     ");
        while (!digitalRead(BUTTON_OPEN_CLOSE) && !isBridgeClose())
          ;
        stopBridge();
        Serial.println("Solte o botão");
        printManualLCD(lcd, "     PONTE      ", "### ABERTA ###");
        while (!digitalRead(BUTTON_OPEN_CLOSE))
          ;
        turnOffAlarm();
      }
    }
  }
}

void stopBridge() {
  digitalWrite(MOTOR_BRIDGE_OPEN, LOW);
  digitalWrite(MOTOR_BRIDGE_CLOSE, LOW);
}

void bip() {
  digitalWrite(BUZZER, HIGH);
  delay(250);
  digitalWrite(BUZZER, LOW);
}

void turnOnAlarm() {
  digitalWrite(BUZZER, HIGH);
}

void turnOffAlarm() {
  digitalWrite(BUZZER, LOW);
}


void verifyClosing() {
  if (flagFechando && !digitalRead(LIMIT_BRIDGE_CLOSE)) {
    flagFechando = false;
    stopBridge();
  }
}

void verifyMode() {
  mode = (!digitalRead(SWITCH_MODE)) ? 'A' : 'M';
  if (mode != modeAnt) {
    modeAnt = mode;
    flagAbrindo=false;
    flagBarco=false;
    flagPassando=false;
    flagFechando = false;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("###   MODO    ###");
    lcd.setCursor(0, 1);
    if (mode == 'A')
      lcd.print("  AUTOMATICO   ");
    else
      lcd.print("     MANUAL     ");
    bip();
    delay(2500);
  }
}


///////////////////////////////////////////////////////////
void modeAlert() {
  static bool aux = false;
  if (aux) {
    modeYellow();
    turnOnAlarm();
  } else {
    modeOff();
    turnOffAlarm();
  }
  aux = !aux;
}

///////////////////////////////////////////////////////////
void modeGreen() {
  digitalWrite(led_verde_1, HIGH);
  digitalWrite(led_amarelo_1, LOW);
  digitalWrite(led_vermelho_1, LOW);

  digitalWrite(led_verde_2, HIGH);
  digitalWrite(led_amarelo_2, LOW);
  digitalWrite(led_vermelho_2, LOW);
  estadoSemaforo = "#### VERDE ####";
  turnOffAlarm();
}

///////////////////////////////////////////////////////////
void modeYellow() {
  digitalWrite(led_verde_1, LOW);
  digitalWrite(led_amarelo_1, HIGH);
  digitalWrite(led_vermelho_1, LOW);

  digitalWrite(led_verde_2, LOW);
  digitalWrite(led_amarelo_2, HIGH);
  digitalWrite(led_vermelho_2, LOW);
  estadoSemaforo = "### AMARELO ###";
}

///////////////////////////////////////////////////////////
void modeRed() {
  digitalWrite(led_verde_1, LOW);
  digitalWrite(led_amarelo_1, LOW);
  digitalWrite(led_vermelho_1, HIGH);

  digitalWrite(led_verde_2, LOW);
  digitalWrite(led_amarelo_2, LOW);
  digitalWrite(led_vermelho_2, HIGH);
  estadoSemaforo = "## VERMELHO ##";
  turnOnAlarm();
}

///////////////////////////////////////////////////////////
void modeOff() {
  digitalWrite(led_verde_1, LOW);
  digitalWrite(led_amarelo_1, LOW);
  digitalWrite(led_vermelho_1, LOW);

  digitalWrite(led_verde_2, LOW);
  digitalWrite(led_amarelo_2, LOW);
  digitalWrite(led_vermelho_2, LOW);
}