//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_COLOR_PRESET_H
#define MAINCODE_COLOR_PRESET_H

#include "Arduino.h"
#include "Segmented_Color.h"

struct Color_Preset {
    uint16_t segment_amount;
    Segmented_Color *segments;
};

#endif //MAINCODE_COLOR_PRESET_H
