//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_MAIN_CONFIG_H
#define MAINCODE_MAIN_CONFIG_H

#include "Arduino.h"

struct MainConfig {
    uint16_t num_leds;
    uint8_t max_brightness;
    uint8_t current_brightness;
    String last_id;
    bool is_solid_color;
    bool is_segmented_color;
    uint32_t color;
    String segmented_preset_id;
};

#endif //MAINCODE_MAIN_CONFIG_H
