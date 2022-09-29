//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_CONFIG_H
#define MAINCODE_CONFIG_H

#include "Arduino.h"
#include "CustomTypes/Effect.h"
#include "CustomTypes/Clock.h"
#include "CustomTypes/Color_Preset_Key.h"
#include "CustomTypes/Color_Preset.h"
#include "CustomTypes/Main_Config.h"

struct Config {
    static uint8_t loadRefreshRates(const Effect &fx, Clock *clocks);
    static int loadEffectList(Effect *&effects);
    static int loadPresetList(Color_Preset_Key *&presets);
    static bool loadPreset(const Color_Preset_Key& key, Color_Preset *out);
    static void loadMainConfig(MainConfig *config);
    static String read_lua_script(const Effect &fx);

    static void saveMainConfig(const MainConfig &config);
    static void saveColorPreset(String name, String id, Color_Preset preset);
    static void saveNewEffect(String &effect_name, String &id, String &script, uint8_t clock_amount, String* clock_names, uint32_t* clock_times);
    static void saveEffect(String &id, String &script, uint8_t clock_amount, String* clock_names, uint32_t* clock_times);
};

#endif //MAINCODE_CONFIG_H
