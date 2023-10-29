/**************************************************
    AUTHOR: Otlevire
    FOR: 
    CREATED AT:28-10-2023
    Principais Componentes usados:
      *** 1-ESP32 (1)
      *** 2-DHT11
      *** 3-Sensor de Chama
      *** 4-Sensor de Fumo (MQ135)
      *** 5- Cooler
*************************************************/

////////////////// Labraries Used  ////////////////
#include <WiFi.h>                             /////
#include "DHT.h"                              /////
#include <Wire.h>                             /////
#include "SPIFFS.h"                           /////
#include <AsyncTCP.h>                         /////
#include <ArduinoJson.h>                      /////
#include <ESPAsyncWebServer.h>                /////
#include <LiquidCrystal_I2C.h>                /////
#include <IOXhop_FirebaseESP32.h>             /////
///////////////////////////////////////////////////

////////////////  PIN CONFIGURATIONS //////////////
#define LED 2                                 /////
#define DHTPIN 15                             /////
#define COOLER 4                              /////
#define DOOR 18                               /////
#define FLAME_SENSOR 34                       /////
#define SMOKE_SENSOR 35                       /////
#define FIREBASE_HOST "https://parque-control-default-rtdb.firebaseio.com/"  // substitua "Link_do_seu_banco_de_dados" pelo link do seu banco de dados
#define FIREBASE_AUTH "AIzaSyDljBC-KlS1MTJTXcNzbxsK-ROP30dnqsU"              // substitua "Senha_do_seu_banco_de_dados" pela senha do seu banco de dados
#define TIME_RESERVATION 120000               /////
#define DHTTYPE DHT11                         /////
///////////////////////////////////////////////////

/////////////  NETWORK CONFIGURATIONS /////////////
#define SSID "NETHOUSE"                       /////
#define PASSWORD "Eduanara3130"               /////
///////////////////////////////////////////////////

////////// VARIABLES USED IN THE PROJECT //////////
float temperature=0, humidity=0;              /////
int flame=0, smoke=0;                         /////
unsigned long int timeDelay = 0;              /////
///////////////////////////////////////////////////

///////////////  OBJECTS DEFINITIONS  /////////////
DHT dht(DHTPIN, DHTTYPE);                     /////
AsyncWebServer server(80);                    /////
LiquidCrystal_I2C lcd(0x27,16,2);             /////
///////////////////////////////////////////////////

////////////// FUNCTION DEFINITIONS  //////////////
void wifiConfig();                            /////
void initConfig();                            /////
bool initMyFS();                              /////
bool isUser(String, String);                  /////
void serverHandlers();                        /////
void saveUserFirebase(String, String);        /////
///////////////////////////////////////////////////

///////////////////////////////////////////////////
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

///////////////////////////////////////////////////
void setup() {
  initConfig();
  wifiConfig();
  Serial.println(initMyFS()?" ## PARTIÇÃO SPIFFS MONTADA! ##":" ## ERRO MONTANDO A PARTIÇÃO SPIFFS ##");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  serverHandlers();
  Serial.println(" ##-------SISTEMA DE DOMÓTICA!--------##");
}

///////////////////////////////////////////////////////////////////////////////
void loop() {
  if (millis() - timeDelay > 1000) {
    timeDelay = millis();
    readSensors();
    printLCD();
    digitalWrite(LED, !digitalRead(LED));
  }
  delay(20);
}

/////////////////////////////////////////////////////////
void initConfig() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  
  pinMode(COOLER, OUTPUT);
  digitalWrite(COOLER, LOW);
  
  pinMode(DOOR, OUTPUT);
  digitalWrite(DOOR, LOW);
  
  Serial.begin(115200);
  delay(500);
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hello, world!");
  lcd.setCursor(2,1);
  lcd.print("Ywrobot Arduino!");
  Serial.println("CONFIGURAÇÕES INICIAS SETADAS!");
  delay(5000);
}

/////////////////////////////////////////////////////////
void wifiConfig() {
  if (WiFi.status() == WL_CONNECTED)
    return;
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
void readSensors() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  flame = map(analogRead(FLAME_SENSOR), 0, 4095, 0, 100);
  smoke = map(analogRead(SMOKE_SENSOR), 0, 4095, 0, 100);
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Falha ao Ler os DHT11! Verifique as Conexões!"));
    humidity = temperature = 0;
  }
}

////////////////////////////////////////////////////////////////////////
bool initMyFS() {
  return (SPIFFS.begin(true));
}



bool isUser(String username, String password) {
  String pass = Firebase.getString("/users/" + username);
  return (pass.equals(password));
}

void saveUserFirebase(String username, String password) {
  Firebase.setString("/users/" + username, password);
}

bool isCoolerOn()
{
  return(digitalRead(COOLER));
}

bool isDoorOpen()
{
  return (digitalRead(DOOR));
}

void turnOffCooler()
{
  digitalWrite(COOLER, LOW);
}

void turnOnCooler()
{
  digitalWrite(COOLER, HIGH);
}

void closeDoor()
{
  digitalWrite(DOOR, LOW);
}

void openDoor()
{
  digitalWrite(DOOR, HIGH);
}

void printLCD()
{
  static byte flag = 0;
  lcd.clear();
  lcd.setCursor(0, 0);

  switch(flag){
    case 0:
      lcd.print("Temp:");
      lcd.print(temperature);
      lcd.print("*C");
      
      lcd.setCursor(0, 1);
      lcd.print("Hum.:");
      lcd.print(humidity);
      lcd.print("%");
    break;
    
    case 1:
      lcd.print("Chamas:");
      lcd.print(flame);
      lcd.print("%");
      
      lcd.setCursor(0, 1);
      lcd.print("Fumo.:");
      lcd.print(smoke);
      lcd.print("%");
    break;
    
    case 2:
      lcd.print("VENTILADOR:");
      lcd.print(isCoolerOn()?"ON":"OFF");
      
      lcd.setCursor(0, 1);
      lcd.print("DOOR.....:");
      lcd.print(isDoorOpen()?"OPEN":"CLOSED");      
    break;
  }
  if(++flag>2)flag=0;
}


////////////////////////////////////////////////////////////////////////
void serverHandlers() {
  // Route to load bootstrap.min.css file
  server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/bootstrap.min.css", "text/css");
  });
  
  // Route to load bootstrap.bundle.min.js file
  server.on("/bootstrap.bundle.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/bootstrap.bundle.min.js", "text/javascript");
  });
  
  // Route to load login.css file
  server.on("/login.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/login.css", "text/css");
  });
  
  // Route to load dashboard.css file
  server.on("/dashboard.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/dashboard.css", "text/css");
  });
  
  // Route to load dashboard.js file
  server.on("/dashboard.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/dashboard.js", "text/javascript");
  });

  


  server.on("/loginIcon.png", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/loginIcon.png", "image/jpeg");
  });
  server.on("/humidity.png", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/humidity.png", "image/jpeg");
    });
     
  server.on("/doorClose.png", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/doorClose.png", "image/jpeg");
    }); 
    
   server.on("/doorOpen.png", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/doorOpen.png", "image/jpeg");
    }); 
    server.on("/fire.png", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/fire.png", "image/jpeg");
    });  

    server.on("/nofire.png", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/nofire.png", "image/jpeg");
    });  
    server.on("/nosmoke.png", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/nosmoke.png", "image/jpeg");
    });  
    server.on("/smoke.png", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/smoke.png", "image/jpeg");
    });  
    server.on("/tel.jpg", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/tel.jpg", "image/jpeg");
    });  
    server.on("/temperature.jpeg", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/temperature.jpeg", "image/jpeg");
    });  

     server.on("/ventilador.png", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/ventilador.png", "image/jpeg");
    }); 
    server.on("/ventiladorOff.png", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/ventiladorOff.png", "image/jpeg");
    }); 
  
  


  /*--------------------------ENDPOINS---------------------------*/
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("-------->redirecionando para login.html");
    request->redirect("/login");
  });



  // POST request para login
  server.on("/getin", HTTP_POST, [](AsyncWebServerRequest *request) {
    Serial.println("------> Req de /getin do login.html");
    if (!(request->hasParam("username", true) && request->hasParam("password", true))) {
      Serial.println("---> Erro... Parametros de Login Incompletos!\nRedirecionando para ----> login.html");
      request->redirect("/login");
    } else {
      String username = request->getParam("username", true)->value();
      String password = request->getParam("password", true)->value();
      if (username.equals("admin") && password.equals("otlevire")) {
        Serial.println("--->Username e Senha de Admin Válidos!\nRedirecionando para ----> register.html");
        request->redirect("/register");
      } else {
        if (!isUser(username, password)) {
          Serial.println("--->Username e Senha Inválidos!\nRedirecionando para ---> login.html");
          request->redirect("/login");
        } else {
          Serial.println("--->Username e Senha Válidos!\nRedirecionando para ---> index.html");
          request->redirect("/dashboard");
        }
      }
    }
  });

  
  // POST request para Cadastro de Usuario
  server.on("/make-register", HTTP_POST, [](AsyncWebServerRequest *request) {
    Serial.println("------> Req de /register do register.html");
    if (!(request->hasParam("username", true) && request->hasParam("passwordUser", true) && request->hasParam("passwordAdmin", true))) {
      Serial.println("---> Erro... Parametros de Login Incompletos!\nRedirecionando para ----> login.html");
      request->redirect("/login");
    } else {
      String username = request->getParam("username", true)->value();
      String passwordUser = request->getParam("passwordUser", true)->value();
      String passwordAdmin = request->getParam("passwordAdmin", true)->value();

      if (passwordAdmin.equals("otlevire")) {
        Serial.println("--->Admin Válido! Salvando dados do novo usuario no Firebase...");
        Firebase.setString("/users/" + username, passwordUser);
        //saveUserFirebase(username, passwordUser);
        Serial.println("Dados do Usuario Salvos!");
        request->redirect("/login");
      } else {
        Serial.println("--->Admin Inválido!Redirecionando para register.html");
        request->redirect("/register");
      }
    }
  });

  // Route for login / web page
  server.on("/login", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("--------> login.html");
    request->send(SPIFFS, "/login.html");
  });

  // Route for root / web page
  server.on("/register", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("--------> register.html");
    request->send(SPIFFS, "/register.html");
  });

  // Route for root / web page
  server.on("/dashboard", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("--------> dashboard.html");
    request->send(SPIFFS, "/dashboard.html");
  });

 

  server.on("/dados", HTTP_GET, [](AsyncWebServerRequest *request) {
    /*  
      0-"D*" + 
      1-String(temperature) + 
      2-String(humidity) + 
      3-String(flame) + 
      4-String(smoke) + 
      5-String(isCoolerOn() ? "1" : "0") +
      6-String(isDoorOpen() ? "1" : "0") + "*"
    */
    String dataStored = "D*" + String(temperature) + "*" + String(humidity) + "*" + String(flame) + "*" + String(smoke) + "*" + String(isCoolerOn() ? "1" : "0") + "*" + String(isDoorOpen() ? "1" : "0") + "*";
    Serial.println("--------> Dados:" + dataStored);
    String resp = "{\"status\":\"success\", \"data\":\"" + dataStored + "\"}";
    request->send(200, "application/json", resp);
  });

  server.on("/cooler", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("--------> cooler");
    if (isCoolerOn())
      turnOffCooler();
    else
      turnOnCooler();
    request->send(200, "application/json", "{\"status\":\"success\"}");
  });

  server.on("/door", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("--------> door");
    if (isDoorOpen())
      closeDoor();
    else
      openDoor();
    request->send(200, "application/json", "{\"status\":\"success\"}");
  });


  server.onNotFound(notFound);
  server.begin();
}
