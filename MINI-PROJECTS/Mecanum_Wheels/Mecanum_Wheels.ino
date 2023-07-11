/**
   AUTHOR: Erivelto Clénio da Costa e Silva (OTLEVIRE)
   IMPLEMENTED DATE: 30/05/2023
   OBJECTIVE: This is a basic program to an robotic car  using mecanum wheels.
   MICROCONTROLLER USED: Arduino Nano
   MODULES USED: SHIELD L293 for motor control
*/

////////////////////////////// LIBRARIES ////////////////////////////////
#include <SoftwareSerial.h>
/////////////////////////////////////////////////////////////////////////

//////////////////////////////// DEFINITIONS ////////////////////////////
#define RX 2
#define TX 13

#define pinSH_CP  4   //Pino Clock  DIR_CLK
#define pinST_CP  12  //Pino Latch  DIR_LATCH
#define pinDS     8   //Pino Data   DIR_SER
#define pinEnable 7   //Pino Enable DIR_EN

#define pinMotor1PWM 11
#define pinMotor2PWM 3
#define pinMotor3PWM 5
#define pinMotor4PWM 6

#define qtdeCI   1

#define M_Frente_D_B 2
#define M_Frente_D_A 3
#define M_Frente_E_B 1
#define M_Frente_E_A 4
#define M_Tras_E_A 5
#define M_Tras_E_B 7
#define M_Tras_D_A 0
#define M_Tras_D_B 6
/////////////////////////////////////////////////////////////////////////

////////////////////////////// OBEJECTS ////////////////////////////////
SoftwareSerial bluetooth(RX, TX); // RX, TX
////////////////////////////////////////////////////////////////////////

/////////////////////// FUNCTION PROTOTYPES ////////////////////////////
void tras();
void parar();
void frente();
void direita();
void esquerda();
void rotacaoEsquerda();
void rotacaoDireita();
void diagonalDireitaTras();
void diagonalDireitaFrente();
void diagonalEsquerdaTras();
void diagonalEsquerdaFrente();
void receberDados();
void delayPWM(unsigned long tempo);
void ci74HC595Write(byte pino, bool estado);
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
void setup() {
   pinMode(pinSH_CP, OUTPUT);
   pinMode(pinST_CP, OUTPUT);
   pinMode(pinEnable, OUTPUT);
   pinMode(pinDS, OUTPUT);

   pinMode(pinMotor1PWM, OUTPUT);
   pinMode(pinMotor2PWM, OUTPUT);
   pinMode(pinMotor3PWM, OUTPUT);
   pinMode(pinMotor4PWM, OUTPUT);

   digitalWrite(pinEnable, LOW);
  delay(100);
  Serial.begin(9600);
  delay(1000);
  bluetooth.begin(9600);
  delay(1000);
  Serial.println(" ## SISTEMA LIGADO, TUDO PRONTO! ##");
}

////////////////////////////////////////////////////////////////////////
void loop() {
  // put your main code here, to run repeatedly:
  receberDados();
  delay(20);
}


////////////////////////////////////////////////////////////////////////
void receberDados()
{
  String numero="";
  if(bluetooth.available())
  {
    while(bluetooth.available())
    {
        char rx = bluetooth.read();
        Serial.println("RECEBIDO:"+rx);
        switch(rx){
          case 'A': diagonalEsquerdaFrente();break;
          case 'B': diagonalDireitaFrente();;break;
          case 'C': diagonalDireitaTras();break;
          case 'D': direita();break;
          case 'E': esquerda();break;
          case 'F': frente();break;
          case 'T': tras();break;
          case 'P': parar();break;
          case 'G': diagonalEsquerdaTras();break; 
        }
    }
  }

  if(Serial.available())
  {
    while(Serial.available())
    {
        char rx = Serial.read();
        Serial.println("RECEBIDO:"+rx);
        switch(rx){
          case 'A': diagonalEsquerdaFrente();break;
          case 'B': diagonalDireitaFrente();;break;
          case 'C': diagonalDireitaTras();break;
          case 'D': direita();break;
          case 'E': esquerda();break;
          case 'F': frente();break;
          case 'T': tras();break;
          case 'P': parar();break;
          case 'G': diagonalEsquerdaTras();break; 
        }
        delayPWM(1000);
    }
  }
}

////////////////////////////////////////////////////////////////////////
void diagonalEsquerdaFrente()
{
 ci74HC595Write(M_Frente_E_A, LOW);
  ci74HC595Write(M_Frente_E_B, LOW);
  
  ci74HC595Write(M_Frente_D_A, HIGH);
  ci74HC595Write(M_Frente_D_B, LOW);
  
  ci74HC595Write(M_Tras_E_A, HIGH);
  ci74HC595Write(M_Tras_E_B, LOW);
  
  ci74HC595Write(M_Tras_D_A, LOW);
  ci74HC595Write(M_Tras_D_B, LOW);
}

/////////////////////////////////////////////////////////////////////
void diagonalDireitaFrente()
{
  ci74HC595Write(M_Frente_E_A, HIGH);
  ci74HC595Write(M_Frente_E_B, LOW);
  
  ci74HC595Write(M_Frente_D_A, LOW);
  ci74HC595Write(M_Frente_D_B, LOW);
  
  ci74HC595Write(M_Tras_E_A, LOW);
  ci74HC595Write(M_Tras_E_B, LOW);
  
  ci74HC595Write(M_Tras_D_A, HIGH);
  ci74HC595Write(M_Tras_D_B, LOW);
}

////////////////////////////////////////////////////////////////////////
void diagonalDireitaTras()
{
  ci74HC595Write(M_Frente_E_A, LOW);
  ci74HC595Write(M_Frente_E_B, LOW);
  
  ci74HC595Write(M_Frente_D_A, LOW);
  ci74HC595Write(M_Frente_D_B, HIGH);
  
  ci74HC595Write(M_Tras_E_A, LOW);
  ci74HC595Write(M_Tras_E_B, HIGH);
  
  ci74HC595Write(M_Tras_D_A, LOW);
  ci74HC595Write(M_Tras_D_B, LOW);
}

////////////////////////////////////////////////////////////////////////
void direita()
{
  ci74HC595Write(M_Frente_E_A, HIGH);
  ci74HC595Write(M_Frente_E_B, LOW);
  
  ci74HC595Write(M_Frente_D_A, LOW);
  ci74HC595Write(M_Frente_D_B, HIGH);
  
  ci74HC595Write(M_Tras_E_A, LOW);
  ci74HC595Write(M_Tras_E_B, HIGH);
  
  ci74HC595Write(M_Tras_D_A, HIGH);
  ci74HC595Write(M_Tras_D_B, LOW);
  delayPWM(2000);
}

////////////////////////////////////////////////////////////////////////
void esquerda()
{
  ci74HC595Write(M_Frente_E_A, LOW);
  ci74HC595Write(M_Frente_E_B, HIGH);
  
  ci74HC595Write(M_Frente_D_A, HIGH);
  ci74HC595Write(M_Frente_D_B, LOW);
  
  ci74HC595Write(M_Tras_E_A, HIGH);
  ci74HC595Write(M_Tras_E_B, LOW);
  
  ci74HC595Write(M_Tras_D_A, LOW);
  ci74HC595Write(M_Tras_D_B, HIGH);
}

////////////////////////////////////////////////////////////////////////
void frente()
{
  ci74HC595Write(M_Frente_E_A, HIGH);
  ci74HC595Write(M_Frente_E_B, LOW);
  
  ci74HC595Write(M_Frente_D_A, HIGH);
  ci74HC595Write(M_Frente_D_B, LOW);
  
  ci74HC595Write(M_Tras_E_A, HIGH);
  ci74HC595Write(M_Tras_E_B, LOW);
  
  ci74HC595Write(M_Tras_D_A, HIGH);
  ci74HC595Write(M_Tras_D_B, LOW);
  //delayPWM(2000);
}


////////////////////////////////////////////////////////////////////////
void tras()
{
  ci74HC595Write(M_Frente_E_A, LOW);
  ci74HC595Write(M_Frente_E_B, HIGH);
  
  ci74HC595Write(M_Frente_D_A, LOW);
  ci74HC595Write(M_Frente_D_B, HIGH);
  
  ci74HC595Write(M_Tras_E_A, LOW);
  ci74HC595Write(M_Tras_E_B, HIGH);
  
  ci74HC595Write(M_Tras_D_A, LOW);
  ci74HC595Write(M_Tras_D_B, HIGH);
  //delayPWM(2000);
}

////////////////////////////////////////////////////////////////////////
void parar()
{
  ci74HC595Write(M_Frente_E_A, LOW);
  ci74HC595Write(M_Frente_E_B, LOW);
  
  ci74HC595Write(M_Frente_D_A, LOW);
  ci74HC595Write(M_Frente_D_B, LOW);
  
  ci74HC595Write(M_Tras_E_A, LOW);
  ci74HC595Write(M_Tras_E_B, LOW);
  
  ci74HC595Write(M_Tras_D_A, LOW);
  ci74HC595Write(M_Tras_D_B, LOW);
}

////////////////////////////////////////////////////////////////////////
void diagonalEsquerdaTras()
{
  ci74HC595Write(M_Frente_E_A, LOW);
  ci74HC595Write(M_Frente_E_B, HIGH);
  
  ci74HC595Write(M_Frente_D_A, LOW);
  ci74HC595Write(M_Frente_D_B, LOW);
  
  ci74HC595Write(M_Tras_E_A, LOW);
  ci74HC595Write(M_Tras_E_B, LOW);
  
  ci74HC595Write(M_Tras_D_A, LOW);
  ci74HC595Write(M_Tras_D_B, HIGH);
}

////////////////////////////////////////////////////////////////////////
void rotacaoDireita()
{
  ci74HC595Write(M_Frente_E_A, HIGH);
  ci74HC595Write(M_Frente_E_B, LOW);
  
  ci74HC595Write(M_Frente_D_A, LOW);
  ci74HC595Write(M_Frente_D_B, HIGH);
  
  ci74HC595Write(M_Tras_E_A, HIGH);
  ci74HC595Write(M_Tras_E_B, LOW);
  
  ci74HC595Write(M_Tras_D_A, LOW);
  ci74HC595Write(M_Tras_D_B, HIGH);  
  
}

////////////////////////////////////////////////////////////////////////
void rotacaoEsquerda()
{
  ci74HC595Write(M_Frente_E_A, LOW);
  ci74HC595Write(M_Frente_E_B, HIGH);
  
  ci74HC595Write(M_Frente_D_A, HIGH);
  ci74HC595Write(M_Frente_D_B, LOW);
  
  ci74HC595Write(M_Tras_E_A, LOW);
  ci74HC595Write(M_Tras_E_B, HIGH);
  
  ci74HC595Write(M_Tras_D_A, HIGH);
  ci74HC595Write(M_Tras_D_B, LOW);  
  
}


////////////////////////////////////////////////////////////////////////
void ci74HC595Write(byte pino, bool estado) {
  static byte ciBuffer[qtdeCI];
  bitWrite(ciBuffer[pino / 8], pino % 8, estado);
  digitalWrite(pinST_CP, LOW); //Inicia a Transmissão 
  digitalWrite(pinDS, LOW);    //Apaga Tudo para Preparar Transmissão
  digitalWrite(pinSH_CP, LOW);

  for (int nC = qtdeCI-1; nC >= 0; nC--) {
      for (int nB = 7; nB >= 0; nB--) {
  
          digitalWrite(pinSH_CP, LOW);  //Baixa o Clock      
          
          digitalWrite(pinDS,  bitRead(ciBuffer[nC], nB) );     //Escreve o BIT
          
          digitalWrite(pinSH_CP, HIGH); //Eleva o Clock
          digitalWrite(pinDS, LOW);     //Baixa o Data para Previnir Vazamento      
      }  
  }
  digitalWrite(pinST_CP, HIGH);  //Finaliza a Transmissão
}

////////////////////////////////////////////////////////////////////////
void delayPWM(unsigned long tempo) {
unsigned long inicio = millis();
byte valA0;  
  
  while ( (millis() - inicio) < tempo ) {
     valA0 = 90;
     analogWrite(pinMotor1PWM, valA0);
     analogWrite(pinMotor2PWM, valA0);
     analogWrite(pinMotor3PWM, valA0);
     analogWrite(pinMotor4PWM, valA0);
  }
}
