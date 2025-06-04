#if PUMP_CONTROL || nanoatmega328_PUMP_CONTROL || ESP32_PUMP_CONTROL
#include "main.h"

bool start_active = false;
uint32_t start_active_time;
uint32_t lamp_millis;

void pump_control_setup() {
    Serial.println("Pump control boot...");
    pinMode(PinConfig.i_start,INPUT_PULLUP);
    pinMode(PinConfig.i_start_with_preset_time,INPUT_PULLUP);
    pinMode(PinConfig.i_stop,INPUT_PULLUP);
    //pinMode(PinConfig.i_emg_stop, INPUT_PULLUP);
    pinMode(PinConfig.o_lamp_start, OUTPUT);
    pinMode(PinConfig.o_lamp_start_with_preset_time, OUTPUT);
    pinMode(PinConfig.o_lamp_stop,OUTPUT);
    pinMode(PinConfig.o_relay_on_off,OUTPUT);
}

void pump_control_loop() {

    /* fizikai io-hoz */
    pump_read_inputs();

    if (!start_active) {
        if (pump_control_io.i_start) {
            Serial.println("start parancs érkezett, control start indítás");
            pump_control_io.i_start = false;
            pump_control.start_type = 0;
            pump_control.set_time = 0;
    
            pump_control_start(pump_control.start_type, pump_control.set_time,false);
        }
    }
    else {
        pump_control_io.i_start = false;
    }
    

    if (pump_control_io.i_start_with_preset_time) {
        if (!start_active) {
            Serial.println("tstart parancs érkezett, control start indítás");
            pump_control_io.i_start_with_preset_time = false;
            pump_control.start_type = 1;
            pump_control.set_time = 50000; // 50s

            pump_control_start(pump_control.start_type, pump_control.set_time, false);
        }
        else {
            Serial.println("újabb tstart parancs érkezett, control start indítás");
            pump_control_io.i_start_with_preset_time = false;
            pump_control_start(pump_control.start_type, pump_control.set_time, true);
        }
    }
    
    
    if (pump_control_io.i_stop) {
        Serial.println("stop parancs érkezett, leállítás");
        pump_control_io.i_stop = false;
    
        pump_stop();
    }
        
    if (start_active) {
        Serial.println("aktiválódott a start_active");
        pump_control_start(pump_control.start_type, pump_control.set_time, false);
    }

    /* fizikai io-hoz */
    pump_write_outputs();

}

void pump_control_start(uint8_t start_type, uint32_t preset_time, bool refresh_timer) {
    if (!start_active) {
        start_active = true;
        start_active_time = millis();
        pump_start();
        
    }
    else {
        if (refresh_timer) {
            refresh_timer = false;
            start_active_time = millis();
        }

        if (start_type == 0) { 
            /*Normal start*/
            pump_start();
        }
        else if (start_type == 1) {
            /*timer start*/

            if (millis() - start_active_time > preset_time) {
                pump_stop();
            }
            else {
                if (millis() - lamp_millis >= 300) {
                    lamp_millis = millis();
                    pump_control_io.o_lamp_start_with_preset_time = !pump_control_io.o_lamp_start_with_preset_time;
                    //pump_control_io.o_relay_on_off = !pump_control_io.o_relay_on_off; only to test the output timer

                }
                Serial.print("Időzített futás állapota:");
                Serial.print(millis() - start_active_time);
                Serial.println();
            }
        }
    }
    
}

void pump_start() {
    pump_control_io.o_lamp_stop = false;
    pump_control_io.o_lamp_start = true;
    pump_control_io.o_relay_on_off = true;
    
    Serial.println("Szivattyú elindítva");
}

void pump_stop() {
    pump_control_io.o_lamp_start = false;
    pump_control_io.o_lamp_start_with_preset_time = false;
    pump_control_io.o_relay_on_off = false;
    pump_control_io.o_lamp_stop = true;

    pump_control_io.i_stop = false;

    start_active = false;
    Serial.println("Szivattyú leállítva");
    
}

void pump_error() {
    Serial.println("Szivattyú hiba");
}

void pump_read_inputs() {
    pump_control_io.i_start = !digitalRead(PinConfig.i_start);
    pump_control_io.i_start_with_preset_time = !digitalRead(PinConfig.i_start_with_preset_time);
    pump_control_io.i_stop = !digitalRead(PinConfig.i_stop);
    //pump_control_io.i_emg_stop = !digitalRead(PinConfig.i_emg_stop);
}

void pump_write_outputs() {
    digitalWrite(PinConfig.o_lamp_start,pump_control_io.o_lamp_start);
    digitalWrite(PinConfig.o_lamp_start_with_preset_time,pump_control_io.o_lamp_start_with_preset_time);
    digitalWrite(PinConfig.o_lamp_stop,pump_control_io.o_lamp_stop);
    digitalWrite(PinConfig.o_relay_on_off,pump_control_io.o_relay_on_off);
}

#endif