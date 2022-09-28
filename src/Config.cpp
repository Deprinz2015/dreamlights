//
// Created by Niklas Koll on 16.07.22.
//

#include "Config.h"
#include "Filehandler.h"
#include "Constants.h"

uint8_t Config::loadRefreshRates(const Effect &fx, Clock *clocks) {
    String filename = effect_config_template;
    filename.replace("{id}", String(fx.id));

    DynamicJsonDocument doc(1024);
    if (!FileHandler::get_json_dynamic_doc(filename.c_str(), doc)) {
        return 0;
    }

    JsonObject obj = doc.as<JsonObject>();
    uint8_t size = obj.size();
    uint8_t index = 0;
    for (JsonPair element: obj) {
        clocks[index].name = element.key()
                .c_str();
        clocks[index].timer
                .setPeriod(element.value()
                                   .as<uint32_t>());

        index++;
    }
    return size;
}

int Config::loadEffectList(Effect *&effects) {
    DynamicJsonDocument doc(6500);
    if (!FileHandler::get_json_dynamic_doc(effect_list_file_name, doc)) {
        return -1;
    }
    JsonObject obj = doc.as<JsonObject>();
    effects = new Effect[obj.size()];
    int index = 0;
    for (JsonPair pair: obj) {
        effects[index].name = pair.value()
                .as<String>();
        effects[index].id = pair.key()
                .c_str();

        index++;
    }
    return index;
}

int Config::loadPresetList(Color_Preset_Key *&presets) {
    DynamicJsonDocument doc(6500);
    if (!FileHandler::get_json_dynamic_doc(preset_list_file_name, doc)) {
        return -1;
    }
    JsonObject obj = doc.as<JsonObject>();

    presets = new Color_Preset_Key[obj.size()];
    int index = 0;
    for (JsonPair pair: obj) {
        presets[index].name = pair.key()
                .c_str();
        presets[index].id = pair.value()
                .as<String>();

        index++;
    }
    return index;
}

bool Config::loadPreset(const Color_Preset_Key &key, Color_Preset *out) {
    String filename = preset_file_template;
    filename.replace("{id}", key.id);

    DynamicJsonDocument doc(25000);
    if (!FileHandler::get_json_dynamic_doc(filename.c_str(), doc)) {
        return -1;
    }
    JsonArray obj = doc.as<JsonArray>();

    out->segment_amount = obj.size();
    out->segments = new Segmented_Color[out->segment_amount];
    int index = 0;
    for (JsonObject item: obj) {
        out->segments[index].color = item["color"];
        out->segments[index].from = item["from"];
        out->segments[index].to = item["to"];

        index++;
    }

    return true;
}

void Config::loadMainConfig(MainConfig *config) {
    StaticJsonDocument<STATIC_JSON_OBJ_SIZE> doc;
    if (!FileHandler::get_json_static_doc(main_config_file_name, doc)) {
        config->num_leds = MAINCONFIG_NUM_LEDS;
        config->max_brightness = MAINCONFIG_MAX_BRIGHTNESS;
        config->current_brightness = MAINCONFIG_CURRENT_BRIGHTNESS;
        config->last_id = MAINCONFIG_LAST_ID;
        config->is_solid_color = MAINCONFIG_IS_SOLID_COLOR;
        config->is_segmented_color = MAINCONFIG_IS_SEGMENTED_COLOR;
        config->color = MAINCONFIG_COLOR;
        config->segmented_preset_id = MAINCONFIG_SEGMENTED_PRESET_ID;
        Config::saveMainConfig(*config);
        return;
    }
    JsonObject obj = doc.as<JsonObject>();

    config->num_leds = obj["num_leds"] | MAINCONFIG_NUM_LEDS;
    config->max_brightness = obj["max_brightness"] | MAINCONFIG_MAX_BRIGHTNESS;
    config->current_brightness = obj["current_brightness"] | MAINCONFIG_CURRENT_BRIGHTNESS;
    config->last_id = obj["last_id"] | MAINCONFIG_LAST_ID;
    config->is_solid_color = obj["is_solid_color"] | MAINCONFIG_IS_SOLID_COLOR;
    config->is_segmented_color = obj["is_segmented_color"] | MAINCONFIG_IS_SEGMENTED_COLOR;
    config->color = obj["color"] | MAINCONFIG_COLOR;
    config->segmented_preset_id = obj["segmented_preset_id"] | MAINCONFIG_SEGMENTED_PRESET_ID;
}

String Config::read_lua_script(const Effect &fx) {
    String filename = effect_file_template;
    filename.replace("{id}", String(fx.id));

    return FileHandler::read_file(filename.c_str());
}

void Config::saveMainConfig(const MainConfig &config) {
    StaticJsonDocument<STATIC_JSON_OBJ_SIZE> obj;
    obj["num_leds"] = config.num_leds;
    obj["max_brightness"] = config.max_brightness;
    obj["current_brightness"] = config.current_brightness;
    obj["last_id"] = config.last_id;
    obj["is_solid_color"] = config.is_solid_color;
    obj["is_segmented_color"] = config.is_segmented_color;
    obj["color"] = config.color;
    obj["segmented_preset_id"] = config.segmented_preset_id;
    FileHandler::save_json_doc(main_config_file_name, obj);
}

void Config::saveColorPreset(String name, String id, Color_Preset preset) {
    // Save preset file
    String filename = preset_file_template;
    filename.replace("{id}", id);

    DynamicJsonDocument doc(25000);

    for (int i = 0; i < preset.segment_amount; i++) {
        JsonObject segment = doc.createNestedObject();
        segment["color"] = preset.segments[i].color;
        segment["from"] = preset.segments[i].from;
        segment["to"] = preset.segments[i].to;
    }

    FileHandler::save_json_doc(filename.c_str(), doc);

    //--------------------------------------------------------------------------

    // read preset list file and append new preset config

    Color_Preset_Key *presets;
    int preset_amount;

    DynamicJsonDocument presetDoc(6500);
    if (!FileHandler::get_json_dynamic_doc(preset_list_file_name, presetDoc)) {
        return;
    }
    JsonObject presetObj = presetDoc.as<JsonObject>();

    presets = new Color_Preset_Key[presetObj.size() + 1];
    int index = 0;
    for (JsonPair item: presetObj) {
        presets[index].name = item.value()
                .as<String>();
        presets[index].id = item.key()
                .c_str();
        index++;
    }

    presets[index].name = name;
    presets[index].id = id;

    preset_amount = index + 1;

    //--------------------------------------------------------------------------

    // save preset list file

    DynamicJsonDocument presetListDoc(6500);
    for (int i = 0; i < preset_amount; i++) {
        presetListDoc[presets[i].id] = presets[i].name;
    }

    FileHandler::save_json_doc(preset_list_file_name, presetListDoc);
}

bool Config::saveEffect(String name, String id, String script, uint8_t clock_amount, String* clock_names, uint32_t* clock_times) {
    Serial.println("Save Effect to SD File:");

    Serial.println(name);
    Serial.println(id);
    Serial.println("-------------");
    Serial.println(script);
    Serial.println("-------------");
    Serial.println(clock_amount);

    for(int i = 0; i < clock_amount; i++) {
        Serial.print(clock_names[i]);
        Serial.print(": ");
        Serial.println(clock_times[i]);
    }

    // TODO
    // 1. Save new Effect File
    // 2. Save new Config File
    // 3. Load current Effect List
    // 4. Append new Effect
    // 5. Save Effect List

    // 1.
    String filename = effect_file_template;
    filename.replace("{id}", id);

    FileHandler::write_file(filename.c_str(), script);

    Serial.println("Successfully saved on SD Card");
    return true;
}
