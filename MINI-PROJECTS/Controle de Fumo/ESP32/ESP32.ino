/**
 * Componente                        ESP32          
 * DHT11 --------------------------> 15
 * BUZZER-------------------------->  4
 * MQ135 --------------------------> 34
 * CHAMAS -------------------------> 35
 * GSM(TX, RX) --------------------> RX2(16), TX2(17)
 * LCD com I2C(SCL, SDA) ----------> SCL(22), SDA(21)  
 * BOTÃO DE PRESSÃO  --------------> 5
 * 
*/


#define BLYNK_TEMPLATE_ID "TMPL2zm081CXh"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "DXEkUH3iqFmJsVuaKH3mSkLFKby8NYmF"
#define BLYNK_PRINT Serial

//////////////////////////////// DEFINITIONS ////////////////////////////
#define LED 2
#define BUZZER 4
#define DHTPIN 15
#define RXD2 16
#define TXD2 17
#define MQ135 34
#define CHAMA 35


#define DHTTYPE DHT11  //Ou poderia ser o DHT22
#define TEMPO_CHAMADA 15000
#define TEMPO_ATE_CHAMADA 10000
#define NUMERO_CHAMADAS 3

#define LIMIAR_TEMPERATURA 35  // Valor para começar deteção(em graus Celcius)
#define LIMIAR_FUMO 60         // Valor para começa a detectar (indo de 0 a 1023)
#define LIMIAR_CHAMA 55        // Valor para começar deteção(em percentagem)

#define ENDERECO_RESIDENCIA "Angola, Luanda, Rangel, Rua Rubra, Casa nº 32"

////////////////////////////// LIBRARIES ////////////////////////////////
#include "DHT.h"
#include <WiFi.h>
#include <String.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


// Your WiFi credentials.
char ssid[] = "AFONSO";
char pass[] = "123456789";

////////////////////////////// GLOBAL VARIEBLES ////////////////////////
int valorFumo = 0, valorChama = 0;
float temperatura = 0, humidade = 0;
const String NUMERO_AUTORIDADE = "+244955749112";
const String NUMERO_PROPRIETARIO = "+244940811141";
unsigned long int tempoDelay = 0, temporizador = 0;
bool flagSMSAutoridade = false, flagChamadaAutoridade = false;
bool flagSMSProprietario = false, flagChamadaProprietario = false;
bool flagFumo = false, flagChama = false, flagTemperatura = false;
String estadoFumo = "", estadoChama = "", estadoTemperatura = "";

////////////////////////////// OBEJECTS ////////////////////////////////
BlynkTimer timer;
HardwareSerial gsm(2);
DHT dht(DHTPIN, DHTTYPE);


// This function sends Arduino's uptime every second to Virtual Pin 2.
void sendDadaToBlynk() {
  Blynk.virtualWrite(V0, temperatura);
  Blynk.virtualWrite(V1, humidade);
  Blynk.virtualWrite(V2, valorChama);
  Blynk.virtualWrite(V3, valorFumo);
}

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  Serial.begin(115200);
  delay(1000);
  gsm.begin(9600, SERIAL_8N1, RXD2, TXD2);
  delay(1000);
  dht.begin();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, sendDadaToBlynk);
}

void loop() {
  comunication();
  if (millis() - tempoDelay > 1500) {
    tempoDelay = millis();
    lerSensores();
    // mostrarDados();
    analise();
    digitalWrite(LED, !digitalRead(LED));
  }

  String response = receberSMS();
  if (response.indexOf("+CMT:") > 0) {
    if (response.indexOf("DESLIGAR") > 0) {
      // flagChamada = false;
      Serial.println("ALARME DESLIGADO");
    } else if (response.indexOf("LIGAR") > 0) {
      // flagAlarmeEnvio = true;
      Serial.println("ALARME LIGADO");
    }
  }

  Serial.println("Hello");
  Blynk.run();
  timer.run();
}


////////////////////////////////////////////////////////////////////////
void lerSensores() {
  humidade = dht.readHumidity();
  temperatura = dht.readTemperature();

  valorFumo = 100 - map(analogRead(MQ135), 0, 4095, 0, 100);
  valorChama = map(analogRead(CHAMA), 0, 4095, 0, 100);
  if (isnan(humidade) || isnan(temperatura)) {
    temperatura = humidade = 0;
    Serial.println(F("Falha ao Detectar o Sensor DHT! Verifique as ligações"));
  }

  if (valorChama > LIMIAR_CHAMA && !flagChama) {
    flagChama = true;
    ativarAlarme();
  } else if (valorChama < LIMIAR_CHAMA && flagChama) {
    flagChama = false;
    desativarAlarme();
  }

  if (valorFumo > LIMIAR_FUMO && !flagFumo) {
    flagFumo = true;
    ativarAlarme();
  } else if (valorFumo < LIMIAR_FUMO && flagFumo) {
    flagFumo = false;
    desativarAlarme();
  }

  flagTemperatura = (temperatura > LIMIAR_TEMPERATURA);

  estadoTemperatura = (flagTemperatura) ? "ALTA TEMPERATURA DETECTADA!" : "TEMPERATURA NORMAL";
  estadoFumo = (flagFumo) ? "POSSIVEL INCÊNCIO DETECTADO" : "NORMAL, SEM FUMO";
  estadoChama = (flagChama) ? "FOGO DETECTADO!" : "NORMAL, SEM FOGO!";
}

/////////////////////////////////////////////////////////////////////////
void ativarAlarme() {
  digitalWrite(BUZZER, HIGH);
}

void desativarAlarme() {
  digitalWrite(BUZZER, LOW);
}


////////////////////////////////////////////////////////////////////////
void mostrarDados() {
  Serial.println("--------------------------DADOS----------------------------");
  Serial.println("Temperatura:" + String(temperatura) + "*C, " + estadoTemperatura);
  Serial.println("Humidade:" + String(humidade) + "%");
  Serial.println("MQ135:" + String(valorFumo) + ", " + estadoFumo);
  Serial.println("Chama:" + String(valorChama) + "%, " + estadoChama);
}



void analise() {
  static byte cont1 =0;
  static byte cont2 =0;

  if (flagTemperatura || flagFumo || flagChama) {
    if (!flagSMSProprietario && !flagChamadaProprietario) 
    {
      flagSMSProprietario = true;
      String mensagem = getMensagem();
      enviarSMS(NUMERO_PROPRIETARIO, mensagem);
      temporizador = millis();
    } else if (flagSMSProprietario && !flagChamadaProprietario && (millis() - temporizador) > TEMPO_ATE_CHAMADA) 
    {
      if (++cont1==NUMERO_CHAMADAS)
        flagChamadaProprietario = true;
      fazerChamada(NUMERO_PROPRIETARIO, TEMPO_CHAMADA);
      temporizador = millis();
    } else {
      Serial.println("Usuario já contactado");

      if (!flagSMSAutoridade && !flagChamadaAutoridade && (millis() - temporizador) > 20000) {
        flagSMSAutoridade = true;
        String mensagem = getMensagemAutoridade();
        enviarSMS(NUMERO_AUTORIDADE, mensagem);
        temporizador = millis();
      } 
      else if (flagSMSAutoridade && !flagChamadaAutoridade && (millis() - temporizador) > TEMPO_ATE_CHAMADA) 
      {
        if (++cont2==NUMERO_CHAMADAS)
          flagChamadaAutoridade = true;
        fazerChamada(NUMERO_AUTORIDADE, TEMPO_CHAMADA);
        temporizador = millis();
      } else {
        Serial.println("Autoridade já contactada");
      }
    }

  } else {
    flagSMSProprietario = false;
    flagChamadaProprietario = false;
    flagSMSAutoridade = false;
    flagChamadaAutoridade = false;
    cont1=0;
    cont2=0;
  }
}

////////////////////////////////////////////////////////////////////////
String getMensagem() {
  String texto = "";
  if (flagTemperatura || flagFumo || flagChama)
    texto += "==== ALERTA DE INCENDIO ====\n";
  texto += "Local......:Minha Residencia\n";
  texto += "SMS Para...:Proprietario da Residencia\n";
  texto += "Temperatura:" + String(temperatura) + "*C, " + estadoTemperatura + "\n";
  texto += "Humidade...:" + String(humidade) + "%\n";
  texto += "MQ135......:" + String(valorFumo) + ", " + estadoFumo + "\n";
  texto += "Chama......:" + String(valorChama) + "%, " + estadoChama + "\n";
  return texto;
}

String getMensagemAutoridade() {
  String texto = "";
  if (flagTemperatura || flagFumo || flagChama)
    texto += "===== ALERTA DE INCENDIO =====\n";
  texto += "Local......:" + String(ENDERECO_RESIDENCIA)+"\n";
  texto += "SMS Para...: Centro de Combate ao Incendio\n";
  texto += "Temperatura:" + String(temperatura) + "*C, " + estadoTemperatura + "\n";
  texto += "Humidade...:" + String(humidade) + "%\n";
  texto += "MQ135......:" + String(valorFumo) + ", " + estadoFumo + "\n";
  texto += "Chama......:" + String(valorChama) + "%, " + estadoChama + "\n";
  texto += "Pede-se a accao imediata do \"Centro de Combate a Incendio\"\n";
  return texto;
}



////////////////////////////////////////////////////////////////////////
void enviarSMS(const String number, String sms) {
  Serial.println("\n################ ENVIANDO MENSAGENS ! #################");
  gsm.println("AT+CMGF=1");
  delay(1000);
  comunication();
  gsm.println("AT+CMGS=\"" + number + "\"\r");
  delay(1000);
  comunication();
  gsm.println(sms);
  delay(100);
  gsm.println((char)26);
  delay(1000);
  comunication();
  delay(4000);
  Serial.println(sms);
  Serial.println("\n################ MENSAGEM ENVIADA! #################");
  configGSM();
}

////////////////////////////////////////////////////////////////////////
void fazerChamada(const String number, unsigned long time) {
  Serial.println("###################### FAZENDO CHAMADA ##################################");
  gsm.println("ATD +" + String(number) + ";");
  delay(100);
  gsm.println();
  delay(time);
  gsm.println("ATH");
  Serial.println("###################### TERMINANDO CHAMADA ##################################");
}

void configGSM() {
  gsm.print("AT+CMGF=1\r");
  delay(1000);
  gsm.print("AT+CNMI=2,2,0,0,0\r");
  delay(1000);
}

///////////////////////////////////////
String receberSMS() {
  String answer = "";
  if (gsm.available() > 0) {
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
void comunication() {
  if (Serial.available())
    while (Serial.available())
      gsm.write(Serial.read());

  if (gsm.available())
    while (gsm.available())
      Serial.write(gsm.read());
}
