//
// Created by Niklas Koll on 25.12.21.
//

#ifndef MAINCODE_LEDCONTROLLER_H
#define MAINCODE_LEDCONTROLLER_H

CRGB *leds;
Clock clocks[10];
uint8_t clockAmount;
Effect currentEffect;
Color_Preset current_preset = {0, nullptr};
bool turned_off = false;

bool play_effect(const String&);
bool display_solid_color(uint32_t);
bool turn_off();
bool turn_on();
bool load_preset(String);
bool color_segment(uint32_t, uint32_t, uint32_t);
bool save_preset(String);
bool setBrightness(double);
bool display_preset();
bool updatePreset();

uint32_t getHexFromCRGB(CRGB);
void runPattern();
int find_effect_by_id(Effect *, const String &);
void set_new_effect(Effect);
int find_preset_by_id(Color_Preset_Key *, const String &);
bool set_new_preset(Color_Preset_Key);
bool display_segment(uint32_t, uint32_t, uint32_t);

bool play_effect(const String& id) {
    Effect fx;

    if (find_effect_by_id(&fx, id) == -1) {
        return false;
    }

    set_new_effect(fx);

    return true;
}

// TODO: change to use hue and saturation
bool display_solid_color(uint32_t value) {
    config.color = value;
    config.is_solid_color = true;
    config.is_segmented_color = false;

    fill_solid(leds, config.num_leds, config.color);
    FastLED.show();

    saveMainConfig(config);

    return true;
}

bool turn_off() {
    close_lua();

    fill_solid(leds, config.num_leds, CRGB::Black);
    FastLED.show();
    turned_off = true;

    return true;
}

bool turn_on() {
    if (config.is_solid_color) {
        fill_solid(leds, config.num_leds, config.color);
        FastLED.show();
    } else if (config.is_segmented_color) {
        display_preset();
    } else {
        open_lua(leds, config.num_leds);
        load_lua_script(currentEffect);
    }

    turned_off = false;

    return true;
}

bool load_preset(String id) {
    Color_Preset_Key preset_key{};
    if (find_preset_by_id(&preset_key, id) == -1) {
        return false;
    }

    set_new_preset(preset_key);

    return true;
}

bool color_segment(uint32_t color, uint32_t from, uint32_t to) {
    if (!display_segment(color, from, to)) {
        return false;
    }
    return updatePreset();
}

bool save_preset(String name) {
    String id = String(preset_amount);
    saveColorPreset(name, id, current_preset);

    preset_amount = loadPresetList(presets);
    return true;
}

bool setBrightness(double brightnessPerc) {
    auto brightness = (uint8_t) (config.max_brightness * brightnessPerc);

    config.current_brightness = brightness;
    FastLED.setBrightness(brightness);
    FastLED.show();

    saveMainConfig(config);

    return true;
}

void set_new_effect(Effect fx) {
    config.is_solid_color = false;
    config.is_segmented_color = false;

    currentEffect = std::move(fx);

    clockAmount = loadRefreshRates(currentEffect, clocks);

    config.last_id = currentEffect.id;
    saveMainConfig(config);

    load_lua_script(currentEffect);
}

bool set_new_preset(Color_Preset_Key preset_key) {
    if (!loadPreset(preset_key, &current_preset)) {
        return false;
    }

    config.is_solid_color = false;
    config.is_segmented_color = true;

    config.segmented_preset_id = preset_key.id;
    fill_solid(leds, config.num_leds, CRGB::Black);
    display_preset();

    return true;
}

int find_effect_by_id(Effect *fx, const String &id) {
    for (int i = 0; i < effect_amount; i++) {
        if (strcmp(effects[i].id.c_str(), id.c_str()) == 0) {
            *fx = effects[i];
            return i;
        }
    }
    return -1;
}

int find_preset_by_id(Color_Preset_Key *preset, const String &id) {
    for (int i = 0; i < preset_amount; i++) {
        if (strcmp(presets[i].id.c_str(), id.c_str()) == 0) {
            preset->id = presets[i].id;
            preset->name = presets[i].name;
            return i;
        }
    }
    return -1;
}

bool display_segment(uint32_t color, uint32_t start_index, uint32_t end_index) {
    if (start_index < 0 || start_index >= config.num_leds || start_index > end_index) {
        return false;
    }

    if (end_index > config.num_leds) {
        return false;
    }

    for (uint16_t i = start_index; i <= end_index; i++) {
        leds[i] = color;
    }

    config.is_solid_color = false;
    config.is_segmented_color = true;

    FastLED.show();

    return true;
}

bool updatePreset() {
    Color_Preset new_preset = Color_Preset();

    new_preset.segment_amount = current_preset.segment_amount + 2;
    new_preset.segments = new Segmented_Color[new_preset.segment_amount];

    uint32_t last_color = 0;
    uint16_t segment_index = 0;
    for (int i = 0; i < config.num_leds; i++) {
        uint32_t current_color = getHexFromCRGB(leds[i]);

        if (current_color != last_color) {
            if (segment_index > 0 && last_color != 0) {
                new_preset.segments[segment_index - 1].to = i - 1; // segment_index now on last segment
            }

            last_color = current_color;

            if (current_color == 0) { // if led is off, segment should be skipped
                continue;
            }

            new_preset.segments[segment_index].from = i;
            new_preset.segments[segment_index].color = current_color;

            segment_index++;
        }
    }

    if (last_color != 0) {
        new_preset.segments[segment_index - 1].to = config.num_leds - 1;
    }

    if (segment_index < new_preset.segment_amount) {
        uint16_t segment_amount = segment_index;
        auto *final_segments = new Segmented_Color[segment_amount];
        for (int i = 0; i < segment_amount; i++) {
            final_segments[i] = new_preset.segments[i];
        }

        new_preset.segments = final_segments;
        new_preset.segment_amount = segment_amount;

    }

    current_preset = new_preset;

    return true;
}

uint32_t getHexFromCRGB(CRGB color) {
    return (color.r << 16) | (color.g << 8) | color.b;
}

bool display_preset() {
    for (int i = 0; i < current_preset.segment_amount; i++) {
        if(!display_segment(current_preset.segments[i].color, current_preset.segments[i].from,
                        current_preset.segments[i].to)){
            return false;
        }
    }

    return true;
}

void runPattern() {
    for (int i = 0; i < clockAmount; i++) {
        if (clocks[i].timer.ready()) {
            runLuaPattern(clocks[i].name.c_str());
        }
    }
}

#endif //MAINCODE_LEDCONTROLLER_H
