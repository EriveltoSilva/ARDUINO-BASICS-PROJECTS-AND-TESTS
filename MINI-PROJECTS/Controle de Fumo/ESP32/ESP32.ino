////////////////////////////// LIBRARIES ////////////////////////////////
#include "DHT.h"
#include <String.h>

//////////////////////////////// DEFINITIONS ////////////////////////////
#define LED 2
#define DHTPIN 15
#define RXD2 16  
#define TXD2 17 
#define MQ135 34
#define CHAMA 35


#define DHTTYPE DHT22   //Ou poderia ser o DHT11 
#define TEMPO_CHAMADA 15000
#define TEMPO_ATE_CHAMADA 10000
#define SERIAL_BAUD_RATE 9600

#define LIMIAR_TEMPERATURA 35 // Valor para começar deteção(em graus Celcius)
#define LIMIAR_FUMO 350       // Valor para começa a detectar (indo de 0 a 1023)
#define LIMIAR_CHAMA 55       // Valor para começar deteção(em percentagem)

////////////////////////////// GLOBAL VARIEBLES ////////////////////////
int valorFumo=0, valorChama=0;
float temperatura=0, humidade=0;
bool flagAlarme=true, flagEnvio=false;
bool flagFumo=false, flagChama=false, flagTemperatura=false;
String estadoFumo="", estadoChama="", estadoTemperatura="";
unsigned long int tempoDelay = 0, temporizador=0;
const String NUMERO_TELEFONE = "+244940811141";

////////////////////////////// OBEJECTS ////////////////////////////////
HardwareSerial gsm(2);
DHT dht(DHTPIN, DHTTYPE);

////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.begin(SERIAL_BAUD_RATE);
  delay(1000);
  gsm.begin(SERIAL_BAUD_RATE, SERIAL_8N1, RXD2, TXD2);
  delay(1000);
  dht.begin();
  fazerChamada(NUMERO_TELEFONE, TEMPO_CHAMADA);
}

////////////////////////////////////////////////////////////////////////
void loop() {
  String response = receberSMS();
  if (response.indexOf("+CMT:") > 0)
  {
    if (response.indexOf("DESLIGAR") > 0)
    {
      flagAlarme=false;
      Serial.println("ALARME DESLIGADO");
    }
    else if (response.indexOf("LIGAR") > 0)
    {
      flagAlarme=true;
      Serial.println("ALARME LIGADO");
    }
  }

  if(millis()-tempoDelay>1000){
    tempoDelay = millis();
    lerSensores();
    mostrarDados();
    analise();

    digitalWrite(LED, !digitalRead(LED));
  }
  
  delay(10);
}

////////////////////////////////////////////////////////////////////////
void lerSensores()
{
  humidade = dht.readHumidity();
  temperatura = dht.readTemperature();
  valorFumo = analogRead(MQ135);
  valorChama = 100-map(analogRead(CHAMA), 0, 1023, 0,100 );
  if (isnan(humidade) || isnan(temperatura))
  {
    temperatura = humidade = 0;
    Serial.println(F("Falha ao Detectar o Sensor DHT! Verifique as ligações"));
  }
  
  flagTemperatura = (temperatura>LIMIAR_TEMPERATURA);
  flagFumo = (valorFumo >LIMIAR_FUMO);
  flagChama = (valorChama >LIMIAR_CHAMA);

  estadoTemperatura = (flagTemperatura)? "ALTA TEMPERATURA DETECTADA!": "TEMPERATURA NORMAL";
  estadoFumo = (flagFumo)? "POSSIVEL INCÊNCIO DETECTADO": "NORMAL, SEM FUMO";
  estadoChama = (flagChama)? "FOGO DETECTADO!": "NORMAL, SEM FOGO!";
}

////////////////////////////////////////////////////////////////////////
void mostrarDados()
{
  Serial.println("--------------------------DADOS----------------------------");
  Serial.println("Temperatura:"+String(temperatura)+"*C, "+estadoTemperatura);
  Serial.println("Humidade:"+String(humidade)+"%");
  Serial.println("MQ135:"+String(valorFumo)+", "+estadoFumo);
  Serial.println("Chama:"+String(valorChama)+"%, "+estadoChama);

}

void analise()
{
  if(flagTemperatura || flagFumo || flagChama)
  {
    if(flagEnvio==false && flagAlarme==true)
    {
      temporizador = millis();
      flagEnvio=true;
      enviarSMS(NUMERO_TELEFONE,getMensagem());
    }
    else if(flagEnvio==true && flagAlarme && (millis()-temporizador)>TEMPO_ATE_CHAMADA)
    {
      fazerChamada(NUMERO_TELEFONE,TEMPO_CHAMADA);
    }
  }
}

////////////////////////////////////////////////////////////////////////
String getMensagem()
{
  String texto = "";
  if(flagTemperatura || flagFumo || flagChama) 
    texto += "==================== ALERTA ====================\n";
  texto += "Temperatura:"+String(temperatura)+"*C, "+estadoTemperatura+"\n";
  texto += "Humidade:"+String(humidade)+"%\n";
  texto += "MQ135:"+String(valorFumo)+", "+estadoFumo+"\n";
  texto += "Chama:"+String(valorChama)+"%, "+estadoChama+"\n";
  return texto;
}

////////////////////////////////////////////////////////////////////////
void enviarSMS(const String number, String sms)
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

////////////////////////////////////////////////////////////////////////
void fazerChamada(const String number, unsigned long time) 
{
  gsm.println("ATD +" + String(number) +";");
  delay(100);
  gsm.println();
  delay(time);// chama durante time milli segundos
  gsm.println("ATH"); // hang up
}

void configGSM()
{
  gsm.print("AT+CMGF=1\r");                         //Set gsm in text mode
  delay(1000);                                      //Wait 1 second
  gsm.print("AT+CNMI=2,2,0,0,0\r");                 //Put gsm waiting for a new message get received
  delay(1000);                                      //Wait 1 second
}

///////////////////////////////////////
String receberSMS()
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
////////////////////////////////////////////////////////////////////////
void comunication()
{
  if (Serial.available())
    while (Serial.available())
      gsm.write(Serial.read());

  if (gsm.available())
    while (gsm.available())
      Serial.write(gsm.read());
}