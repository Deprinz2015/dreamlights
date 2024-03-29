//
// Created by Niklas Koll on 16.07.22.
//

#include "HTTP_API.h"
#include "Globals.h"
#include "Config.h"

bool HTTP_API::getUrlArgument(const String& arg, String &value) {
    value = server.arg(arg);
    if(value == "") {
        send_response(400, "Bad Request, missing argument: " + arg);
        return false;
    }
    return true;
}

bool HTTP_API::getBody(String &value) {
    value = server.arg("plain");
    if(value == "") {
        send_response(400, "Bad Request, missing body");
        return false;
    }
    return true;
}

String HTTP_API::getUrlArgumentWithoutCheck(const String& arg) {
    return server.arg(arg);
}

void HTTP_API::load_preset() {
    String id;
    if(!getUrlArgument("id", id)) {
        return;
    }

    leds.loadPreset(id);
    send_response(200, "Preset loaded");
    // TODO Update Homespan
}

void HTTP_API::save_preset() {
    String presetName;
    if(!getUrlArgument("name", presetName)) {
        return;
    }

    leds.save_preset(presetName);
    send_response(200, "Preset saved");
}

void HTTP_API::color_segment() {
    if(!config.is_segmented_color) {
        fill_solid(leds.leds, config.num_leds, CRGB::Black);
    }

    String color;
    String from;
    String to = getUrlArgumentWithoutCheck("to");
    if(!getUrlArgument("color", color) || !getUrlArgument("from", from)) {
        return;
    }

    if(to == "") {
        to = from;
        return;
    }

    if(!color.startsWith("0x")) {
        color = "0x" + color;
    }

    uint32_t color_value = strtol(color.c_str(), nullptr, HEX);
    uint16_t start_index = strtol(from.c_str(), nullptr, DEC);
    uint16_t end_index = strtol(to.c_str(), nullptr, DEC);

    leds.color_segment(start_index, end_index, color_value);
    send_response(200, "Segment colored");
    // TODO Update Homespan
    // Change Preset number to make it obvious that its not saved (zb. -1)
}

void HTTP_API::play_effect() {
    Serial.println("Play Effect");

    String id;
    if(!getUrlArgument("id", id)) {
        return;
    }
    leds.play_effect(id);
    send_response(200, "Effect played");
    leds.update_homespan(UPDATE_EFFECT);
}

void HTTP_API::save_new_effect() {
    String script;
    if(!getBody(script)) {
        return;
    }

    String effectName = "";

    auto clock_names = new String[10];
    auto clock_times = new uint32_t[10];

    int clockIndex = 0;
    int argCount = server.args();
    String argName;
    String timeString;
    for(int i = 0; i < argCount; i++) {
        argName = server.argName(i);

        if(argName == "plain") {
            continue;
        }

        if(argName == "name") {
            if(!getUrlArgument(argName, effectName)) {
                return;
            }
            continue;
        }

        if(!getUrlArgument(argName, timeString)) {
            return;
        }

        clock_names[clockIndex] = argName;
        clock_times[clockIndex] = strtol(timeString.c_str(), nullptr, DEC);

        clockIndex++;
    }

    String newId = leds.save_new_effect(effectName, script, clockIndex, clock_names, clock_times);
    leds.play_effect(newId);
    leds.update_homespan(UPDATE_EFFECT);

    send_response(200, "effect saved");
}

void HTTP_API::save_effect() {
    String script;
    if(!getBody(script)) {
        return;
    }

    String effectID = "";

    auto clock_names = new String[10];
    auto clock_times = new uint32_t[10];

    int clockIndex = 0;
    int argCount = server.args();
    String argName;
    String timeString;
    for(int i = 0; i < argCount; i++) {
        argName = server.argName(i);

        if(argName == "plain") {
            continue;
        }

        if(argName == "id") {
            if(!getUrlArgument(argName, effectID)) {
                return;
            }
            continue;
        }

        if(!getUrlArgument(argName, timeString)) {
            return;
        }

        clock_names[clockIndex] = argName;
        clock_times[clockIndex] = strtol(timeString.c_str(), nullptr, DEC);

        clockIndex++;
    }

    leds.save_effect(effectID, script, clockIndex, clock_names, clock_times);
    leds.play_effect(effectID);
    leds.update_homespan(UPDATE_EFFECT);

    send_response(200, "effect saved");
}

void HTTP_API::display_solid_color() {
    String value;
    if(!getUrlArgument("value", value)) {
        return;
    }

    if(!value.startsWith("0x")) {
        value = "0x" + value;
    }

    uint32_t color_value = strtol(value.c_str(), nullptr, HEX);
    leds.display_solid_color(color_value);
    send_response(200, "Solid color displayed");
    leds.update_homespan(UPDATE_COLOR);
}

void HTTP_API::change_brightness() {
    String brightness;
    if(!getUrlArgument("value", brightness)) {
        return;
    }

    uint8_t brightness_value = strtol(brightness.c_str(), nullptr, DEC);

    if(brightness_value > 100 || brightness_value < 0) {
        send_response(400, "Bad Request, brightness value must be between 0 and 100");
        return;
    }

    leds.setBrightness(brightness_value);
    send_response(200, "Brightness changed");
    leds.update_homespan(UPDATE_BRIGHTNESS);
}

void HTTP_API::set_power() {
    String power;
    if(!getUrlArgument("value", power)) {
        return;
    }

    if(power == "on") {
        leds.set_power(POWER_ON);
    } else if(power == "off") {
        leds.set_power(POWER_OFF);
    } else {
        send_response(400, "Bad Request, power must be 'on' or 'off'");
        return;
    }

    leds.update_homespan(UPDATE_POWER);

    send_response(200, "Power " + power);
}

void HTTP_API::get_current_status() {
    // TODO Implement Endpoint

    // Whats in the response:
    // 1. current brightness
    // 2. current mode (effect, solid color, preset)
    // 3. if effect/preset -> effect-(preset-)id
    // 4. if solid color -> color-value
}

// TODO redo JSON answers
// Maybe use helper methods to build the json constructs
// more general method to return stuff
void HTTP_API::send_response(int code, const String &message) {
    String part1 = R"({"message": ")";
    if(code > 400) {
        part1 = R"({"error": ")";
    }
    String part2 = R"("})";
    String response = part1 + message + part2;
    server.send(code, "application/json", response);
}