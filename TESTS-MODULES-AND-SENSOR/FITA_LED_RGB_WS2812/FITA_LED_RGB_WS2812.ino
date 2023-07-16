
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 75

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 7
#define CLOCK_PIN 13
#define BOTAO 8


byte contadorCor = 0;
unsigned long int tempoDelay=0;
// Define the array of leds
CRGB cor= CRGB::White;
CRGB leds[NUM_LEDS];
CRGB cores[]={
              CRGB::White, CRGB::Red,             CRGB::Yellow,
              CRGB::Orange,CRGB::Pink,            CRGB::Blue, 
              CRGB::Green, CRGB::Purple,          CRGB::Aqua,
              CRGB::LightCoral, CRGB::DarkOrange,  CRGB::Aquamarine,
              CRGB::DarkBlue };


void setup() { 
  pinMode(BOTAO, INPUT_PULLUP);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed  
}

void loop() { 

  if((millis()-tempoDelay)>(1000*30))
  {
    tempoDelay= millis();
    trocarCor();
  }

  if(!digitalRead(BOTAO))
  {
    while(!digitalRead(BOTAO));
    trocarCor();
    delay(1500);
  }

  // Turn the LED on, then pause
  for(byte i=0; i<NUM_LEDS; i++)
    leds[i] = cor;
  FastLED.show();

  delay(500);
  // Now turn the LED off, then pause
  /*for(byte i=0; i<NUM_LEDS; i++)
    leds[i] = cor;
  FastLED.show();
  delay(500);*/
}

void trocarCor()
{
  if(++contadorCor>12) contadorCor=0;
    cor = cores[contadorCor];
}
