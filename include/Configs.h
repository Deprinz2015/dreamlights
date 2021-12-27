#ifndef MAINCODE_CONFIGS_H
#define MAINCODE_CONFIGS_H

#include <utility>

#include "ArduinoJson.h"
#include "ConfigDefaults.h"
#include "Clock.h"
#include "StreamUtils.h"

const char *main_config_file_name = "/config.cnf";
const char *effect_list_file_name = "/fxlist.cnf";
const char *preset_list_file_name = "/presets.cnf";
const char *preset_file_template = "/Presets/preset{id}.cnf";
const char *effect_config_template = "/Configs/fx{id}.cnf";
const char *effect_file_template = "/Effects/fx{id}.lua";

struct Effect {
    String id;
    String name;
};

struct Segmented_Color {
    uint32_t color;
    uint16_t from;
    uint16_t to;
};

struct Color_Preset {
    uint16_t segment_amount;
    Segmented_Color *segments;
};

struct Color_Preset_Key {
    String id;
    String name;
};

struct MainConfig {
    int num_leds;
    uint8_t max_brightness;
    uint8_t current_brightness;
    String last_id;
    bool is_solid_color;
    bool is_segmented_color;
    uint32_t color;
    String segmented_preset_id;
};

uint8_t loadRefreshRates(const Effect &fx, Clock *clocks) {
    String filename = effect_config_template;
    filename.replace("{id}", fx.id);

    fs::File file = SD.open(filename, FILE_READ);
    if (file) {
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, file);
        uint8_t size = 0;
        if (!error) {
            JsonObject root = doc.as<JsonObject>();
            size = root.size();
            uint8_t index = 0;
            for (JsonPair element : root) {
                clocks[index].name = element.key().c_str();
                clocks[index].timer.setPeriod(element.value().as<uint32_t>());

                index++;
            }
        }
        file.close();
        return size;
    }

    return 0;
}

int loadEffectList(Effect *&effects) {
    fs::File file = SD.open(effect_list_file_name, FILE_READ);

    if (file) {
        DynamicJsonDocument doc(6500);
        //ReadLoggingStream loggingStream(file, Serial);
        //DeserializationError error = deserializeJson(doc, loggingStream);
        DeserializationError error = deserializeJson(doc, file);

        if (!error) {
            JsonObject root = doc.as<JsonObject>();
            effects = new Effect[root.size()];
            int index = 0;
            for (JsonPair pair : root) {
                effects[index].name = pair.value().as<String>();
                effects[index].id = pair.key().c_str();

                index++;
            }
            file.close();
            return index;
        } else {
            Serial.println(error.c_str());
        }
    }

    return -1;
}

int loadPresetList(Color_Preset_Key *&presets) {
    fs::File file = SD.open(preset_list_file_name, FILE_READ);

    if (file) {
        DynamicJsonDocument doc(6500);
        //ReadLoggingStream loggingStream(file, Serial);
        //DeserializationError error = deserializeJson(doc, loggingStream);
        DeserializationError error = deserializeJson(doc, file);

        if (!error) {
            JsonObject root = doc.as<JsonObject>();
            presets = new Color_Preset_Key[root.size()];
            int index = 0;
            for (JsonPair item : root) {
                presets[index].name = item.value().as<String>();
                presets[index].id = item.key().c_str();

                index++;
            }

            file.close();
            return index;
        }
    }

    return -1;
}

bool loadPreset(Color_Preset_Key key, Color_Preset *out) {
    String filename = preset_file_template;
    filename.replace("{id}", key.id);

    fs::File file = SD.open(filename);

    if (file) {
        DynamicJsonDocument doc(25000);
        DeserializationError error = deserializeJson(doc, file);

        if (!error) {
            JsonArray root = doc.as<JsonArray>();
            out->segment_amount = root.size();
            out->segments = new Segmented_Color[root.size()];

            int index = 0;
            for (JsonObject item : root) {
                out->segments[index].from = item["from"];
                out->segments[index].to = item["to"];
                out->segments[index].color = item["color"];
                index++;
            }

            file.close();
            return true;
        }
    } else {
        Serial.println("Preset File could not be opened");
        return false;
    }
}

void saveColorPreset(String name, String id, Color_Preset preset) {
    String filename = preset_file_template;
    filename.replace("{id}", id);
    fs::File presetFile = SD.open(filename, FILE_WRITE);

    if (presetFile) {
        DynamicJsonDocument doc(25000);

        for (int i = 0; i < preset.segment_amount; i++) {
            JsonObject segment = doc.createNestedObject();
            segment["color"] = preset.segments[i].color;
            segment["from"] = preset.segments[i].from;
            segment["to"] = preset.segments[i].to;
        }

        if (!serializeJson(doc, presetFile)) {
            Serial.println("Could not save Preset File file");
            presetFile.close();
            return;
        }

        presetFile.close();
    } else {
        Serial.println("Preset File could not be opened");
        return;
    }

    fs::File presetListFile = SD.open(preset_list_file_name, FILE_READ);
    Color_Preset_Key *presets;
    int preset_amount;

    if (presetListFile) {
        DynamicJsonDocument doc(6500);
        DeserializationError error = deserializeJson(doc, presetListFile);

        if (!error) {
            JsonObject root = doc.as<JsonObject>();
            presets = new Color_Preset_Key[root.size() + 1];
            int index = 0;
            for (JsonPair item : root) {
                presets[index].name = item.value().as<String>();
                presets[index].id = item.key().c_str();

                index++;
            }

            presets[index].name = name.c_str();
            presets[index].id = id.c_str();

            preset_amount = index + 1;

            presetListFile.close();
        } else {
            Serial.println("Error while deserializing Color Preset List");
            return;
        }
    } else {
        Serial.println("Preset List could not be opened");
        return;
    }

    presetListFile = SD.open(preset_list_file_name, FILE_WRITE);

    if (presetListFile) {
        DynamicJsonDocument doc(6500);

        for (int i = 0; i < preset_amount; i++) {
            doc[presets[i].id] = presets[i].name;
        }

        if (!serializeJson(doc, presetListFile)) {
            Serial.println("Could not save Preset File file");
        }

        presetListFile.close();
    } else {
        Serial.println("Preset List could not be opened");
        return;
    }

}

void loadMainConfig(MainConfig *config) {
    fs::File file = SD.open(main_config_file_name, FILE_READ);
    StaticJsonDocument<250> doc;

    if (file) {

        DeserializationError error = deserializeJson(doc, file);
        if (!error) {
            config->num_leds = doc["num_leds"] | MAINCONFIG_NUM_LEDS;
            config->max_brightness = doc["max_brightness"] | MAINCONFIG_MAX_BRIGHTNESS;
            config->current_brightness = doc["current_brightness"] | MAINCONFIG_CURRENT_BRIGHTNESS;
            config->last_id = doc["last_id"] | MAINCONFIG_LAST_ID;
            config->is_solid_color = doc["is_solid_color"] | MAINCONFIG_IS_SOLID_COLOR;
            config->is_segmented_color = doc["is_segmented_color"] | MAINCONFIG_IS_SEGMENTED_COLOR;
            config->color = doc["color"] | MAINCONFIG_COLOR;
            config->segmented_preset_id = doc["segmented_preset_id"] | MAINCONFIG_SEGMENTED_PRESET_ID;

            file.close();
            return;
        }

        Serial.println("Error while reading json structure of Config file, using default Values");
    }
    Serial.println("Config File could not be opened, using default values");

    config->num_leds = MAINCONFIG_NUM_LEDS;
    config->max_brightness = MAINCONFIG_MAX_BRIGHTNESS;
    config->current_brightness = MAINCONFIG_CURRENT_BRIGHTNESS;
    config->last_id = MAINCONFIG_LAST_ID;
    config->is_solid_color = MAINCONFIG_IS_SOLID_COLOR;
    config->color = MAINCONFIG_COLOR;
    config->is_segmented_color = MAINCONFIG_IS_SEGMENTED_COLOR;
    config->segmented_preset_id = MAINCONFIG_SEGMENTED_PRESET_ID;
}

void saveMainConfig(const MainConfig &config) {
    SD.remove(main_config_file_name);
    fs::File file = SD.open(main_config_file_name, FILE_WRITE);
    StaticJsonDocument<250> doc;

    if (file) {
        doc["num_leds"] = config.num_leds;
        doc["max_brightness"] = config.max_brightness;
        doc["current_brightness"] = config.current_brightness;
        doc["last_id"] = config.last_id;
        doc["is_solid_color"] = config.is_solid_color;
        doc["is_segmented_color"] = config.is_segmented_color;
        doc["color"] = config.color;
        doc["segmented_preset_id"] = config.segmented_preset_id;

        if (!serializeJson(doc, file)) {
            Serial.println("Could not save Config file");
        }

        file.close();
    } else {
        Serial.println("Config File could not be opened");
    }
}

String read_lua_script(const Effect &fx) {
    String filename = effect_file_template;
    filename.replace("{id}", fx.id);
    fs::File scriptFile = SD.open(filename, FILE_READ);

    String scriptString = "";
    while (scriptFile.available()) {
        scriptString += scriptFile.readString();
    }

    scriptFile.close();
    return scriptString;
}

#endif //MAINCODE_CONFIGS_H
