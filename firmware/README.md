# Kwartlab 3D Printer Status Light System - Firmware

This is firmware for the hardware that's contained in the same repo. To reprogram the controller board:

1. Ensure you have the following:
    - Arduino IDE 2.3 or higher
	- The Arduino ESP32 core ("esp32 by Espressif Systems") version 3.3 or higher
    - The arduino-littlefs-upload plugin (https://github.com/earlephilhower/arduino-littlefs-upload)
    - The following Arduino libraries:
      - PicoMQTT
	  - Adafruit NeoPixel
      - SerialCLI
1. Open the Kwartzlab3DPSL.ino file in the Arduino IDE.
1. If a fresh board with no firmware, hold down the BOOT button and connect the board via USB. If the board already has firmware on it and you're just upgrading it, you don't need to hold anything down.
1. Select the board "ESP32C6 Dev Module" and set the port to the port the board is showing up as.
1. Select Upload to flash the firmware.
1. Press CTRL-SHIFT-P and type "Upload", then select "Upload LittleFS to Pico/ESP8266/ESP32". This will upload the HTML/CSS/images for the web interface.
