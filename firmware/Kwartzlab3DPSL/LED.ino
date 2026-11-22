#include <Adafruit_NeoPixel.h>

#define LED_STATUS 0

Adafruit_NeoPixel pixels(16, 21, NEO_GRB + NEO_KHZ800);
uint32_t flashingPixelColours[10];
uint32_t isPixelFlashing[10];

void LEDInit(void) {
  pinMode(21, OUTPUT);  // LED is an output
  pixels.begin();

  pixels.setPixelColor(LED_STATUS, pixels.Color(0, 0, 64));
  for(uint8_t i=1; i<10; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    isPixelFlashing[i] = false;
  }
  pixels.show();
}

void LedStatusHeartbeat(void) {
  static uint8_t hbCounter = 0;

  if(hbCounter == 0 || hbCounter == 2) {
    pixels.setPixelColor(LED_STATUS, pixels.Color(0, 64, 0));
  } else {
    pixels.setPixelColor(LED_STATUS, pixels.Color(0, 0, 0));
  }
  
  pixels.show();
  hbCounter++;
  if(hbCounter == 25) hbCounter = 0;
}

void LedSetColour(uint8_t ledNbr, uint8_t red, uint8_t green, uint8_t blue, bool flashing) {
  if(!flashing) {
    LedSetColour(ledNbr, red, green, blue);
    return;
  }

  // if we get here, the pixel is flashing, so save the colour and mark it as flashing
  isPixelFlashing[ledNbr] = true;
  flashingPixelColours[ledNbr] = pixels.Color(red, green, blue);
  pixels.setPixelColor(ledNbr, pixels.Color(red, green, blue));
  pixels.show();
}

void LedSetColour(uint8_t ledNbr, uint8_t red, uint8_t green, uint8_t blue) {
  isPixelFlashing[ledNbr] = false;
  pixels.setPixelColor(ledNbr, pixels.Color(red, green, blue));
  pixels.show();
}

void LedHandleFlashing(void) {
  static uint8_t flashCycle = 0;

  for(uint8_t i=1; i<10; i++) {
    if(!isPixelFlashing[i]) continue;
    if(flashCycle == 0)
      pixels.setPixelColor(i, 0);
    if(flashCycle == 7)
      pixels.setPixelColor(i, flashingPixelColours[i]);
  }

  flashCycle++;
  if(flashCycle == 14) flashCycle = 0;
}