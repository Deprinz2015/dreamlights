//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_SEGMENTED_COLOR_H
#define MAINCODE_SEGMENTED_COLOR_H

#include "Arduino.h"

struct Segmented_Color {
    uint32_t color;
    uint16_t from;
    uint16_t to;
};

#endif //MAINCODE_SEGMENTED_COLOR_H
