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
    uint8_t loadRefreshRates(const Effect &fx, Clock *clocks);
    int loadEffectList(Effect *&effects);
    int loadPresetList(Color_Preset_Key *&presets);
    bool loadPreset(const Color_Preset_Key& key, Color_Preset *out);
    void loadMainConfig(MainConfig *config);
    String read_lua_script(const Effect &fx);

    void saveMainConfig(const MainConfig &config);
    void saveColorPreset(String name, String id, Color_Preset preset);
};

#endif //MAINCODE_CONFIG_H
