/**************************************************
    AUTHOR: Erivelto Silva
    FOR:
    CREATED AT:16-12-2023
    Principais Componentes usados:
      *** 1-ESP32 (1)
      *** 2-
      *** 3-
      *** 4-
      *** 5-
*************************************************/

////////////////// Libraries Used  ////////////////
#include <WiFi.h>                             /////
//#include <Wire.h>                           /////
#include "SPIFFS.h"                           /////
#include <AsyncTCP.h>                         /////
#include <ArduinoJson.h>                      /////
#include <HardwareSerial.h>                   /////
#include <ESPAsyncWebServer.h>                /////
#include <LiquidCrystal_I2C.h>                /////
#include <IOXhop_FirebaseESP32.h>             /////
///////////////////////////////////////////////////


////////////////  PIN CONFIGURATIONS //////////////
#define LED 2                                 /////
#define RXD2 16   //RX da serial do ESP32     /////
#define TXD2 17   //TX da serial do ESP32     /////
// substitua "Link_do_seu_banco_de_dados" pelo link do seu banco de dados
#define FIREBASE_HOST "https://parque-control-default-rtdb.firebaseio.com/"
// substitua "Senha_do_seu_banco_de_dados" pela senha do seu banco de dados
#define FIREBASE_AUTH "AIzaSyDljBC-KlS1MTJTXcNzbxsK-ROP30dnqsU"
///////////////////////////////////////////////////

/////////////  NETWORK CONFIGURATIONS /////////////
#define SSID "GERADOR"                        /////
#define PASSWORD "123456789"                  /////
///////////////////////////////////////////////////

////////// VARIABLES USED IN THE PROJECT //////////
String dataStored = "D*0*0*0*0*0*0*0*0*0*";   /////
unsigned long int tempoDelay = 0;             /////
///////////////////////////////////////////////////

///////////////  OBJECTS DEFINITIONS  /////////////
//Serial 2 p/ comunicação com o Módulo GSM    /////
HardwareSerial arduino(2);                    /////
AsyncWebServer server(80);                    /////
LiquidCrystal_I2C lcd(0x27, 16, 2);           /////
///////////////////////////////////////////////////

////////////// FUNCTION DEFINITIONS  //////////////
void askData();                               /////
bool initMyFS();                              /////
void wifiConfig();                            /////
void initConfig();                            /////
void serverHandlers();                        /////
bool isUser(String, String);                  /////
void saveUserFirebase(String, String);        /////
///////////////////////////////////////////////////

/////////////////////////////////////////////////////////
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}


void setup() {
  initConfig();
  wifiConfig();
  Serial.println(initMyFS() ? " ## PARTIÇÃO SPIFFS MONTADA! ##" : " ## ERRO MONTANDO A PARTIÇÃO SPIFFS ##");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  serverHandlers();
  Serial.println(" ##-------SISTEMA DE DOMÓTICA!--------##");
}


void loop() {
  if ((millis() - tempoDelay) > 1500) {
    tempoDelay = millis();
    askData();
    String dataReceived = receiveData();
    if (dataReceived != "")
    {
      dataStored = dataReceived;
      Serial.println("--> RECEBIDO DO ARDUINO:" + dataStored);
    }
  }
  delay(1);
}
//Fim do Loop



void initConfig()
{
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.begin(9600);
  delay(500);
  arduino.begin(9600, SERIAL_8N1, RXD2, TXD2);
  delay(500);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("#  UNIVERSIDADE #");
  lcd.setCursor(0, 1);
  lcd.print("#    METODISTA  #");
  delay(3000);
  Serial.println("SISTEMA LIGADO!!\nTUDO PRONTO !!");
}

/////////////////////////////////////////////////////////
void wifiConfig() {
  if (WiFi.status() == WL_CONNECTED)
    return;
  Serial.println();
  Serial.print("CONECTANDO A WIFI:");
  Serial.println(SSID);
  Serial.print("PROCURANDO");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CONECTANDO A REDE");
  lcd.setCursor(5, 1);
  lcd.print(SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  for (int i = 0; WiFi.status() != WL_CONNECTED; i++) {
    digitalWrite(LED, !digitalRead(LED));
    delay(150);
    Serial.print(".");
    if (i == 100)
      ESP.restart();
  }
  Serial.print("\nCONECTADO AO WIFI NO IP:");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("##  CONECTADO ##");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  Serial.println(WiFi.localIP());
  delay(5000);
}

////////////////////////////////////////////////////////////////////////
bool initMyFS() {
  return (SPIFFS.begin(true));
}

//////////////////////////////////////////////////////////////////////////////
void askData()
{
  arduino.println('D');
}

//////////////////////////////////////////////////////////////////////////////
String receiveData()
{
  if (arduino.available())
  {
    String txt = "";
    while (arduino.available())
      txt = arduino.readStringUntil('\n');
    return txt;
  }
  return "";
}

////////////////////////////////////////////////////////////////////////
bool isUser(String username, String password) {
  String pass = Firebase.getString("/users/" + username);
  return (pass.equals(password));
}

////////////////////////////////////////////////////////////////////////
void saveUserFirebase(String username, String password)
{
  Firebase.setString("/users/" + username, password);
}


////////////////////////////////////////////////////////////////////////
void serverHandlers()
{
  //------------------------ ARQUIVOS ESTÁTICOS ----------------------------------
  server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/bootstrap.min.css", "text/css");
  });

  server.on("/bootstrap.bundle.min.js", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/bootstrap.bundle.min.js", "text/javascript");
  });

  server.on("/aos.css", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/aos.css", "text/css");
  });

  server.on("/aos.js", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/aos.js", "text/javascript");
  });

  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/index.js", "text/javascript");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

  server.on("/gerador.jpg", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/gerador.jpg", "image/jpg");
  });

  server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/index.css", "text/css");
  });
  
  

  

  server.on("/login.css", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/login.css", "text/css");
  });

  server.on("/login.js", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/login.js", "text/javascript");
  });


  //----------------------------- ROTAS ------------------------------
  // POST request para login
  server.on("/getin", HTTP_POST, [](AsyncWebServerRequest * request)
  {
    Serial.println("------> Req de /getin do login.html");
    if (!(request->hasParam("username", true) && request->hasParam("password", true)))
    {
      Serial.println("---> Erro... Parametros de Login Incompletos!\nRedirecionando para ----> login.html");
      request->redirect("/login");
    }
    else {
      String username = request->getParam("username", true)->value();
      String password = request->getParam("password", true)->value();
      if (username.equals("admin") && password.equals("otlevire"))
      {
        Serial.println("--->Username e Senha de Admin Válidos!\nRedirecionando para ----> register.html");
        request->redirect("/register");
      }
      else {
        if (!isUser(username, password))
        {
          Serial.println("--->Username e Senha Inválidos!\nRedirecionando para ---> login.html");
          request->redirect("/login");
        }
        else {
          Serial.println("--->Username e Senha Válidos!\nRedirecionando para ---> index.html");
          request->redirect("/home");
        }
      }
    }
  });

  // POST request para Cadastro de Usuario
  server.on("/make-register", HTTP_POST, [](AsyncWebServerRequest * request)
  {
    Serial.println("------> Req de /register do register.html");
    if (!(request->hasParam("username", true) && request->hasParam("passwordUser", true) && request->hasParam("passwordAdmin", true)))
    {
      Serial.println("---> Erro... Parametros de Login Incompletos!\nRedirecionando para ----> login.html");
      request->redirect("/login");
    }
    else {
      String username = request->getParam("username", true)->value();
      String passwordUser = request->getParam("passwordUser", true)->value();
      String passwordAdmin = request->getParam("passwordAdmin", true)->value();

      if (passwordAdmin.equals("otlevire"))
      {
        Serial.println("--->Admin Válido! Salvando dados do novo usuario no Firebase...");
        Firebase.setString("/users/" + username, passwordUser);
        Serial.println("Dados do Usuario Salvos!");
        request->redirect("/login");
      }
      else {
        Serial.println("--->Admin Inválido!Redirecionando para register.html");
        request->redirect("/register");
      }
    }
  });


  server.on("/login", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    Serial.println("--------> login.html");
    request->send(SPIFFS, "/login.html");
  });

  server.on("/register", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    Serial.println("--------> register.html");
    request->send(SPIFFS, "/register.html");
  });

  server.on("/home", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    Serial.println("--------> Index.html");
    request->send(SPIFFS, "/index.html");
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    Serial.println("-------->redirecionando para login.html");
    request->redirect("/login");
  });





  server.on("/gerador", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    Serial.println("--------> gerador:");
    Serial.println("======= MUNDADO ESTADO GERADOR ====================");
    arduino.println("G");
    request->send(200, "application/json", "{\"status\":\"success\"}");
  });

  server.on("/modo", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    Serial.println("--------> modo:");
    Serial.println("======= TROCANDO MODO =============================");
    arduino.println("M");
    request->send(200, "application/json", "{\"status\":\"success\"}");
  });





  server.on("/dados", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    //Serial.println("--------> Dados:" + dataStored);
    dataStored.replace("\n", "");
    dataStored.replace("\r", "");
    String resp = "{\"status\":\"success\", \"data\":\"" + dataStored + "\"}";
    request->send(200, "application/json", resp);
  });



  server.onNotFound(notFound);
  server.begin();
}
