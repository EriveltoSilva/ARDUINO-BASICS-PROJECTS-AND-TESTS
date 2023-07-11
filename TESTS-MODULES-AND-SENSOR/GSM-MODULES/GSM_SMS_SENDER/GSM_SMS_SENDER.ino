/**
 * AUTHOR: Erivelto Clénio da Costa e Silva (OTLEVIRE)
 * IMPLEMENTED DATE: 11/07/2023
 * Objective: This is a basic program to test every kind of GSM module like SIM800L, SIM900, SIM808, A6 MINI etc.
 * NOTE: For SIM800L, SIM900, and SIM808 default BAUD RATE is 9600. And for AG MINI default BAUD RATE is 115200. 
 * MICROCONTROLLER USED: Arduino Uno
*/


////////////////////////////// LIBRARIES ////////////////////////////////
#include <String.h>
#include <SoftwareSerial.h>
/////////////////////////////////////////////////////////////////////////

//////////////////////////////// DEFINITIONS ////////////////////////////
#define BOTAO 8
#define LED 13
#define RX 2
#define TX 3
#define GSM_BAUD_RATE 9600
#define SERIAL_BAUD_RATE 9600
/////////////////////////////////////////////////////////////////////////

////////////////////////////// GLOBAL VARIEBLES ////////////////////////
int num = 0;
long int tempoDelay = 0;
const String PHONE_NUMBER = "+244940811141";
////////////////////////////////////////////////////////////////////////

////////////////////////////// OBEJECTS ////////////////////////////////
SoftwareSerial gsm(RX, TX); // RX, TX
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BOTAO, INPUT_PULLUP);
  digitalWrite(LED, LOW);
  Serial.begin(SERIAL_BAUD_RATE);
  delay(1000);
  gsm.begin(GSM_BAUD_RATE);
  delay(1000);
  sendSMS(PHONE_NUMBER, "## OLA ANGOLA  ##");
}

////////////////////////////////////////////////////////////////////////
void loop() {
  comunication();
  if(millis()-tempoDelay>1000){
    tempoDelay = millis();
    digitalWrite(LED, !digitalRead(LED));
  }
  
  if (!digitalRead(BOTAO)) {
    while (!digitalRead(BOTAO));
    num++;
    String text = "## BUTTON PRESSED ";
    text += num;
    text += "º TIMES ##";
    sendSMS(PHONE_NUMBER, text);
  }
  delay(10);
}


/////////////////////////////////////////////////////////
void sendSMS(const String number, String sms)
{
  Serial.print("SMS: "); Serial.println(sms);
  gsm.println("AT+CMGF=1");
  delay(1000);  comunication();
  gsm.println("AT+CMGS=\"" + number + "\"\r");
  delay(1000);  comunication();
  gsm.println(sms);
  delay(100);
  gsm.println((char)26);
  delay(1000);  comunication();
  delay(4000);
  Serial.println("\n################ SMS SENT! #################");
}

/////////////////////////////////////////////////////////
void comunication()
{
  if (Serial.available())
    while (Serial.available())
      gsm.write(Serial.read());

  if (gsm.available())
    while (gsm.available())
      Serial.write(gsm.read());
}
