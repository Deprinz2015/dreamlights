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
#include "Constants.h"

struct LED_API {
public:
    LED_API();

    CRGB* leds;
    Clock clocks[10];
    uint8_t numClocks;
    Effect currentEffect;
    int numEffect;
    Effect *allEffects;
    Color_Preset currentPreset = {0, nullptr};
    Color_Preset_Key *allPresets;
    int numPreset;
    bool turnedOn = POWER_OFF;

    // TODO When adding new Characteristic to Sync with Homespan, add it to this array
    bool updateCharacteristics[4] = {
            false, // Power
            false, // Color
            false, // Brightness
            false  // Effect
    };

    void display_solid_color(uint32_t color);
    void set_power(bool power);
    void setBrightness(uint8_t brightness);

    void play_effect(const String& id);
    String save_new_effect(String &name, String &script, uint8_t clock_amount, String* clock_names, uint32_t* clock_times);
    void save_effect(String &id, String &script, uint8_t clock_amount, String* clock_names, uint32_t* clock_times);
    void set_new_effect(Effect effect);


    void loadPreset(const String& id);
    void color_segment(uint16_t from, uint16_t to, uint32_t color);
    void save_preset(String &name);
    void set_new_preset(const Color_Preset_Key& preset);

    void runPattern();

    bool find_effect_by_id(Effect &effect, const String &effectName) const;
    bool find_preset_by_id(Color_Preset_Key *preset, const String &presetName) const;

    void update_homespan(int characteristic);

private:
    void turn_off();
    void turn_on();
    void setIsSolidColor();
    void setIsSegmentedColor();
    void setIsEffect();
    void display_segment(uint16_t from, uint16_t to, uint32_t color);
    void display_preset();
    void updatePreset();
};

#endif //MAINCODE_LED_API_H
