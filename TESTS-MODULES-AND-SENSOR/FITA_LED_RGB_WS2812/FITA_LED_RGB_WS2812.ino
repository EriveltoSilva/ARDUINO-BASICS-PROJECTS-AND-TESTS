#include <FastLED.h> // Library FastLED by Daniel Garcia

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 7
#define CLOCK_PIN 13
#define BOTAO_AUTO 8
#define BOTAO_TROCAR 9
#define BOTAO_NUM_LEDS 10

#define NUM_LEDS 47 

byte contadorCor = 0;
unsigned int inicio = 0;
unsigned int fim = 47;
unsigned long int tempoDelay=0;


CRGB cor= CRGB::White;
CRGB leds[NUM_LEDS];
CRGB cores[]={
              CRGB::White, CRGB::Red,             CRGB::Yellow,
              CRGB::Orange,CRGB::Pink,            CRGB::Blue, 
              CRGB::Green, CRGB::Purple,          CRGB::Aqua,
              CRGB::LightCoral, CRGB::DarkOrange,  CRGB::Aquamarine,
              CRGB::DarkBlue };


void setup() { 
  pinMode(BOTAO_AUTO, INPUT_PULLUP);
  pinMode(BOTAO_TROCAR, INPUT_PULLUP);
  pinMode(BOTAO_NUM_LEDS, INPUT_PULLUP);
  delay(1000);
  if(digitalRead(BOTAO_NUM_LEDS))
  {
    inicio = 18;
    fim=40;
  }
  else{
    inicio = 9;
    fim=47;
  }
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed

  for(byte i=inicio; i<fim; i++)
    leds[i] = cor;
  FastLED.show();  
}

void loop() 
{ 
  if(!digitalRead(BOTAO_AUTO)) // Trocar Automaticamente
  {
    if((millis()-tempoDelay)>(1000*5))
    {
      tempoDelay= millis();
      trocarCor();
    }
  }

  if(!digitalRead(BOTAO_TROCAR))// Trocar Manualmente
  {
    while(!digitalRead(BOTAO_TROCAR));
    trocarCor();
    delay(1500);
  }

  // Turn the LED on, then pause
  for(byte i=inicio; i<fim; i++)
    leds[i] = cor;
  FastLED.show();
  delay(500);
}

void trocarCor()
{
  if(++contadorCor>12) contadorCor=0;
    cor = cores[contadorCor];
}
