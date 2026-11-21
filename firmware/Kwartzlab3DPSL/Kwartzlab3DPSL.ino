#include <SimpleCLI.h>
#include <LittleFS.h>
#include <ESPAsyncWebServer.h>
#include "Printer.h"

void setup() {
  delay(1000);

  LEDInit();
  SerialInit();
  StorageInit();
  WiFiInit();
  PrinterInit();
  MqttInit();
  WebInit();
}

void loop() {
  LedStatusHeartbeat();
  SerialLoop();
  MqttLoop();
  delay(100);
}
