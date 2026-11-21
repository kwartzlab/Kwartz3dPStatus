SimpleCLI cli;
bool failsafe_mode;

void SerialCmdWifiSetup(cmd *c) {
  Command cmd(c);

  StoragePutString("WiFi/SSID", cmd.getArg("ssid").getValue());
  StoragePutString("WiFi/Key", cmd.getArg("key").getValue());

  Serial.println("OK, wifi setup updated. Restarting...");
  delay(250);
  ESP.restart();
}

void SerialCmdLs(cmd *c) {
  File root = LittleFS.open("/");
  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void SerialCmdReboot(cmd *c) {
  Serial.println("OK, rebooting...");
  ESP.restart();
}

void SerialCmdReset(cmd *c) {
  StorageClearAll();
  Serial.println("OK, configuration reset. Rebooting...");
  ESP.restart();
}

void SerialCmdHelp(cmd *c) {
  Serial.println("wifi-setup -ssid [ssid] -key [key] - Change WiFi configuration");
  Serial.println("ls - View list of files in LittleFS storage");
  Serial.println("reboot - Reboot the controller");
  Serial.println("reset - Reset all configuration to 'factory' defaults");
  Serial.println("help - This help message");
}

void SerialInit(void) {
  failsafe_mode = false;

  Serial.begin(115200);
  Serial.println("Kwartzlab 3D Printer Status Lights booting...");

  Command cmdWifiSetup = cli.addCommand("wifi-setup", SerialCmdWifiSetup);
  cmdWifiSetup.addArg("ssid");
  cmdWifiSetup.addArg("key");

  Command cmdLs = cli.addCommand("ls", SerialCmdLs);
  Command cmdReboot = cli.addCommand("reboot", SerialCmdReboot);
  Command cmdReset = cli.addCommand("reset", SerialCmdReset);

  Command cmdQuestion = cli.addCommand("?", SerialCmdHelp);
  Command cmdHelp = cli.addCommand("help", SerialCmdHelp);
}

void SerialLoop(void) {
  uint8_t buf;
  
  if(cli.errored()) {
    CommandError e = cli.getError();
    Serial.println(e.toString());
  }

  if(Serial.available()) {
    String input = Serial.readStringUntil('\n');
    Serial.println(input);
    if(input.length() == 0 or input == "\n") {
      SerialPrompt();
    } else {
      cli.parse(input);
      SerialPrompt();
    }
  }
}

void SerialPrompt(void) {
  if(failsafe_mode) {
    Serial.print("3dpsl-failsafe> ");
  } else {
    Serial.print("3dpsl> ");
  }
}

// we couldn't connect to wifi, so enter a failsafe mode where the user can reconfigure things
void SerialFailsafe(void) {
  Serial.println("Couldn't connect to Wifi, entering failsafe mode.");
  LedSetColour(0, 255, 0, 0);
  SerialPrompt();
  
  while(true) {
    SerialLoop();
    delay(50);
  }
}