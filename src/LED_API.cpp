//
// Created by Niklas Koll on 16.07.22.
//

#include "LED_API.h"
#include "Globals.h"
#include "Constants.h"

void LED_API::init() {
    leds = new CRGB[config.num_leds];
#ifdef SK9822_TYPE
    FastLED.addLeds<SK9822, LED_PIN, LED_CLK_PIN, BGR, DATA_RATE_MHZ(12)>(leds, config.num_leds);
#endif
#ifdef WS2812B_TYPE
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, config.num_leds);
#endif

    FastLED.setBrightness(config.current_brightness);
    FastLED.clear(true);

    set_power(POWER_ON);
}

void LED_API::play_effect(const String &effect) {
    // TODO
}

void LED_API::display_solid_color(uint32_t color) {
    // TODO
}

void LED_API::set_power(bool power) {
    // TODO

    turnedOn = power;
}

void LED_API::color_segment(uint32_t from, uint32_t to, uint32_t color) {
    // TODO
}

void LED_API::save_preset(String name) {
    // TODO
}

void LED_API::setBrightness(float brightness) {
    // TODO
}

void LED_API::display_preset() {
    // TODO
}

void LED_API::updatePreset() {
    // TODO
}

void LED_API::runPattern() {
    // TODO
}

bool LED_API::find_effect_by_id(Effect *effect, const String &name) {
    // TODO
    return false;
}

bool LED_API::find_preset_by_id(Color_Preset_Key *preset, const String &name) {
    // TODO
    return false;
}

void LED_API::set_new_effect(Effect effect) {
    // TODO
}

void LED_API::set_new_preset(Color_Preset_Key preset) {
    // TODO
}
