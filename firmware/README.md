# Kwartlab 3D Printer Status Light System - Firmware

This is firmware for the hardware that's contained in the same repo.

## AI/LLM Policy

No LLMs/"AI" should be used for this project, human contributions only.

## To reprogram the controller board:

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

## Status LED
- Blue - Booting, trying to connect to WiFi
- Red - Unable to connect to WiFi or get an IP via DHCP, check WiFi system or connect controller via USB to reconfigure WiFi parameters
- Two quick green flahes every few seconds - Normal status, controller is in service
- Purple - OTA firmware update in progress

## Interfaces

- USB Serial - Low-level setup, to configure the WiFi itself and such. Connect to a computer and it'll show up a USB serial (CDC) port, type 'help' for details.
- Web - General configuration, troubleshooting, firmware updates, etc. Listens on port 80.
- MQTT - Receives updates about printer status changes to actually control the lights.

## MQTT Interface
- Runs its own broker, so you can connect to the controller board on tcp/1883 and send messages directly to it
- Send plain text messages to printer/[printer number]/print_status to update the current state of the printer
  - "printing" for when a job starts printing
  - "done" for when a job completes
  - "cancelled" for when a job is cancelled
  - "error" when a job experiences an error and the printer is currently not printing due to it
  - "paused" if a job is manually paused
- Send plain text messages to printer/[printer number]/claim_status to update whether the currently-running job is claimed or not
  - "claimed" to claim a job
  - "unclaimed" to un-claim a job (claim status is also automatically reset when a print is done or cancelled)
