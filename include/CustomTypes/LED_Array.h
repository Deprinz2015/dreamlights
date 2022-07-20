//
// Created by Niklas Koll on 20.07.22.
//

#ifndef MAINCODE_LED_ARRAY_H
#define MAINCODE_LED_ARRAY_H

#include "FastLED.h"

struct LED_Array {
    uint16_t size;
    CRGB *leds;
};

#endif //MAINCODE_LED_ARRAY_H
