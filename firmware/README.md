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
1. Select the board type "ESP32C6 Dev Module".
1. Ensure "USB CDC On Boot" under the Tools menu is enabled. If this is not enabled, the USB serial console for the controller board will not function.
1. To update via USB:
    1. If a fresh board with no firmware, hold down the BOOT button and connect the board via USB. If the board already has firmware on it and you're just upgrading it, you don't need to hold anything down, just connect it normally.
    1. Set the port in the Tools menu to the port the board is showing up as.
	1. Select Upload to flash the firmware.
    1. Press CTRL-SHIFT-P, type "LittleFS", then select "Upload LittleFS to Pico/ESP8266/ESP32". This will upload the HTML/CSS/images for the web interface.
1. To update over-the-air via the web interface:
    1. Select Sketch->Export Compiled Binary to generate the firmware binaries.
	1. Press CTRL-SHIFT-P, type "LittleFS", then select "Build LittleFS image in sketch directory" to generate the filesystem image.
	1. Access the web interface of the controller, and under the Tools section select Launch Update Tool.
	1. Browse for the firmware image, which is called "Kwartzlab3DPSL.ino.bin" in the "build" directory under the sketch directory, and click "Update Firmware".
	1. Browse for the filesystem image, which is called "mklittlefs.bin" in the sketch directory, and click "Update FileSystem".
