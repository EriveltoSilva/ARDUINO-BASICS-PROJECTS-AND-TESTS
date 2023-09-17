/**************************************************
     AUTHOR: ERIVELTO CLÉNIO DA COSTA E SILVA
     FOR: 
     CREATED AT: 10-09-2023.
     Objectivo: Controlar um carro robotizado de 4 rodas por meio do uma página web
   no computador ou celular, isso via WIFI.


  Principais Componentes usados:
 *** 1- ARDUINO MEGA 2560 (1)
 *** 2- LDR 5mm (6)
 *** 3- POTENCIOMETRO (6)
 *** 4- LCD 16X04 (2)
 *** 5- BOTÃO DE PRESSÃO (2)
 *** 6- RESISTOR 10K (2)
 *** 7- RESISTOR 100R (1)
 *** 8- RESISTOR 220R (1)
 *** 9-SERVO (6)
 *** 10-ULTRASSÓNICO (4)
 *************************************************/


/*

** 
  Arranjar a VAGA3 do parque1...
*/

////////////////////////// INCLUSION ///////////////////////////////
#include <Wire.h>
#include <Servo.h>
#include <Ultrasonic.h>
#include <LiquidCrystal_I2C.h>

#define LED 13
//----------------- PARQUE1 ------------------
#define P1_LDR1 A0
#define P1_LDR2 A1
#define P1_LDR3 A2
#define TRIGGER_RESERVATION1 4
#define ECHO_RESERVATION1 5

//----------------- PARQUE2 ------------------
#define P2_LDR1 A3
#define P2_LDR2 A4
#define P2_LDR3 A5
#define TRIGGER_RESERVATION2 6
#define ECHO_RESERVATION2 7

#define PIN_SERVO_ENTRANCE1 24
#define PIN_SERVO_EXIT1 23
#define PIN_SERVO_RESERVATION1 22

#define PIN_SERVO_ENTRANCE2 27
#define PIN_SERVO_EXIT2 26
#define PIN_SERVO_RESERVATION2 25

#define TRIGGER_EXIT1 8
#define ECHO_EXIT1 9

#define TRIGGER_EXIT2 10
#define ECHO_EXIT2 11

#define BUTTON_ENTRANCE1 2
#define BUTTON_ENTRANCE2 3

#define TIME_TO_CLOSE 5000

/////////////////////////////// Variables /////////////////////////////////////
bool p1Vaga1 = false;
bool p1Vaga2 = false;
bool p1Vaga3 = false;
bool p1Reservation = false;

bool p2Vaga1 = false;
bool p2Vaga2 = false;
bool p2Vaga3 = false;
bool p2Reservation = false;

bool flagEntrance1 = false;
bool flagEntrance2 = false;

bool flagExit1 = false;
bool flagExit2 = false;

bool flagAgend1 = false;
bool flagAgend2 = false;

String dataStored = "";

unsigned long int timeDelay = 0;
unsigned long int timerExit1 = 0;
unsigned long int timerExit2 = 0;
//////////////////////////////// Objects Instances /////////////////////////////
Servo servoEntrance1;
Servo servoExit1;
Servo servoReservation1;

Servo servoEntrance2;
Servo servoExit2;
Servo servoReservation2;
LiquidCrystal_I2C lcd2(0x27, 16, 4);  // set the LCD address to 0x26 for a 16X04
LiquidCrystal_I2C lcd1(0x26, 16, 4);  // set the LCD address to 0x27 for a 16x02
Ultrasonic ultrassonicReservation1(TRIGGER_RESERVATION1, ECHO_RESERVATION1);
Ultrasonic ultrassonicReservation2(TRIGGER_RESERVATION2, ECHO_RESERVATION2);
Ultrasonic ultrassonicExit1(TRIGGER_EXIT1, ECHO_EXIT1);
Ultrasonic ultrassonicExit2(TRIGGER_EXIT2, ECHO_EXIT2);

/////////////////////////////////////////////////////////////////////////////
void sendData();
bool hasCar(byte num);
bool hasCar(Ultrasonic, String);
void openServo(Servo, String);
void closeServo(Servo, String);
void closeAllServo();
String readSensors();
void sendData();
void receiveCommand();
void verifyTimeToCloseExit();
void printDataLCD(byte numLCD, LiquidCrystal_I2C lcd);


///////////////////////////////////////////////////////////
void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  pinMode(BUTTON_ENTRANCE1, INPUT_PULLUP);
  pinMode(BUTTON_ENTRANCE2, INPUT_PULLUP);

  servoEntrance1.attach(PIN_SERVO_ENTRANCE1);
  servoExit1.attach(PIN_SERVO_EXIT1);
  servoReservation1.attach(PIN_SERVO_RESERVATION1);
  delay(500);

  servoEntrance2.attach(PIN_SERVO_ENTRANCE2);
  servoExit2.attach(PIN_SERVO_EXIT2);
  servoReservation2.attach(PIN_SERVO_RESERVATION2);
  delay(500);

  lcd1.init();  // initialize the lcd
  lcd1.backlight();
  delay(50);

  lcd2.init();  // initialize the lcd
  lcd2.backlight();
  delay(50);

  lcd1.setCursor(0, 0);
  lcd1.print(" UNIV.METODISTA");
  lcd1.setCursor(0, 1);
  lcd1.print(" PROJECTO FINAL");
  lcd1.setCursor(-4, 2);
  lcd1.print(" ## ERIVELTO  ## ");

  lcd2.setCursor(0, 0);
  lcd2.print(" UNIV.METODISTA");
  lcd2.setCursor(0, 1);
  lcd2.print(" PROJECTO FINAL ");
  lcd2.setCursor(-4, 2);
  lcd2.print(" ## OSVALDO ## ");

  Serial.begin(9600);
  delay(500);
  Serial2.begin(9600);
  delay(500);

  closeAllServo();
  Serial.println(" ## SISTEMA LIGADO, TUDO PRONTO! ##");
}

void loop() {
  receiveCommand();
  verifyTimeToCloseExit();

  if (millis() - timeDelay > 1000) {
    timeDelay = millis();
    dataStored = readSensors();
    printDataLCD(1, lcd1);
    printDataLCD(2, lcd2);
    digitalWrite(LED, !digitalRead(LED));
  }

  if (!digitalRead(BUTTON_ENTRANCE1)) {
    while (!digitalRead(BUTTON_ENTRANCE1))
      ;
    flagEntrance1 = !flagEntrance1;
    if (flagEntrance1)
      closeServo(servoEntrance1, "ENTRANCE1");
    else
      openServo(servoEntrance1, "ENTRANCE1");
  }

  if (!digitalRead(BUTTON_ENTRANCE2)) {
    while (!digitalRead(BUTTON_ENTRANCE2))
      ;
    flagEntrance2 = !flagEntrance2;
    if (flagEntrance2)
      closeServo(servoEntrance2, "ENTRANCE2");
    else
      openServo(servoEntrance2, "ENTRANCE2");
  }

  delay(10);
}

bool hasCar(byte num) {
  return (num > 50);
}

bool hasCar(Ultrasonic ultrassonic, String text) {
  int distance = ultrassonic.read();
  Serial.println("###Vaga" + text + ":" + String(distance));
  if (text.equalsIgnoreCase("EXIT1") || text.equalsIgnoreCase("EXIT2"))
    return (distance > 0 && distance < 14);
  else if (text.equalsIgnoreCase("RESERVATION1") || text.equalsIgnoreCase("RESERVATION2"))
    return (distance > 0 && distance < 7);
}

void openExit(Servo servo, String text) {
  if (text.equalsIgnoreCase("EXIT1"))
  {
    timerExit1 = millis();
    flagExit1 = true;
  }
  else if (text.equalsIgnoreCase("EXIT2"))
  {  
    timerExit2 = millis();
    flagExit2 = true;
  }
  servo.write(90);
}

void verifyTimeToCloseExit()
{
  if ((millis() - timerExit1 > TIME_TO_CLOSE) && flagExit1) {
    flagExit1 = false;
    closeServo(servoExit1, "EXIT1");
  }
  if ((millis() - timerExit2 > TIME_TO_CLOSE) && flagExit2) {
    flagExit2 = false;
    closeServo(servoExit2, "EXIT2");
  }
}

void openServo(Servo servo, String text) {
  if (text.equalsIgnoreCase("RESERVATION1") || text.equalsIgnoreCase("ENTRANCE1") || text.equalsIgnoreCase("EXIT1") || text.equalsIgnoreCase("RESERVATION2") || text.equalsIgnoreCase("EXIT2"))
    servo.write(90);
  else if (text.equalsIgnoreCase("ENTRANCE2"))
    servo.write(145);
}

void closeServo(Servo servo, String text) {
  if (text.equalsIgnoreCase("RESERVATION1") || text.equalsIgnoreCase("EXIT1") || text.equalsIgnoreCase("EXIT2"))
    servo.write(180);
  else if (text.equalsIgnoreCase("ENTRANCE1") || text.equalsIgnoreCase("RESERVATION2"))
    servo.write(0);
  else if (text.equalsIgnoreCase("ENTRANCE2"))
    servo.write(45);
}

void closeAllServo() {
  closeServo(servoEntrance1, "ENTRANCE1");
  delay(10);
  closeServo(servoExit1, "EXIT1");
  delay(10);
  closeServo(servoReservation1, "RESERVATION1");
  delay(10);

  closeServo(servoEntrance2, "ENTRANCE2");
  delay(10);
  closeServo(servoExit2, "EXIT2");
  delay(10);
  closeServo(servoReservation2, "RESERVATION2");
  delay(10);
}

String readSensors() {
  int ldr1 = analogRead(P1_LDR1);
  int ldr2 = analogRead(P1_LDR2);
  int ldr3 = analogRead(P1_LDR3);

  int ldr4 = analogRead(P2_LDR1);
  int ldr5 = analogRead(P2_LDR2);
  int ldr6 = analogRead(P2_LDR3);


  p1Vaga1 = hasCar(map(ldr1, 0, 1023, 0, 100));
  p1Vaga2 = hasCar(map(ldr2, 0, 1023, 0, 100));
  p1Vaga3 = hasCar(map(ldr3, 0, 1023, 0, 100));
  p1Reservation = hasCar(ultrassonicReservation1, "RESERVATION1");

  p2Vaga1 = hasCar(map(ldr4, 0, 1023, 0, 100));
  p2Vaga2 = hasCar(map(ldr5, 0, 1023, 0, 100));
  p2Vaga3 = hasCar(map(ldr6, 0, 1023, 0, 100));
  p2Reservation = hasCar(ultrassonicReservation2, "RESERVATION2");

  if (hasCar(ultrassonicExit1, "EXIT1"))
    openExit(servoExit1, "EXIT1");

  if (hasCar(ultrassonicExit2, "EXIT2"))
    openExit(servoExit2, "EXIT2");

  Serial.println("---------------------- Dados P1 -----------------------");
  Serial.println("V1:" + String(ldr1) + "(" + p1Vaga1 + ")");
  Serial.println("V2:" + String(ldr2) + "(" + p1Vaga2 + ")");
  Serial.println("V3:" + String(ldr3) + "(" + p1Vaga3 + ")");
  Serial.println("R1:" + String(p1Reservation));

  // Serial.println("---------------------- Dados P2 -----------------------");
  Serial.println("V1:" + String(ldr4) + "(" + p2Vaga1 + ")");
  Serial.println("V2:" + String(ldr5) + "(" + p2Vaga2 + ")");
  Serial.println("V3:" + String(ldr6) + "(" + p2Vaga3 + ")");
  Serial.println("R2:" + String(p2Reservation));
  Serial.println("--------------------------------------------------------\n");
  String txt = ("D*" + String(p1Vaga1) + "*" + String(p1Vaga2) + "*" + String(p1Vaga3) + "*" + String(p1Reservation) + "*" + String(p2Vaga1) + "*" + String(p2Vaga2) + "*" + String(p2Vaga3) + "*" + String(p2Reservation) + "*");
  return txt;
}

void sendData(String txt) {
  Serial2.println(txt);
}

void receiveCommand() {
  if (Serial.available()) {
    String text = "";
    while (Serial.available()) {
      char rx = Serial.read();
      text += rx;
      switch (rx) {
        case 'A':  // abrir servo reserva p1
          openServo(servoReservation1, "RESERVATION1");
          break;
        case 'a':  // fechar servo reserva p1
          closeServo(servoReservation1, "RESERVATION1");
          break;
        case 'B':  // abrir servo entrada p1
          openServo(servoEntrance1, "ENTRANCE1");
          break;
        case 'b':  // fechar servo entrada p1
          closeServo(servoEntrance1, "ENTRANCE1");
          break;
        case 'C':  // abrir servo p1
          openServo(servoExit1, "EXIT1");
          break;
        case 'c':  // abrir servo p1
          closeServo(servoExit1, "EXIT1");
          break;

        case 'D':  // abrir servo reserva p2
          openServo(servoReservation2, "RESERVATION2");
          break;
        case 'd':  // fechar servo reserva p2
          closeServo(servoReservation2, "RESERVATION2");
          break;
        case 'E':  // abrir servo entrada p2
          openServo(servoEntrance2, "ENTRANCE2");
          break;
        case 'e':  // fechar servo entrada p2
          closeServo(servoEntrance2, "ENTRANCE2");
          break;
        case 'F':  // abrir servo p2
          openServo(servoExit2, "EXIT2");
          break;
        case 'f':  // abrir servo p2
          closeServo(servoExit2, "EXIT2");
          break;
        case 'R': sendData(readSensors()); break;
        case 'y': flagAgend1 = false; break;  //
        case 'Y': flagAgend1 = true; break;   //
        case 'z': flagAgend2 = false; break;  //
        case 'Z': flagAgend2 = true; break;   //
      }
    }
  }


  if (Serial2.available()>0) {
    String text = "";
    while (Serial2.available()>0) {
      char rx = Serial2.read();
      text += rx;
      switch (rx) {
        case 'A':  // abrir servo reserva p1
          openServo(servoReservation1, "RESERVATION1");
          break;
        case 'a':  // fechar servo reserva p1
          closeServo(servoReservation1, "RESERVATION1");
          break;
        case 'B':  // abrir servo entrada p1
          openServo(servoEntrance1, "ENTRANCE1");
          break;
        case 'b':  // fechar servo entrada p1
          closeServo(servoEntrance1, "ENTRANCE1");
          break;
        case 'C':  // abrir servo p1
          openServo(servoExit1, "EXIT1");
          break;
        case 'c':  // abrir servo p1
          closeServo(servoExit1, "EXIT1");
          break;

        case 'D':  // abrir servo reserva p2
          openServo(servoReservation2, "RESERVATION2");
          break;
        case 'd':  // fechar servo reserva p2
          closeServo(servoReservation2, "RESERVATION2");
          break;
        case 'E':  // abrir servo entrada p2
          openServo(servoEntrance2, "ENTRANCE2");
          break;
        case 'e':  // fechar servo entrada p2
          closeServo(servoEntrance2, "ENTRANCE2");
          break;
        case 'F':  // abrir servo p2
          openServo(servoExit2, "EXIT2");
          break;
        case 'f':  // abrir servo p2
          closeServo(servoExit2, "EXIT2");
          break;
        case 'R': sendData(readSensors()); break;
        case 'y': flagAgend1 = false; break;  //
        case 'Y': flagAgend1 = true; break;   //
        case 'z': flagAgend2 = false; break;  //
        case 'Z': flagAgend2 = true; break;   //
      }
    }
  }
}

//////////////////////////////////////////////////////////
void printDataLCD(byte numLCD, LiquidCrystal_I2C lcd) {
  lcd.init();
  lcd.clear();
  lcd.setCursor(0, 0);
  if (numLCD == 1) {
    lcd.print("VAGA1:");
    lcd.print((p1Vaga1) ? "OCUPADO" : "LIVRE");
    lcd.setCursor(0, 1);
    lcd.print("VAGA2:");
    lcd.print((p1Vaga2) ? "OCUPADO" : "LIVRE");
    lcd.setCursor(-4, 2);
    lcd.print("VAGA3:");
    lcd.print((p1Vaga3) ? "OCUPADO" : "LIVRE");
    lcd.setCursor(-4, 3);
    lcd.print("ESPECIAL:");
    if (flagAgend1)
      lcd.print("AGENDADO");
    else
      lcd.print((p1Reservation) ? "OCUPADO" : "LIVRE");
  } else {
    lcd.print("VAGA1:");
    lcd.print((p2Vaga1) ? "OCUPADO" : "LIVRE");
    lcd.setCursor(0, 1);
    lcd.print("VAGA2:");
    lcd.print((p2Vaga2) ? "OCUPADO" : "LIVRE");
    lcd.setCursor(-4, 2);
    lcd.print("VAGA3:");
    lcd.print((p2Vaga3) ? "OCUPADO" : "LIVRE");
    lcd.setCursor(-4, 3);
    lcd.print("ESPECIAL:");
    if (flagAgend2)
      lcd.print("AGENDADO");
    else
      lcd.print((p2Reservation) ? "OCUPADO" : "LIVRE");
  }
}