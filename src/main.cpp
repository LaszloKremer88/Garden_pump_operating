#include <Arduino.h>
#include "main.h"

#if PUMP_CONTROL || nanoatmega328_PUMP_CONTROL || ESP32_PUMP_CONTROL
  pump_control_struct pump_control;
  pump_control_io_struct pump_control_io;
#endif

void setup() {
  void pump_control_setup();
}

void loop() {
  void pump_control_loop();
}

