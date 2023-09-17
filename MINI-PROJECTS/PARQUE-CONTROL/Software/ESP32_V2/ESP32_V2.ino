/**************************************************
    AUTHOR: ERIVELTO CLÉNIO DA COSTA E SILVA
    FOR:
    CREATED AT: 10-09-2023.
    Objectivo: Controlar um carro robotizado de 4 rodas por meio do uma página web
      no computador ou celular, isso via WIFI.


    Principais Componentes usados:
      *** 1-ESP32 (1)
      *** 2- (1)
      *** 3- (1)
      *** 4- (1)
*************************************************/

////////////////// Labraries Used  ////////////////
#include <WiFi.h>                             /////
#include "SPIFFS.h"                           /////
#include <AsyncTCP.h>                         /////
#include <ArduinoJson.h>
//#include <Arduino_JSON.h>
#include <ESPAsyncWebServer.h>                /////
///////////////////////////////////////////////////

////////////////  PIN CONFIGURATIONS //////////////
#define LED 2                                 /////
#define RXD2 16  //RX da serial do ESP32      /////
#define TXD2 17  //TX da serial do ESP32      /////
///////////////////////////////////////////////////

/////////////  NETWORK CONFIGURATIONS /////////////
#define SSID "O"                              /////
#define PASSWORD "123456789"                  /////
///////////////////////////////////////////////////

////////// VARIABLES USED IN THE PROJECT //////////
bool flagAgend1 = false;          /////
bool flagAgend2 = false;          /////
String dataStored = "";
unsigned long int timeDelay = 0;  /////
const char* PARAM_MESSAGE = "message";
///////////////////////////////////////////////////

///////////////  OBJECTS DEFINITIONS  /////////////
AsyncWebServer server(80);                    /////
HardwareSerial arduino(2);                    /////
///////////////////////////////////////////////////

////////////// FUNCTION DEFINITIONS  //////////////
void wifiConfig();                            /////
void initConfig();                            /////
bool initMyFS();                              /////
void serverHandlers();                        /////
///////////////////////////////////////////////////

////////////////////////////////////////////////////
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

////////////////////////////////////////////////////////////////////////////////
void setup() {
  initConfig();
  wifiConfig();

  if (initMyFS())
    Serial.println(" ## PARTIÇÃO SPIFFS MONTADA! ##");
  else
    Serial.println(" ## ERRO MONTANDO A PARTIÇÃO SPIFFS ##");

  serverHandlers();
  Serial.println(" ##-------SISTEMA DE DOMÓTICA!--------##");
}

///////////////////////////////////////////////////////////////////////////////
void loop() {
  if(millis()-timeDelay>800)
  {
     timeDelay = millis();
     askData();
     String dataReceived = receiveData();
     if(dataReceived != "")
     {
       dataStored = dataReceived;
       Serial.println("--------------Arduino --> ESP32:--------------\nR:" + dataStored);
       digitalWrite(LED, !digitalRead(LED));
     }
  }
  delay(20);
}

/////////////////////////////////////////////////////////
void initConfig() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  delay(500);
  arduino.begin(9600, SERIAL_8N1, RXD2, TXD2);
  delay(500);
  Serial.println("CONFIGURAÇÕES INICIAS SETADAS!");
}

/////////////////////////////////////////////////////////
void wifiConfig() {
  if (WiFi.status() == WL_CONNECTED) return;
  Serial.println();
  Serial.print("CONECTANDO A WIFI:");
  Serial.println(SSID);
  Serial.print("PROCURANDO");
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
  Serial.println(WiFi.localIP());
}

//////////////////////////////////////////////////////////////////////////////
void askData() {
  arduino.println('R');
}

//////////////////////////////////////////////////////////////////////////////
String receiveData() {
  if (arduino.available()) {
    String txt = "";
    while (arduino.available()) 
      txt= arduino.readStringUntil('\n');
    return txt;
  }
  return "";
}

bool initMyFS()
{
  return (SPIFFS.begin(true));
}

String readMyFS(String path)
{
  /**
     Return value meaning:
     "": Error opening file
     string text: success reading the file
  */

  if (path.charAt(0) != '/')  path = "/" + path;
  File file = SPIFFS.open(path);
  if (!file)  return "";
  String content = "";
  while (file.available())
    content += (char)file.read();
  file.close();
  return content;
}

int appendMyFile(String path, String content)
{
  /**
     Return value meaning:
     -1: Error opening file
      0: Error appending in the file open
      1: success appending in the file
  */

  if (path.charAt(0) != '/') path = "/" + path;
  File file = SPIFFS.open(path, FILE_APPEND);
  if (!file) return -1;
  byte flag = (file.print(content + "\n")) ? 1 : 0;
  file.close();
  return flag;
}

int writeMyFile(String path, String content)
{
  /**
     Return value meaning:
     -1: Error opening file
      0: Error writing in the file open
      1: success writing in the file
  */
  if (path.charAt(0) != '/') path = "/" + path;
  File file = SPIFFS.open(path, FILE_WRITE);
  if (!file) return -1;
  byte flag = (file.print(content + "\n")) ? 1 : 0;
  file.close();
  return flag;
}



void serverHandlers()
{
  // Route to load bootstrap.min.css file
  server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/bootstrap.min.css", "text/css");
  });
  // Route to load bootstrap.bundle.min.js file
  server.on("/bootstrap.bundle.min.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/bootstrap.bundle.min.js", "text/javascript");
  });
  // Route to load image-park.jpeg file
  server.on("/image-park.jpeg", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/image-park.jpeg", "image/jpeg");
  });
  // Route to load vaga-livre.png file
  server.on("/vaga-livre.png", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/vaga-livre.png", "image/png");
  });
  // Route to load vaga-ocupada.png file
  server.on("/vaga-ocupada.png", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/vaga-ocupada.png", "image/png");
  });
  // Route to load index.css file
  server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.css", "text/css");
  });
  // Route to load index.js file
  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.js", "text/javascript");
  });


  /*--------------------------ENDPOINS---------------------------*/
  // Route for root / web page
  server.on("/welcome", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("-------->redirecionando para index.html");
    request->redirect("/");
  });
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("--------> Index.html");
    request->send(SPIFFS, "/index.html");
  });

  server.on("/dados", HTTP_GET, [] (AsyncWebServerRequest * request) {
    Serial.println("--------> Dados:"+dataStored);
    dataStored.replace("\n", "");
    dataStored.replace("\r", "");      
    String resp = "{\"status\":\"success\", \"data\":\""+dataStored+"\"}";
    request->send(200, "application/json", resp);
  });

  server.onNotFound(notFound);
  server.begin();
}
