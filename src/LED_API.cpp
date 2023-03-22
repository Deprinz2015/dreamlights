//
// Created by Niklas Koll on 16.07.22.
//

#include "LED_API.h"

#include "Globals.h"
#include "Constants.h"
#include "Config.h"

// Utils

uint32_t getHexFromCRGB(CRGB color) {
    return (color.r << 16) | (color.g << 8) | color.b;
}

//----------------------------------------------------------------------------------------------------------------------

LED_API::LED_API() {
    numEffect = Config::loadEffectList(allEffects);
    numPreset = Config::loadPresetList(allPresets);

    leds = new CRGB[config.num_leds];
#ifdef SK9822_TYPE
    FastLED.addLeds<SK9822, LED_PIN, LED_CLK_PIN, BGR, DATA_RATE_MHZ(12)>(leds, config.num_leds);
#endif
#ifdef WS2812B_TYPE
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, config.num_leds);
#endif

    FastLED.setBrightness(config.current_brightness);
    FastLED.clear(true);

    find_effect_by_id(currentEffect, config.last_id);
    numClocks = Config::loadRefreshRates(currentEffect, clocks);

    Color_Preset_Key presetKey{};
    if (find_preset_by_id(&presetKey, config.segmented_preset_id)) {
        set_new_preset(presetKey);
    }

    set_power(POWER_ON);
}

void LED_API::loadPreset(const String& id) {
    Color_Preset_Key presetKey{};
    if (find_preset_by_id(&presetKey, id)) {
        set_new_preset(presetKey);
    }
}

void LED_API::play_effect(const String &id) {
    Effect fx;
    if (find_effect_by_id(fx, id)) {
        set_new_effect(fx);
    }
}

/**
 * Sets the entire Strip to the given color
 * and executes FastLED.show() to refresh the physical Strip
 * Also saves the new Color Value to the Config File
 *
 * @param value color value as a 32-bit integer (RGB-Hex value)
 */
void LED_API::display_solid_color(uint32_t value) {
    config.color = value;
    setIsSolidColor();

    fill_solid(leds, config.num_leds, config.color);
    FastLED.show();

    Config::saveMainConfig(config);
}

void LED_API::set_power(bool power) {
    if (turnedOn != power) {
        turnedOn = power;
        if (turnedOn) {
            turn_on();
        } else {
            turn_off();
        }
    }
}

void LED_API::color_segment(uint16_t from, uint16_t to, uint32_t color) {
    display_segment(from, to, color);
    updatePreset();
}

void LED_API::save_preset(String &newName) {
    String id = String(numPreset);
    Config::saveColorPreset(newName, id, currentPreset);

    numPreset = Config::loadPresetList(allPresets);
}

/**
 * Saves a new Effect with the given parameters
 *
 * @param newName Name of the effect
 * @param script Lua Script describing the effect
 * @param clock_amount amount of timers
 * @param clock_names names of timers
 * @param clock_times times of timers
 * @returns id ID of the newly created Effect
 */
String LED_API::save_new_effect(String &newName, String &script, uint8_t clock_amount, String* clock_names, uint32_t* clock_times) {
    String id = String(numEffect + 1);
    Config::saveNewEffect(newName, id, script, clock_amount, clock_names, clock_times);

    numEffect = Config::loadEffectList(allEffects);
    return id;
}

/**
 * Saves new Effect Parameters for an already existing project
 *
 * @param id ID of the Effect to be saved
 * @param script Lua Script describing the effect
 * @param clock_amount amount of timers
 * @param clock_names names of timers
 * @param clock_times times of timers
 */
void LED_API::save_effect(String &id, String &script, uint8_t clock_amount, String* clock_names, uint32_t* clock_times) {
    Config::saveEffect(id, script, clock_amount, clock_names, clock_times);

    numEffect = Config::loadEffectList(allEffects);
}

/**
 * Sets brightness of LED-Strip
 * and executes FastLED.show() to refresh Strip
 * Saves new brightness to MainConfig
 *
 * @param brightnessPerc New Brightness level in Percent
 */
void LED_API::setBrightness(uint8_t brightnessPerc) {
    if (brightnessPerc < 0 || brightnessPerc > 100) {
        return;
    }

    uint8_t brightness = config.max_brightness * ((double) (brightnessPerc) / 100);
    config.current_brightness = brightness;
    FastLED.setBrightness(brightness);
    FastLED.show();

    Config::saveMainConfig(config);
}

void LED_API::display_preset() {
    for (uint16_t i = 0; i < currentPreset.segment_amount; i++) {
        display_segment(currentPreset.segments[i].from, currentPreset.segments[i].to, currentPreset.segments[i].color);
    }
}

void LED_API::updatePreset() {
    Color_Preset newPreset = Color_Preset();

    newPreset.segment_amount = currentPreset.segment_amount + 2;
    newPreset.segments = new Segmented_Color[newPreset.segment_amount];

    uint32_t last_color = 0;
    uint16_t segment_index = 0;
    for (uint16_t i = 0; i < config.num_leds; i++) {
        uint32_t current_color = getHexFromCRGB(leds[i]);

        if (current_color != last_color) {
            if (segment_index > 0 && last_color != 0) {
                newPreset.segments[segment_index - 1].to = i - 1;
            }

            last_color = current_color;

            if (current_color == 0) {
                continue;
            }

            newPreset.segments[segment_index].from = i;
            newPreset.segments[segment_index].color = current_color;
            segment_index++;
        }
    }

    if (last_color != 0) {
        newPreset.segments[segment_index - 1].to = config.num_leds - 1;
    }

    if (segment_index < newPreset.segment_amount) {
        uint16_t segment_amount = segment_index;
        auto *final_segments = new Segmented_Color[segment_amount];
        for (uint16_t i = 0; i < segment_amount; i++) {
            final_segments[i] = newPreset.segments[i];
        }

        newPreset.segments = final_segments;
        newPreset.segment_amount = segment_amount;
    }

    currentPreset = newPreset;
}

void LED_API::runPattern() {
    for (uint8_t i = 0; i < numClocks; i++) {
        if (clocks[i].timer.ready()) {
            lua.run_lua_pattern(clocks[i].name.c_str());
        }
    }
}

bool LED_API::find_effect_by_id(Effect &effect, const String &effectID) const {
    for (int i = 0; i < numEffect; i++) {
        if (strcmp(allEffects[i].id.c_str(), effectID.c_str()) == 0) {
            effect = allEffects[i];
            return true;
        }
    }
    return false;
}

bool LED_API::find_preset_by_id(Color_Preset_Key *preset, const String &presetID) const {
    for (int i = 0; i < numPreset; i++) {
        if (strcmp(allPresets[i].id.c_str(), presetID.c_str()) == 0) {
            *preset = allPresets[i];
            return true;
        }
    }
    return false;
}

/**
 * Sets the given Effect to be played
 * Saves effect id to Main Config
 *
 * @param effect
 */
void LED_API::set_new_effect(Effect effect) {
    setIsEffect();

    currentEffect = std::move(effect);

    numClocks = Config::loadRefreshRates(currentEffect, clocks);

    config.last_id = currentEffect.id;
    Config::saveMainConfig(config);

    lua.load_lua_script(currentEffect);
}

void LED_API::set_new_preset(const Color_Preset_Key &preset) {
    if (!Config::loadPreset(preset, &currentPreset)) {
        return;
    }

    setIsSegmentedColor();

    config.segmented_preset_id = preset.id;
    fill_solid(leds, config.num_leds, CRGB::Black);
    display_preset();

    Config::saveMainConfig(config);

    display_preset();
}

void LED_API::update_homespan(int characteristic) {
    if(!updateCharacteristics[characteristic]) {
        updateCharacteristics[characteristic] = true;
    }
}

//------------------------------------------------------------------------------
// Private functions

void LED_API::setIsSolidColor() {
    config.is_solid_color = true;
    config.is_segmented_color = false;
}

void LED_API::setIsSegmentedColor() {
    config.is_solid_color = false;
    config.is_segmented_color = true;
}

void LED_API::setIsEffect() {
    config.is_solid_color = false;
    config.is_segmented_color = false;
}

void LED_API::turn_on() {
    if (config.is_solid_color) {
        fill_solid(leds, config.num_leds, config.color);
        FastLED.show();
    } else if (config.is_segmented_color) {
        display_preset();
    } else {
        lua.open_lua(leds, config.num_leds);
        lua.load_lua_script(currentEffect);
    }
}

void LED_API::turn_off() {
    if (!config.is_solid_color && !config.is_segmented_color) {
        lua.close_lua();
    }

    fill_solid(leds, config.num_leds, CRGB::Black);
    FastLED.show();
}

void LED_API::display_segment(uint16_t from, uint16_t to, uint32_t color) {
    if (from > to) {
        uint16_t tmp = from;
        from = to;
        to = tmp;
    }

    if (from < 0 || to >= config.num_leds) {
        return;
    }

    for (uint16_t i = from; i <= to; i++) {
        leds[i] = color;
    }

    setIsSegmentedColor();

    FastLED.show();
}
