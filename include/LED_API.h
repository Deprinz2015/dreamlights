//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_LED_API_H
#define MAINCODE_LED_API_H

#include "FastLED.h"
#include "CustomTypes/Clock.h"
#include "CustomTypes/Effect.h"
#include "CustomTypes/Color_Preset.h"
#include "CustomTypes/Color_Preset_Key.h"
#include "Globals.h"
#include "Constants.h"

struct LED_API {
    CRGB* leds;
    uint16_t numLeds;
    Clock clock[10];
    uint8_t numClocks;
    Effect effect;
    Color_Preset currentPreset = {0, nullptr};
    bool turnedOn = true;

    void init();

    void play_effect(const String& effect);
    void display_solid_color(uint32_t color);
    void set_power(bool power);
    void color_segment(uint32_t from, uint32_t to, uint32_t color);
    void save_preset(String name);
    void setBrightness(double brightness);
    void display_preset();
    void updatePreset();

    void runPattern();

    bool find_effect_by_id(Effect *effect, const String &name);
    bool find_preset_by_id(Color_Preset_Key *preset, const String &name);

    void set_new_effect(Effect effect);
    void set_new_preset(Color_Preset_Key preset);
};

#endif //MAINCODE_LED_API_H
