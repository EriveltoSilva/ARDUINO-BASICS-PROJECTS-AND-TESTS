#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define AIR A0
#define HEAD 2
#define MQ135 3
#define MOTOR 4
#define INTERRUPTOR 12
#define LED 13
#define TIME_TO_TURN_OFF 10000

bool flagStart = false, flagStarCar = false, flagTimer=false;
String motivo="";
unsigned long int timeDelay = 0, temporizador=0;

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void startCar();

void setup() {
  pinMode(HEAD, INPUT);
  pinMode(MQ135, INPUT);
  pinMode(INTERRUPTOR, INPUT_PULLUP);

  pinMode(MOTOR, OUTPUT);
  pinMode(LED, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("# UNIV. IMETRO  #");
  lcd.setCursor(0, 1);
  lcd.print(" PROJECTO FINAL ");
  delay(2500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("-----AUTORA-----");
  lcd.setCursor(0, 1);
  lcd.print("  SUZANA RANCA  ");
  Serial.begin(9600);
  delay(2500);
  Serial.println("## SISTEMA PRONTO ##");
  startTest();
}

void loop() {
  if(!hasCapacete() && digitalRead(MOTOR) && !flagTimer)
  {
    flagTimer=true;
    temporizador=millis(); 
    printLCD("### CAPACETE ###", "   RETIRADO   ");
  }
  else if(!hasCapacete() && digitalRead(MOTOR) && flagTimer)
  {
    printLCD( "MOTO DESLIGANDO...","EM:"+String((TIME_TO_TURN_OFF-millis()-temporizador)/1000)+"s");
    if(millis()-temporizador>TIME_TO_TURN_OFF)
    {
      printLCD("### CAPACETE ###", "   RETIRADO   ");
      delay(3000);
      printLCD("### DESLIGANDO ###", "     A MOTO     ");
      delay(3000);
      flagStarCar = flagTimer =false;
      motivo="CAPACETE RETIRADO";
      digitalWrite(MOTOR, LOW);
      printLCD("### DESLIGANDO ###", "     A MOTO     ");
    }
  }
  else if(hasCapacete() && digitalRead(MOTOR) && flagTimer ){
    printLCD("### CAPACETE ###", "   RECOLOCADO   ");
    delay(3000);
  }

  if (flagStarCar && !digitalRead(INTERRUPTOR))
  {
    printLCD("### ATENCAO ####", "   IGNICAO ON    ");
    digitalWrite(MOTOR, HIGH);
    }
  else if (flagStarCar && digitalRead(INTERRUPTOR))
  {
    digitalWrite(MOTOR, LOW);
    printLCD("### ATENCAO ####", "   IGNICAO OFF   ");
  }
  else if (!flagStarCar && !digitalRead(INTERRUPTOR)) {
    Serial.println("NÃO TEM PERMISSÃO DE LIGAR A MOTO NESSAS CONDIÇÕES");
    printLCD("### ATENCAO ####", motivo);
  }

  digitalWrite(LED, !digitalRead(LED));
  delay(100);
}



///////////////////////////////////////////////////////////////////
bool hasCapacete()
{
  return (digitalRead(HEAD));
}


bool testCapacete()
{
  bool flag = true;
  while (flag)
  {
    lcd.init();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   CAPACETE   ");
    lcd.setCursor(0, 1);
    if (hasCapacete())
    {
      lcd.print("   DETECTADO   ");
      //flag=true;
      delay(5000);
      return true;
    }
    else
      lcd.print(" NAO DETECTADO  ");
    delay(1000);
  }
}

bool testAlcool()
{
  while (true)
  {
    lcd.init();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SOPRE NO SENSOR");
    lcd.setCursor(0, 1);
    bool mq135 = digitalRead(MQ135);
    int motorSopro = analogRead(AIR);

    Serial.println("ALCOOL" + String(mq135));
    Serial.println("SOPRO:" + String(motorSopro));

    if (!digitalRead(MQ135)) //TEM ALCOOL
    {
      Serial.println("DETECTADO ALCOOL");
      lcd.print("  HA ALCOOL  ");
      delay(3000);
      return true;
    }
    else if (mq135 && motorSopro > 10) // NÃO TEM ALCOOL E SOPROU
    {
      Serial.println("Não foi detectado alcool:" + motorSopro);
      lcd.print(" NAO HA ALCOOL ");
      delay(3000);
      return false;
    }
    else {
      Serial.println("NÃO SOPRADO");
      lcd.print("### SEM SOPRO ###");
    }
    delay(200);
  }
}

void startTest()
{
  bool capacete = testCapacete();
  bool alcool = testAlcool();
  if (capacete && !alcool)
  {
    flagStarCar = true;
    Serial.println("NÃO FOI DETECTADO ALCOOL, PODE LIGAR");
    printLCD("   SEM ALCOOL   ", "   PODE LIGAR   ");
  }
  else if (alcool)
  {
    flagStarCar = false;
    motivo="ALCOOL DETECTADO";
    Serial.println("ALCOOL DETECTADO, NÃO PODE LIGAR");
    printLCD("   TEM ALCOOL   ", " NAO PODE LIGAR   ");
  }
}


//////////////////////////////////////////////////////////
void startCar()
{
  digitalWrite(MOTOR, LOW);
  delay(200);
  digitalWrite(MOTOR, HIGH);
  delay(2000);
  digitalWrite(MOTOR, LOW);
}

void printLCD(String linha1, String linha2)
{
  lcd.init();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(linha1);
  lcd.setCursor(0, 1);
  lcd.print(linha2);
}
