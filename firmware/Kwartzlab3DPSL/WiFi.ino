#include <WiFi.h>

void WiFiInit(void) {
  String ssid = StorageGetString("WiFi/SSID");

  if(ssid.isEmpty()) {
    Serial.println("WARN: No WiFi configuration found, entering failsafe mode to allow configuration.");
    SerialFailsafe(); // never returns
  }

  Serial.print("Connecting to ");
  Serial.print(ssid);

  WiFi.setHostname("printerlights");
  WiFi.begin(ssid, StorageGetString("WiFi/Key"));
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(millis() > 15000) {
      // give up on connecting and let the user in to fix whatever settings are wrong
      SerialFailsafe();
    }
  }
  
  Serial.print("\nConnected to network, IP address is ");
  Serial.println(WiFi.localIP());
}