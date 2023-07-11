/**
   AUTHOR: Erivelto Clénio da Costa e Silva (OTLEVIRE)
   IMPLEMENTED DATE: 11/07/2023
   OBJECTIVE: This is a basic program to test every kind of GSM module like SIM800L, SIM900, SIM808, A6 MINI etc.
   NOTE: For SIM800L, SIM900, and SIM808 default BAUD RATE is 9600. And for AG MINI default BAUD RATE is 115200.
   MICROCONTROLLER USED: Arduino Uno
*/


////////////////////////////// LIBRARIES ////////////////////////////////
#include <String.h>
#include <SoftwareSerial.h>
/////////////////////////////////////////////////////////////////////////

//////////////////////////////// DEFINITIONS ////////////////////////////
#define LED 13
#define RX 2
#define TX 3
#define GSM_BAUD_RATE 9600
#define SERIAL_BAUD_RATE 9600
/////////////////////////////////////////////////////////////////////////

////////////////////////////// OBEJECTS ////////////////////////////////
SoftwareSerial gsm(RX, TX); // RX, TX
////////////////////////////////////////////////////////////////////////

/////////////////////// FUNCTION PROTOTYPES ////////////////////////////
void configGSM();
void comunication();
String recieveSMS();
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.begin(SERIAL_BAUD_RATE);
  delay(1000);
  gsm.begin(GSM_BAUD_RATE);
  delay(1000);
  configGSM();
  Serial.println(" ## SYSTEM INITIALIZED ##");
}

////////////////////////////////////////////////////////////////////////
void loop() {
  comunication();
  String response = recieveSMS();
  if (response.indexOf("+CMT:") > 0)
  {
    if (response.indexOf("LED ON") > 0)
    {
      digitalWrite(LED, HIGH);
      Serial.println("LED TURN ON");
    }
    else if (response.indexOf("LED OFF") > 0)
    {
      digitalWrite(LED, LOW);
      Serial.println("LED TURN OFF");
    }
  }
  delay(10);
}

void configGSM()
{
  gsm.print("AT+CMGF=1\r");                         //Set gsm in text mode
  delay(1000);                                      //Wait 1 second
  gsm.print("AT+CNMI=2,2,0,0,0\r");                 //Put gsm waiting for a new message get received
  delay(1000);                                      //Wait 1 second
}

///////////////////////////////////////
String recieveSMS()
{
  String answer = "";
  if (gsm.available() > 0)
  {
    while (gsm.available() > 0) {
      delay(5);
      char sms = gsm.read();
      answer += sms;
    }
    Serial.print(answer);
  }
  return answer;
}

/////////////////////////////////////////////////////////
void comunication()
{
  if (Serial.available())
    while (Serial.available())
      gsm.write(Serial.read());
}
