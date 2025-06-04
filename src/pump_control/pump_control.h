#if PUMP_CONTROL || nanoatmega328_PUMP_CONTROL || ESP32_PUMP_CONTROL
#ifndef __PUMP_CONTROL_H
#define __PUMP_CONTROL_H
#include "main.h"

struct PinConfig_struct {
    const uint8_t i_stop;
    const uint8_t i_start_with_preset_time;
    const uint8_t i_start;
    const uint8_t i_emg_stop;
    const uint8_t o_relay_on_off;
    const uint8_t o_lamp_stop;
    const uint8_t o_lamp_start;
    const uint8_t o_lamp_start_with_preset_time;
    
    
};
constexpr PinConfig_struct PinConfig = {2,3,4,5,6,7,8,9};

struct PumpErrors_struct {
    bool check_relay_on_state = false; 
};

void pump_control_setup();
void pump_control_loop();
void pump_read_inputs();
void pump_write_outputs();
void pump_control_start(uint8_t start_type, uint32_t preset_time, bool refresh_time);
void pump_start();
void pump_stop();
void pump_error();

class pump_function_c {
    private: 
        bool active;

        bool relay;
        bool start_lamp;
        bool stop_lamp;
        bool t_start_lamp;
        
    public:
        pump_function_c();

        void command(uint8_t start_type, uint32_t preset_time, bool refresh_time);
        void start();
        void stop();
};

#endif
#endif