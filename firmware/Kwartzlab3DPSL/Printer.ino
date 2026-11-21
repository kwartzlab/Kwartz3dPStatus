struct PrinterStatus Printers[10];

void PrinterInit(void) {
  uint8_t i;

  for(i=1; i<10; i++) {
    Printers[i].current_state = STATE_IDLE;
    Printers[i].claimed = false;

    String mapKey = String("Printer/Map/") + String(i);
    if(StorageDoesKeyExist(mapKey)) {
      Printers[i].led = StorageGetUInt8(mapKey);
    } else {
      Printers[i].led = PRINTER_NOT_PRESENT;
    }
  }
}

void PrinterUpdateState(uint8_t printerNbr, const char *newState) {
  if(strcmp(newState, "printing") == 0) {
    PrinterUpdateState(printerNbr, STATE_PRINTING);
  } else if(strcmp(newState, "cancelled") == 0 or strcmp(newState, "done") == 0) {
    PrinterUpdateState(printerNbr, STATE_IDLE);
  } else if(strcmp(newState, "error") == 0) {
    PrinterUpdateState(printerNbr, STATE_ERROR);
  } else if(strcmp(newState, "paused") == 0) {
    PrinterUpdateState(printerNbr, STATE_PAUSED);
  } else {
    Serial.print("WARN: Invalid printer status received: ");
    Serial.println(newState);
  }
}

void PrinterUpdateState(uint8_t printerNbr, PrintState state) {
  Printers[printerNbr].current_state = state;
  if(Printers[printerNbr].current_state == STATE_IDLE) Printers[printerNbr].claimed = false;
  Serial.printf("Setting printer %d state to %d\n", printerNbr, state);

  LedSetPrinterStatus(PrinterGetMapping(printerNbr), &Printers[printerNbr]);
}

PrintState PrinterGetState(uint8_t printerNbr) {
  return Printers[printerNbr].current_state;
}

void PrinterUpdateClaimed(uint8_t printerNbr, bool claimed) {
  Printers[printerNbr].claimed = claimed;

  LedSetPrinterStatus(PrinterGetMapping(printerNbr), &Printers[printerNbr]);
}

bool PrinterGetClaimed(uint8_t printerNbr) {
  return Printers[printerNbr].claimed;
}

void PrinterSetMapping(uint8_t printerNbr, uint8_t ledNbr) {
  Printers[printerNbr].led = ledNbr;
  String mapKey = String("Printer/Map/") + String(printerNbr);
  if(ledNbr == PRINTER_NOT_PRESENT) {
    StorageDeleteKey(mapKey);
  } else {
    StoragePutUInt8(mapKey, ledNbr);
  }
}

uint8_t PrinterGetMapping(uint8_t printerNbr) {
  return Printers[printerNbr].led;
}