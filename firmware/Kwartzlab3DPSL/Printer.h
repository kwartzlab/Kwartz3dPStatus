#define PRINTER_NOT_PRESENT 0

enum PrintState {
  STATE_IDLE,
  STATE_PRINTING,
  STATE_PAUSED,
  STATE_ERROR
};

struct PrinterStatus {
  uint8_t     led;
  PrintState  current_state;
  bool        claimed;
};
