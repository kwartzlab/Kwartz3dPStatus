#include <PicoMQTT.h>

PicoMQTT::Server mqtt;

void MqttInit(void) {
  mqtt.subscribe("printer/+/print_status", [](const char *topic, const char *payload) {
    uint8_t printerNbr = atoi(strchr(topic, '/')+1);
    PrinterUpdateState(printerNbr, payload);
  });
  mqtt.subscribe("printer/+/claim_status", [](const char *topic, const char *payload) {
    uint8_t printerNbr = atoi(strchr(topic, '/')+1);
    if(strcmp(payload, "claimed") == 0) {
      PrinterUpdateClaimed(printerNbr, true);
    } else if(strcmp(payload, "unclaimed") == 0) {
      PrinterUpdateClaimed(printerNbr, false);
    }    
  });

  mqtt.begin();
}

void MqttLoop(void) {
  mqtt.loop();
}
