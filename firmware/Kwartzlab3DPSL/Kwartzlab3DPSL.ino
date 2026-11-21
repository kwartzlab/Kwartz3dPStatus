#include <SimpleCLI.h>
#include <LittleFS.h>
#include <ESPAsyncWebServer.h>
#include "Printer.h"

uint8_t heartbeatCycles;

void setup() {
  delay(1000);
  heartbeatCycles = 0;

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
