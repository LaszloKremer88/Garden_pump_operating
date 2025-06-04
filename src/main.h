#ifndef __MAIN_H
#define __MAIN_H

#include "arduino.h"

#if PUMP_CONTROL || nanoatmega328_PUMP_CONTROL || ESP32_PUMP_CONTROL
#include "pump_control/pump_control.h"

struct pump_control_struct {
    uint8_t start_type = 0;
    uint32_t set_time;
};

struct pump_control_io_struct {
    bool i_start = false;
    bool i_start_with_preset_time = false;
    bool i_stop = false;
    bool i_emg_stop = false;
    bool o_lamp_start = false;
    bool o_lamp_start_with_preset_time = false;
    bool o_lamp_stop = false;
    bool o_relay_on_off = false;
};

extern pump_control_struct pump_control;
extern pump_control_io_struct pump_control_io;

#endif
#endif
