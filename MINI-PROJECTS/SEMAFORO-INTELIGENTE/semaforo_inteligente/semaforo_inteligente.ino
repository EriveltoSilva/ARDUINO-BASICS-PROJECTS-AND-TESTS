//*******************************
//Variable de Estados
#define estado_verde 1
#define estado_amarelo 2
#define estado_vermelho  3

//Funciones
#define led_verde_1 2
#define led_amarelo_1 3
#define led_vermelho_1  4

#define led_verde_2 5
#define led_amarelo_2 6
#define led_vermelho_2  7

#define led_verde_3 8
#define led_amarelo_3 9
#define led_vermelho_3  10

#define led_verde_4 11
#define led_amarelo_4 12
#define led_vermelho_4  13

#define led_verde_5 22
#define led_amarelo_5 23
#define led_vermelho_5  24

#define led_verde_6 25
#define led_amarelo_6 26
#define led_vermelho_6  27

#define led_verde_7 28
#define led_amarelo_7 29
#define led_vermelho_7 30

#define ldr1 A13
#define ldr2 A8

#define ldr3 A7
#define ldr4 A5
#define ldr5 A4
#define ldr6 A1 

#define ldr7 A0
#define ldr8 A3
#define ldr9 A6
#define ldr10 A2 //TEM UM PROBLEMA COM O LDR (TALVEZ UM CURTO CIRCUITO ENTRE AS PERNAS)


#define ldr11 A9 
#define ldr12 A10 
#define ldr13 A11
#define ldr14 A12

#define pot A15
#define LED 31

//Variable
bool Evento_TA1;
bool Evento_TR1;
bool Evento_TV1;

bool Evento_TA2;
bool Evento_TR2;
bool Evento_TV2;

bool Evento_TA3;
bool Evento_TR3;
bool Evento_TV3;

bool Evento_TA4;
bool Evento_TR4;
bool Evento_TV4;

bool Evento_TA5;
bool Evento_TR5;
bool Evento_TV5;

bool Evento_TA6;
bool Evento_TR6;
bool Evento_TV6;

bool Evento_TA7;
bool Evento_TR7;
bool Evento_TV7;

bool Habilitado_TV1;
bool Habilitado_TA1;
bool Habilitado_TR1;

bool Habilitado_TV2;
bool Habilitado_TA2;
bool Habilitado_TR2;

bool Habilitado_TV3;
bool Habilitado_TA3;
bool Habilitado_TR3;

bool Habilitado_TV4;
bool Habilitado_TA4;
bool Habilitado_TR4;

bool Habilitado_TV5;
bool Habilitado_TA5;
bool Habilitado_TR5;

bool Habilitado_TV6;
bool Habilitado_TA6;
bool Habilitado_TR6;

bool Habilitado_TV7;
bool Habilitado_TA7;
bool Habilitado_TR7;

byte estado_semaforo_1;
byte estado_semaforo_2;
byte estado_semaforo_3;
byte estado_semaforo_4;
byte estado_semaforo_5;
byte estado_semaforo_6;
byte estado_semaforo_7;

boolean engarrafado1_2=false,engarrafado3=false,engarrafado4_5=false, engarrafado6=false;
boolean engarrafado7_8=false, engarrafado9=false, engarrafado10_11=false;
boolean engarrafado12_13=false, engarrafado14=false;
boolean haCarro1=false, haCarro4=false, haCarro7=false, haCarro11=false, haCarro13=false;
//**************************************************************************************


//Recursos
int contador1=0, contador2=0, contador3=0, contador4=0, contador5=0, contador6=0, contador7=0, contador8=0; 
int contador9=0, contador10=0, contador11=0, contador12=0, contador13=0, contador14=0;
int contador3_01=0, contador3_02=0, contador6_01=0, contador6_02=0, contador9_01=0, contador9_02=0; 
int contador14_01=0, contador14_02=0;
int contadorHa1=0, contadorHa2=0, contadorHa4=0, contadorHa5=0, contadorHa7=0, contadorHa8=0;
int contadorHa10=0, contadorHa11=0, contadorHa12=0, contadorHa13=0;
long tempo_ant=0,tempo_act=0, tempo_req=1000;
boolean controleAlerta=false, controleVez=false;
int nSegundos=5;
int marcoLDR = 600, maximo=0;
byte contadorEngarrafamento=0, contadorEngarrafamentoAuxiliar=0;
//**********************************


void modo_verde_1(){
  digitalWrite(led_verde_1,HIGH);
  digitalWrite(led_amarelo_1,LOW);
  digitalWrite(led_vermelho_1,LOW);
  }
  

void modo_amarelo_1(){
  digitalWrite(led_verde_1,LOW);
  digitalWrite(led_amarelo_1,HIGH);
  digitalWrite(led_vermelho_1,LOW);
  }

 void modo_vermelho_1(){
  digitalWrite(led_verde_1,LOW);
  digitalWrite(led_amarelo_1,LOW);
  digitalWrite(led_vermelho_1,HIGH);
  }

 void modo_verde_2(){
  digitalWrite(led_verde_2,HIGH);
  digitalWrite(led_amarelo_2,LOW);
  digitalWrite(led_vermelho_2,LOW);
  }

  void modo_amarelo_2(){
  digitalWrite(led_verde_2,LOW);
  digitalWrite(led_amarelo_2,HIGH);
  digitalWrite(led_vermelho_2,LOW);
  }

 void modo_vermelho_2(){
  digitalWrite(led_verde_2,LOW);
  digitalWrite(led_amarelo_2,LOW);
  digitalWrite(led_vermelho_2,HIGH);
  }


 void modo_verde_3(){
  digitalWrite(led_verde_3,HIGH);
  digitalWrite(led_amarelo_3,LOW);
  digitalWrite(led_vermelho_3,LOW);
  }

  void modo_amarelo_3(){
  digitalWrite(led_verde_3,LOW);
  digitalWrite(led_amarelo_3,HIGH);
  digitalWrite(led_vermelho_3,LOW);
  }

 void modo_vermelho_3(){
  digitalWrite(led_verde_3,LOW);
  digitalWrite(led_amarelo_3,LOW);
  digitalWrite(led_vermelho_3,HIGH);
  }


  void modo_verde_4(){
  digitalWrite(led_verde_4,HIGH);
  digitalWrite(led_amarelo_4,LOW);
  digitalWrite(led_vermelho_4,LOW);
  
  }
  

void modo_amarelo_4(){
  digitalWrite(led_verde_4,LOW);
  digitalWrite(led_amarelo_4,HIGH);
  digitalWrite(led_vermelho_4,LOW);
  }

 void modo_vermelho_4(){
  digitalWrite(led_verde_4,LOW);
  digitalWrite(led_amarelo_4,LOW);
  digitalWrite(led_vermelho_4,HIGH);
 }

  void modo_verde_5(){
  digitalWrite(led_verde_5,HIGH);
  digitalWrite(led_amarelo_5,LOW);
  digitalWrite(led_vermelho_5,LOW);
  }

  void modo_amarelo_5(){
  digitalWrite(led_verde_5,LOW);
  digitalWrite(led_amarelo_5,HIGH);
  digitalWrite(led_vermelho_5,LOW);
  }

 void modo_vermelho_5(){
  digitalWrite(led_verde_5,LOW);
  digitalWrite(led_amarelo_5,LOW);
  digitalWrite(led_vermelho_5,HIGH);
  }

void modo_verde_6(){
  digitalWrite(led_verde_6,HIGH);
  digitalWrite(led_amarelo_6,LOW);
  digitalWrite(led_vermelho_6,LOW);
}

void modo_amarelo_6(){
  digitalWrite(led_verde_6,LOW);
  digitalWrite(led_amarelo_6,HIGH);
  digitalWrite(led_vermelho_6,LOW);
}

void modo_vermelho_6(){
  digitalWrite(led_verde_6,LOW);
  digitalWrite(led_amarelo_6,LOW);
  digitalWrite(led_vermelho_6,HIGH);
}

void modo_verde_7(){
  digitalWrite(led_verde_7,HIGH);
  digitalWrite(led_amarelo_7,LOW);
  digitalWrite(led_vermelho_7,LOW);
}

void modo_amarelo_7(){
  digitalWrite(led_verde_7,LOW);
  digitalWrite(led_amarelo_7,HIGH);
  digitalWrite(led_vermelho_7,LOW);
}

void modo_vermelho_7(){
  digitalWrite(led_verde_7,LOW);
  digitalWrite(led_amarelo_7,LOW);
  digitalWrite(led_vermelho_7,HIGH);
}

void enviarDados(){
  if(digitalRead(led_verde_1)){
    Serial.print(1); 
  }
  else if(digitalRead(led_amarelo_1))
  {
    Serial.print(2); 
  }
  else if(digitalRead(led_vermelho_1))
  {
    Serial.print(3); 
  }else{
    Serial.print(4); 
  }
  Serial.print('*');

  if(digitalRead(led_verde_2)){
    Serial.print(1); 
  }
  else if(digitalRead(led_amarelo_2))
  {
    Serial.print(2); 
  }
  else if(digitalRead(led_vermelho_2))
  {
    Serial.print(3); 
  }
  else{
    Serial.print(4); 
  }
  Serial.print('*');

  if(digitalRead(led_verde_3)){
    Serial.print(1); 
  }
  else if(digitalRead(led_amarelo_3))
  {
    Serial.print(2); 
  }
  else if(digitalRead(led_vermelho_3))
  {
    Serial.print(3); 
  }
  else{
    Serial.print(4); 
  }
  Serial.print('*');


  if(digitalRead(led_verde_4)){
    Serial.print(1); 
  }
  else if(digitalRead(led_amarelo_4))
  {
    Serial.print(2); 
  }
  else if(digitalRead(led_vermelho_4))
  {
    Serial.print(3); 
  }
  else{
    Serial.print(4); 
  }
  Serial.print('*');
  
  if(digitalRead(led_verde_5)){
    Serial.print(1);
  }
  else if(digitalRead(led_amarelo_5))
  {
    Serial.print(2); 
  }
  else if(digitalRead(led_vermelho_5))
  {
    Serial.print(3);
  }
  else{
    Serial.print(4); 
  }
  Serial.print('*');

   if(digitalRead(led_verde_6)){
    Serial.print(1);
  }
  else if(digitalRead(led_amarelo_6))
  {
    Serial.print(2); 
  }
  else if(digitalRead(led_vermelho_6))
  {
    Serial.print(3);
  }
  else{
    Serial.print(4); 
  }
  Serial.print('*');
  
   if(digitalRead(led_verde_7)){
    Serial.print(1);
  }
  else if(digitalRead(led_amarelo_7))
  {
    Serial.print(2); 
  }
  else if(digitalRead(led_vermelho_7))
  {
    Serial.print(3);
  }
  else{
    Serial.print(4); 
  }
 // Serial.print('*');
  Serial.println();
}


 
void setup() {

  
  //Semaforo 1
  pinMode(led_verde_1,OUTPUT);
  pinMode(led_amarelo_1,OUTPUT);
  pinMode(led_vermelho_1,OUTPUT);
  digitalWrite(led_verde_1,LOW);
  digitalWrite(led_amarelo_1,LOW);
  digitalWrite(led_vermelho_1,HIGH);
   //Semaforo 2
  pinMode(led_verde_2,OUTPUT);
  pinMode(led_amarelo_2,OUTPUT);
  pinMode(led_vermelho_2,OUTPUT);
  digitalWrite(led_verde_2,LOW);
  digitalWrite(led_amarelo_2,LOW);
  digitalWrite(led_vermelho_2,HIGH);
   //Semaforo 3
  pinMode(led_verde_3,OUTPUT);
  pinMode(led_amarelo_3,OUTPUT);
  pinMode(led_vermelho_3,OUTPUT);
  digitalWrite(led_verde_3,LOW);
  digitalWrite(led_amarelo_3,LOW);
  digitalWrite(led_vermelho_3,HIGH);
    //Semaforo 4
  pinMode(led_verde_4,OUTPUT);
  pinMode(led_amarelo_4,OUTPUT);
  pinMode(led_vermelho_4,OUTPUT);
  digitalWrite(led_verde_4,LOW);
  digitalWrite(led_amarelo_4,LOW);
  digitalWrite(led_vermelho_4,HIGH);
      //Semaforo 5
  pinMode(led_verde_5,OUTPUT);
  pinMode(led_amarelo_5,OUTPUT);
  pinMode(led_vermelho_5,OUTPUT);
  digitalWrite(led_verde_5,LOW);
  digitalWrite(led_amarelo_5,LOW);
  digitalWrite(led_vermelho_5,HIGH);

  pinMode(led_verde_6,OUTPUT);
  pinMode(led_amarelo_6,OUTPUT);
  pinMode(led_vermelho_6,OUTPUT);
  digitalWrite(led_verde_6,LOW);
  digitalWrite(led_amarelo_6,LOW);
  digitalWrite(led_vermelho_6,HIGH);

  pinMode(led_verde_7,OUTPUT);
  pinMode(led_amarelo_7,OUTPUT);
  pinMode(led_vermelho_7,OUTPUT);
  digitalWrite(led_verde_7,LOW);
  digitalWrite(led_amarelo_7,LOW);
  digitalWrite(led_vermelho_7,HIGH);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  delay(8000);
  
  Serial.begin(9600);
  delay(1000);

  //*****************************
  //Inicializacion de semaforo 1 en el Primer estadp
  estado_semaforo_1=estado_vermelho;
  estado_semaforo_2=estado_verde;
  estado_semaforo_3=estado_verde;
  estado_semaforo_4=estado_vermelho;
  estado_semaforo_5=estado_vermelho;
  estado_semaforo_6=estado_verde;
  estado_semaforo_7=estado_verde;
  
  tempo_ant=millis();
  tempo_req=1000;
  Habilitado_TR1=true;
  Habilitado_TV2=true;
  Habilitado_TV3=true;
  Habilitado_TR4=true;
  Habilitado_TR5=true;
  Habilitado_TR6=true;
  Habilitado_TR7=true;
  
 }



void maquina_de_estado(byte estado_maquina)
{
  //1.-*************************
  //1-Semaforo
  if(Habilitado_TV1){
    tempo_act=millis();
    if(tempo_act-tempo_ant>=tempo_req){
      Evento_TV1=true;
      }
    }
    if(Habilitado_TA1){
      tempo_act=millis();
      if(tempo_act-tempo_ant>=tempo_req){
        Evento_TA1=true;
        }
      }
      if(Habilitado_TR1){
      tempo_act=millis();
      if(tempo_act-tempo_ant>=tempo_req){
        Evento_TR1=true;
        }
      } 
//semaforo 2
 if(Habilitado_TV2){
      tempo_act=millis();
      if(tempo_act-tempo_ant>=tempo_req){
        Evento_TV2=true;
        }
      } 
       if(Habilitado_TA2){
      tempo_act=millis();
      if(tempo_act-tempo_ant>=tempo_req){
        Evento_TA2=true;
        }
      } 
       if(Habilitado_TR2){
      tempo_act=millis();
      if(tempo_act-tempo_ant>=tempo_req){
        Evento_TR2=true;
        }
      } 
       //Semaforo 3
  if(Habilitado_TV3){
    tempo_act=millis();
    if(tempo_act-tempo_ant>=tempo_req){
      Evento_TV3=true;
      }
    }
    if(Habilitado_TA3){
      tempo_act=millis();
      if(tempo_act-tempo_ant>=tempo_req){
        Evento_TA3=true;
        }
      }
      if(Habilitado_TR3){
      tempo_act=millis();
      if(tempo_act-tempo_ant>=tempo_req){
        Evento_TR3=true;
        }
      } 

     //Semaforo 4
  if(Habilitado_TV4){
    tempo_act=millis();
    if(tempo_act-tempo_ant>=tempo_req){
      Evento_TV4=true;
      }
    }
    if(Habilitado_TA4){
      tempo_act=millis();
      if(tempo_act-tempo_ant>=tempo_req){
        Evento_TA4=true;
        }
      }
      if(Habilitado_TR4){
      tempo_act=millis();
      if(tempo_act-tempo_ant>=tempo_req){
        Evento_TR4=true;
        }
      } 

     //Semaforo 5
  if(Habilitado_TV5){
    tempo_act=millis();
    if(tempo_act-tempo_ant>=tempo_req){
      Evento_TV5=true;
      }
    }
    if(Habilitado_TA5){
      tempo_act=millis();
      if(tempo_act-tempo_ant>=tempo_req){
        Evento_TA5=true;
        }
      }
      if(Habilitado_TR5){
      tempo_act=millis();
      if(tempo_act-tempo_ant>=tempo_req){
        Evento_TR5=true;
        }
      } 

           //Semaforo 6
  if(Habilitado_TV6){
    tempo_act=millis();
    if(tempo_act-tempo_ant>=tempo_req){
      Evento_TV6=true;
      }
    }
    if(Habilitado_TA6){
      tempo_act=millis();
      if(tempo_act-tempo_ant>=tempo_req){
        Evento_TA6=true;
        }
      }
      if(Habilitado_TR6){
      tempo_act=millis();
      if(tempo_act-tempo_ant>=tempo_req){
        Evento_TR6=true;
        }
      }

  //Semaforo 7
  if(Habilitado_TV7){
    tempo_act=millis();
    if(tempo_act-tempo_ant>=tempo_req){
      Evento_TV7=true;
      }
    }
    if(Habilitado_TA7){
      tempo_act=millis();
      if(tempo_act-tempo_ant>=tempo_req){
        Evento_TA7=true;
        }
      }
      if(Habilitado_TR7){
      tempo_act=millis();
      if(tempo_act-tempo_ant>=tempo_req){
        Evento_TR7=true;
        }
      } 
   
    
   //2.-*************************
   
if(Evento_TV2){
  estado_semaforo_1=estado_vermelho;
  estado_semaforo_3=estado_verde;
  estado_semaforo_2=estado_amarelo;
  estado_semaforo_4=estado_vermelho;
  estado_semaforo_5=estado_vermelho;
  estado_semaforo_6=estado_verde;
  estado_semaforo_7=estado_vermelho;
  tempo_ant=millis();
  tempo_req=2000;
  Habilitado_TR1=true;
  Habilitado_TA2=true;
  Habilitado_TV3=true;
  Habilitado_TR4=true;
  Habilitado_TR5=true;
  Habilitado_TV2=false;
  Habilitado_TV6=true;
  Habilitado_TR7=true;
  }

  if(Evento_TA2){
  estado_semaforo_1=estado_vermelho;
  estado_semaforo_3=estado_verde;
  estado_semaforo_2=estado_vermelho;
  estado_semaforo_4=estado_vermelho;
  estado_semaforo_5=estado_verde;
  estado_semaforo_6=estado_verde;
  estado_semaforo_7=estado_verde;
  tempo_ant=millis();
  tempo_req=2000;
  Habilitado_TR1=true;
  Habilitado_TR2=true;
  Habilitado_TV3=true;
  Habilitado_TR4=true;
  Habilitado_TR5=false;
  Habilitado_TV5=true;
  Habilitado_TA2=false;
  Habilitado_TV6=true;
  Habilitado_TV7=true;
  }

   if(Evento_TV5){
  estado_semaforo_1=estado_vermelho;
  estado_semaforo_2=estado_vermelho;
  estado_semaforo_3=estado_amarelo;
  estado_semaforo_4=estado_vermelho;  
  estado_semaforo_5=estado_amarelo;
  estado_semaforo_6=estado_amarelo;
  estado_semaforo_7=estado_amarelo;
  tempo_ant=millis();
  tempo_req=2000;
  Habilitado_TR1=true;
  Habilitado_TR2=true;
  Habilitado_TA3=true;
  Habilitado_TV3=false;
  Habilitado_TR4=true;
  Habilitado_TA5=true;
  Habilitado_TV5=false;
  Habilitado_TA6=true;
  Habilitado_TV6=false;
  Habilitado_TA7=true;
  Habilitado_TV7=false;
  }

  if(Evento_TA5){
  estado_semaforo_1=estado_verde;
  estado_semaforo_2=estado_vermelho;
  estado_semaforo_3=estado_vermelho;
  estado_semaforo_4=estado_verde;  
  estado_semaforo_5=estado_vermelho;
  estado_semaforo_6=estado_vermelho;
  estado_semaforo_7=estado_vermelho;
  tempo_ant=millis();
  tempo_req=2000;
  Habilitado_TR1=false;
  Habilitado_TV1=true;
  Habilitado_TR4=false;
  Habilitado_TV4=true;
  Habilitado_TR2=true;
  Habilitado_TA3=false;
  Habilitado_TR3=true;
  Habilitado_TA5=false;
  Habilitado_TR5=true;
  Habilitado_TA6=false;
  Habilitado_TR6=true;
  Habilitado_TA7=false;
  Habilitado_TR7=true;
  }

   if(Evento_TV1){
  estado_semaforo_1=estado_amarelo;
  estado_semaforo_2=estado_vermelho;
  estado_semaforo_3=estado_vermelho;
  estado_semaforo_4=estado_amarelo;  
  estado_semaforo_5=estado_vermelho;
  estado_semaforo_6=estado_vermelho;
  estado_semaforo_7=estado_vermelho;
  tempo_ant=millis();
  tempo_req=2000;
  Habilitado_TV1=false;
  Habilitado_TA1=true;
  Habilitado_TV4=false;
  Habilitado_TA4=true;
  Habilitado_TR2=true;
  Habilitado_TA3=false;
  Habilitado_TR3=true;
  Habilitado_TA5=false;
  Habilitado_TR5=true;
  Habilitado_TA6=false;
  Habilitado_TR6=true;
  Habilitado_TA7=false;
  Habilitado_TR7=true;
  }

  if(Evento_TA1){
  estado_semaforo_1=estado_vermelho;
  estado_semaforo_2=estado_verde;
  estado_semaforo_3=estado_verde;
  estado_semaforo_4=estado_vermelho;  
  estado_semaforo_5=estado_vermelho;
  estado_semaforo_6=estado_verde;
  estado_semaforo_7=estado_vermelho;
  
  tempo_ant=millis();
  tempo_req=2000;
  Habilitado_TA1=false;
  Habilitado_TR1=true;
  Habilitado_TA4=false;
  Habilitado_TR4=true;
  Habilitado_TR2=false;
  Habilitado_TV2=true;
  Habilitado_TR3=false;
  Habilitado_TV3=true;
  Habilitado_TR5=true;
  Habilitado_TR6=false;
  Habilitado_TV6=true;
  Habilitado_TR7=true;
  }





    

       //3.-*************************
  //Ejecions de estado
  switch(estado_maquina)
  {
    case 1:
      if(estado_semaforo_1==estado_verde){
    modo_verde_1();
    }else if(estado_semaforo_1==estado_amarelo){
    modo_amarelo_1();
    }else if(estado_semaforo_1==estado_vermelho){
    modo_vermelho_1();
    }

     if(estado_semaforo_2==estado_verde){
    modo_verde_2();
    }else if(estado_semaforo_2==estado_amarelo){
    modo_amarelo_2();
    }else if(estado_semaforo_2==estado_vermelho){
    modo_vermelho_2();
    }

      if(estado_semaforo_3==estado_verde){
    modo_verde_3();
    }else if(estado_semaforo_3==estado_amarelo){
    modo_amarelo_3();
    }else if(estado_semaforo_3==estado_vermelho){
    modo_vermelho_3();
    }

    if(estado_semaforo_4==estado_verde){
    modo_verde_4();
    }else if(estado_semaforo_4==estado_amarelo){
    modo_amarelo_4();
    }else if(estado_semaforo_4==estado_vermelho){
    modo_vermelho_4();
    }


   if(estado_semaforo_5==estado_verde){
    modo_verde_5();
    }else if(estado_semaforo_5==estado_amarelo){
    modo_amarelo_5();
    }else if(estado_semaforo_5==estado_vermelho){
    modo_vermelho_5();
    }

    if(estado_semaforo_6==estado_verde){
    modo_verde_6();
    }else if(estado_semaforo_6==estado_amarelo){
    modo_amarelo_6();
    }else if(estado_semaforo_6==estado_vermelho){
    modo_vermelho_6();
    }
    if(estado_semaforo_7==estado_verde){
    modo_verde_7();
    }else if(estado_semaforo_7==estado_amarelo){
    modo_amarelo_7();
    }else if(estado_semaforo_7==estado_vermelho){
    modo_vermelho_7();
    }
      break;

    case 2:
      if((estado_semaforo_1==estado_verde) && (!engarrafado3) && (haCarro1)){
    modo_verde_1();
   }
   else if((estado_semaforo_1==estado_verde) && (engarrafado3) && (Habilitado_TV1))
   {
    tempo_ant = millis()- tempo_req;
   }
   else if((estado_semaforo_1==estado_amarelo) && (!engarrafado3) && (haCarro1)){
    modo_amarelo_1();
   }
   else if((estado_semaforo_1==estado_amarelo) && (engarrafado3) && (Habilitado_TA1))
   {
    tempo_ant = millis()- tempo_req;
   }else if(estado_semaforo_1==estado_vermelho){
    modo_vermelho_1();
    }
    else{
     tempo_ant = millis()- tempo_req; 
    }

   if((estado_semaforo_2==estado_verde) && (!engarrafado6) && (haCarro4)){
    modo_verde_2();
   }
   else if((estado_semaforo_2==estado_verde) && (engarrafado6) && (Habilitado_TV2))
   {
    tempo_ant = millis()- tempo_req;
   }
   else if((estado_semaforo_2==estado_amarelo) && (!engarrafado6) && (haCarro4)){
    modo_amarelo_2();
   }
   else if((estado_semaforo_2==estado_amarelo) && (engarrafado6) && (Habilitado_TA2))
   {
    tempo_ant = millis()- tempo_req;
   }else if(estado_semaforo_2==estado_vermelho){
    modo_vermelho_2();
    }
    else{
     tempo_ant = millis()- tempo_req; 
    }

    if((estado_semaforo_3==estado_verde) && (!engarrafado9) && (haCarro7)){
    modo_verde_3();
   }
   else if((estado_semaforo_3==estado_verde) && (engarrafado9) && (Habilitado_TV3))
   {
    tempo_ant = millis()- tempo_req;
   }
   else if((estado_semaforo_3==estado_amarelo) && (!engarrafado9) && (haCarro7)){
    modo_amarelo_3();
   }
   else if((estado_semaforo_3==estado_amarelo) && (engarrafado9) && (Habilitado_TA3))
   {
    tempo_ant = millis()- tempo_req;
   }else if(estado_semaforo_3==estado_vermelho){
    modo_vermelho_3();
    }
    else{
     tempo_ant = millis()- tempo_req; 
    }

    
    if((estado_semaforo_4==estado_verde) && (!engarrafado6) && (haCarro11)){
    modo_verde_4();
   }
   else if((estado_semaforo_4==estado_verde) && (engarrafado6) && (Habilitado_TV4))
   {
    tempo_ant = millis()- tempo_req;
   }
   else if((estado_semaforo_4==estado_amarelo) && (!engarrafado6) && (haCarro11)){
    modo_amarelo_4();
   }
   else if((estado_semaforo_4==estado_amarelo) && (engarrafado6) && (Habilitado_TA4))
   {
    tempo_ant = millis()- tempo_req;
   }else if(estado_semaforo_4==estado_vermelho){
    modo_vermelho_4();
    }
    else{
     tempo_ant = millis()- tempo_req; 
    }
    
   if((estado_semaforo_5==estado_verde) && (!engarrafado14) && (haCarro13)){
    modo_verde_5();
   }
   else if((estado_semaforo_5==estado_verde) && (engarrafado14) && (Habilitado_TV5))
   {
    tempo_ant = millis()- tempo_req;
   }
   else if((estado_semaforo_5==estado_amarelo) && !engarrafado14 && haCarro13){
    modo_amarelo_5();
   }
   else if((estado_semaforo_5==estado_amarelo) && (engarrafado14) && (Habilitado_TA5))
   {
    tempo_ant = millis()- tempo_req;
   }
   else if(estado_semaforo_5==estado_vermelho){
    modo_vermelho_5();
    }
    else{
     tempo_ant = millis()- tempo_req; 
    }

    if((estado_semaforo_6==estado_verde) && (!engarrafado3) && (haCarro7)){
    modo_verde_6();
   }
   else if((estado_semaforo_6==estado_verde) && (engarrafado3) && (Habilitado_TV6))
   {
   }
   else if((estado_semaforo_6==estado_amarelo) && (!engarrafado3) && (haCarro7)){
    modo_amarelo_6();
   }
   else if((estado_semaforo_6==estado_amarelo) && (engarrafado3) && (Habilitado_TA6))
   {
    
   }else if(estado_semaforo_6==estado_vermelho){
    modo_vermelho_6();
    }

    if((estado_semaforo_7==estado_verde) && (!engarrafado6) && (haCarro13)){
    modo_verde_7();
   }
   else if((estado_semaforo_7==estado_verde) && (engarrafado6) && (Habilitado_TV7))
   {
    
   }
   else if((estado_semaforo_7==estado_amarelo) && (!engarrafado6) && (haCarro13)){
    modo_amarelo_7();
   }
   else if((estado_semaforo_7==estado_amarelo) && (engarrafado6) && (Habilitado_TA7))
   {
    
   }else if(estado_semaforo_7==estado_vermelho){
    modo_vermelho_7();
    }



    
      break;
  }
  
   
    //4.-*************************
  //Reiniciando Eventos
  Evento_TV1=false;
  Evento_TA1=false;
  Evento_TR1=false;
  Evento_TV2=false;
  Evento_TA2=false;
  Evento_TR2=false;
  Evento_TV3=false;
  Evento_TA3=false;
  Evento_TR3=false;
  Evento_TV4=false;
  Evento_TA4=false;
  Evento_TR4=false;
  Evento_TV5=false;
  Evento_TA5=false;
  Evento_TR5=false;
  Evento_TR5=false;
  Evento_TA6=false;
  Evento_TR6=false;
  Evento_TR6=false;
  Evento_TA7=false;
  Evento_TR7=false;
  Evento_TR7=false;
  
}


void modo_alerta()
{
    modo_amarelo_1();
    modo_amarelo_2();
    modo_amarelo_3();
    modo_amarelo_4();
    modo_amarelo_5();
    modo_amarelo_6();
    modo_amarelo_7();
}
void modo_apagados(){
  digitalWrite(led_verde_1,LOW);
  digitalWrite(led_amarelo_1,LOW);
  digitalWrite(led_vermelho_1,LOW);

  digitalWrite(led_verde_2,LOW);
  digitalWrite(led_amarelo_2,LOW);
  digitalWrite(led_vermelho_2,LOW);

  digitalWrite(led_verde_3,LOW);
  digitalWrite(led_amarelo_3,LOW);
  digitalWrite(led_vermelho_3,LOW);

  digitalWrite(led_verde_4,LOW);
  digitalWrite(led_amarelo_4,LOW);
  digitalWrite(led_vermelho_4,LOW);

  digitalWrite(led_verde_5,LOW);
  digitalWrite(led_amarelo_5,LOW);
  digitalWrite(led_vermelho_5,LOW);

  digitalWrite(led_verde_6,LOW);
  digitalWrite(led_amarelo_6,LOW);
  digitalWrite(led_vermelho_6,LOW);

    digitalWrite(led_verde_7,LOW);
  digitalWrite(led_amarelo_7,LOW);
  digitalWrite(led_vermelho_7,LOW);

}

void loop() {
  
  int LDR1 = analogRead(ldr1);    int LDR2 = analogRead(ldr2);
  int LDR3 = analogRead(ldr3);    int LDR4 = analogRead(ldr4);
  int LDR5 = analogRead(ldr5);    int LDR6 = analogRead(ldr6);
  int LDR7 = analogRead(ldr7);    int LDR8 = analogRead(ldr8);
  int LDR9 = analogRead(ldr9);    int LDR10 = analogRead(ldr10);
  int LDR11 = analogRead(ldr11);  int LDR12 = analogRead(ldr12);
  int LDR13 = analogRead(ldr13);  int LDR14 = analogRead(ldr14);
  int POT = analogRead(pot);

  marcoLDR = map(POT, 0, 1023, 100, 1000);
  if(LDR1>maximo){maximo=LDR1;}if(LDR2>maximo){maximo=LDR2;}
  if(LDR3>maximo){maximo=LDR3;}if(LDR4>maximo){maximo=LDR4;}
  if(LDR6>maximo){maximo=LDR6;}
  if(LDR7>maximo){maximo=LDR7;}if(LDR8>maximo){maximo=LDR8;}
  if(LDR9>maximo){maximo=LDR9;}if(LDR10>maximo){maximo=LDR10;}
  if(LDR12>maximo){maximo=LDR12;}if(LDR12>maximo){maximo=LDR12;}
  if(LDR13>maximo){maximo=LDR13;}if(LDR14>maximo){maximo=LDR14;}
  if(marcoLDR>maximo){digitalWrite(LED, HIGH);}
  else {digitalWrite(LED, LOW);}

  if(LDR1 > marcoLDR)
  {
     contadorHa1++;
     if(contadorHa1>nSegundos){
      contadorHa2=0;
      haCarro1= true;
     } 
  }else{
    contadorHa2++;
    if(contadorHa2>nSegundos){
      contadorHa1=0;
      haCarro1= false;
     } 
  }

  if(LDR4 > marcoLDR)
  {
     contadorHa4++;
     if(contadorHa4>nSegundos){
      contadorHa5=0;
      haCarro4= true;
     } 
  }else{
    contadorHa5++;
    if(contadorHa5>nSegundos){
      contadorHa4=0;
      haCarro4= false;
     } 
  }

  if(LDR7 > marcoLDR)
  {
     contadorHa7++;
     if(contadorHa7>nSegundos){
      contadorHa8=0;
      haCarro7= true;
     } 
  }else{
    contadorHa8++;
    if(contadorHa8>nSegundos){
      contadorHa7=0;
      haCarro7= false;
     } 
  }

  if(LDR11 > marcoLDR)
  {
     contadorHa11++;
     if(contadorHa11>nSegundos){
      contadorHa10=0;
      haCarro11= true;
     } 
  }else{
    contadorHa10++;
    if(contadorHa10>nSegundos){
      contadorHa11=0;
      haCarro11= false;
     } 
  }

  if(LDR13 > marcoLDR)
  {
     contadorHa13++;
     if(contadorHa13>nSegundos){
      contadorHa12=0;
      haCarro13= true;
     } 
  }else{
    contadorHa12++;
    if(contadorHa12>nSegundos){
      contadorHa13=0;
      haCarro13= false;
     } 
  }
  
  
  if((LDR1 > marcoLDR) && (LDR2 > marcoLDR))
  {
     contador1++;
     if(contador1>nSegundos){
      contador2=0;
      //Serial.println("Engarrafamento 1_2 confirmado");
      engarrafado1_2= true;
      
      //delay(10000);
     } 
  }else{
    contador2++;
    if(contador2>nSegundos){
      contador1=0;
      engarrafado1_2= false;
     } 
  }

//--------semaforo
  if((LDR4 > marcoLDR) && (LDR5 > marcoLDR))
  {
     contador4++;
     if(contador4>nSegundos){
      contador5=0;
      engarrafado4_5= true;
     } 
  }else{
    contador5++;
    if(contador5>nSegundos){
      contador4=0;
      engarrafado4_5= false;
     } 
  }

//--------semaforo
  if((LDR7 > marcoLDR) && (LDR8 > marcoLDR))
  {
     contador7++;
     if(contador7>nSegundos){
      contador8=0;
      engarrafado7_8= true;
     } 
  }else{
    contador8++;
    if(contador8 > nSegundos){
      contador7=0;
      engarrafado7_8= false;
     } 
  }

  //--------semaforo
  if((LDR10 > marcoLDR) && (LDR11 > marcoLDR))
  {
     contador10++;
     if(contador10>nSegundos){
      contador11=0;
      engarrafado10_11= true;
     } 
  }else{
    contador11++;
    if(contador11 > nSegundos){
      contador10=0;
      engarrafado10_11= false;
     } 
  }

  //--------semaforo5
  if((LDR12 > marcoLDR) && (LDR13 > marcoLDR))
  {
     contador12++;
     if(contador12>nSegundos){
      contador13=0;
      engarrafado12_13= true;
     } 
  }else{
    contador13++;
    if(contador13 > nSegundos){
      contador12=0;
      engarrafado12_13= false;
     } 
  }

  if(LDR3 > marcoLDR)
  {
     contador3_01++;
     if(contador3_01>nSegundos){
      contador3_02=0;
      engarrafado3= true;
     } 
  }else{
    contador3_02++;
    if(contador3_02 > nSegundos){
      contador3_01=0;
      engarrafado3= false;
     } 
  }
  if(LDR6 > marcoLDR)
  {
     contador6_01++;
     if(contador6_01>nSegundos){
      contador6_02=0;
      engarrafado6= true;
     } 
  }else{
    contador6_02++;
    if(contador6_02 > nSegundos){
      contador6_01=0;
      engarrafado6= false;
     } 
  }

  if(LDR9 > marcoLDR)
  {
     contador9_01++;
     if(contador9_01>nSegundos){
      contador9_02=0;
      engarrafado9= true;
     } 
  }else{
    contador9_02++;
    if(contador9_02 > nSegundos){
      contador9_01=0;
      engarrafado9= false;
     } 
  }

  if(LDR14 > marcoLDR)
  {
     contador14_01++;
     if(contador14_01>nSegundos){
      contador14_02=0;
      engarrafado14= true;
     } 
  }else{
    contador14_02++;
    if(contador14_02 > nSegundos){
      contador14_01=0;
      engarrafado14= false;
     } 
  }

  //------------------Contando engarrafamento----------------------
  if(engarrafado1_2)contadorEngarrafamento++;   if(engarrafado3){contadorEngarrafamento++;contadorEngarrafamentoAuxiliar++;}
  if(engarrafado4_5)contadorEngarrafamento++;   if(engarrafado6){contadorEngarrafamento++;contadorEngarrafamentoAuxiliar++;}
  if(engarrafado7_8)contadorEngarrafamento++;   if(engarrafado9){contadorEngarrafamento++;contadorEngarrafamentoAuxiliar++;}
  if(engarrafado10_11)contadorEngarrafamento++;   if(engarrafado12_13)contadorEngarrafamento++;
  if(engarrafado14){contadorEngarrafamento++;contadorEngarrafamentoAuxiliar++;}
  
  
     
     if(contadorEngarrafamentoAuxiliar==4) {
        tempo_act=millis();
        if(tempo_act-tempo_ant>=tempo_req){
          controleVez = !controleVez;
          tempo_ant = millis();
        if(controleVez)
        {  
          modo_alerta();
        }
        else{
          modo_apagados();
        } 
      
      }
      }
      else if((!haCarro1) && (!haCarro4) && (!haCarro7) && (!haCarro11) && (!haCarro13))
        maquina_de_estado(1);
        else
        maquina_de_estado(2);

 
  
  maximo=0;
  enviarDados();
  delay(1000);
  contadorEngarrafamento=0;
  contadorEngarrafamentoAuxiliar=0;

}
