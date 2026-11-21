#include <Adafruit_NeoPixel.h>

#define LED_STATUS 0

Adafruit_NeoPixel pixels(16, 21, NEO_GRB + NEO_KHZ800);

void LEDInit(void) {
  pinMode(21, OUTPUT);  // LED is an output
  pixels.begin();

  pixels.setPixelColor(LED_STATUS, pixels.Color(0, 0, 64));
  for(uint8_t i=1; i<10; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
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

void LedSetPrinterStatus(uint8_t printerNbr, struct PrinterStatus *status) {
  switch(status->current_state) {
    case STATE_IDLE:
      pixels.setPixelColor(printerNbr, pixels.Color(0, 0, 0));
      break;
    case STATE_PRINTING:
      if(status->claimed) {
        pixels.setPixelColor(printerNbr, pixels.Color(0, 0, 255));
      } else {
        pixels.setPixelColor(printerNbr, pixels.Color(255, 0, 0));      
      }
      break;
    case STATE_PAUSED:
      // TODO: flashing
      pixels.setPixelColor(printerNbr, pixels.Color(255, 255, 0));
      break;
    case STATE_ERROR:
      // TODO: flashing
      pixels.setPixelColor(printerNbr, pixels.Color(255, 255, 0));
      break;
  }
  pixels.show();
}

void LedSetColour(uint8_t ledNbr, uint8_t red, uint8_t green, uint8_t blue) {
  pixels.setPixelColor(ledNbr, pixels.Color(red, green, blue));
  pixels.show();
}