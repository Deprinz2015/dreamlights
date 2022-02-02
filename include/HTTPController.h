//
// Created by Niklas Koll on 25.12.21.
//

#ifndef MAINCODE_HTTPCONTROLLER_H
#define MAINCODE_HTTPCONTROLLER_H

void http_load_preset();

void http_play_effect();

void http_turn_off();

void http_turn_on();

void http_color_segment();

void http_save_preset();

void http_save_new_effect();

void http_display_solid_color();

void http_change_brightness();

void http_send_response();

void http_send_error();

void http_send_missing_argument(const String &);

void http_play_effect() {
    String id = server.arg("id");
    if (id == "") {
        http_send_missing_argument("id");
        return;
    }

    if (play_effect(id)) {
        http_send_response();
        // TODO: update device EFFECT
        return;
    }

    http_send_error();
}

void http_display_solid_color() {
    if (server.arg("value") == "") {
        http_send_missing_argument("value");
        return;
    }
    String value = server.arg("value").c_str();
    if (!value.startsWith("0x")) {
        value = "0x" + value;
    }
    uint32_t color = strtol(value.c_str(), nullptr, HEX);

    if (display_solid_color(color)) {
        http_send_response();
        update_device(UPDATE_COLOR);
        return;
    }

    http_send_error();
}

void http_change_brightness() {
    if (server.arg("value") == "") {
        http_send_missing_argument("value");
        return;
    }

    double brightnessPerc = strtod(server.arg("value").c_str(), nullptr) / 100;

    if (brightnessPerc > 1) {
        server.send(400);
        return;
    }

    if (setBrightness(brightnessPerc)) {
        http_send_response();
        update_device(UPDATE_BRIGHTNESS);
        return;
    }

    http_send_error();
}

void http_turn_off() {
    if (turn_off()) {
        update_device(UPDATE_ON);
        http_send_response();
        return;
    }

    http_send_error();
}

void http_turn_on() {
    if (turn_on()) {
        update_device(UPDATE_ON);
        http_send_response();
        return;
    }

    http_send_error();
}

void http_load_preset() {
    String id = server.arg("id");

    if (id == "") {
        http_send_missing_argument("id");
        return;
    }

    if (load_preset(id)) {
        http_send_response();
        // TODO: update device PRESET
        return;
    }

    http_send_error();
}

void http_color_segment() {
    if (!config.is_segmented_color) {
        fill_solid(leds, config.num_leds, CRGB::Black);
    }
    String color = server.arg("color");
    String from = server.arg("from");
    String to = server.arg("to");

    if (color == "") {
        http_send_missing_argument("color");
        return;
    }
    if (from == "") {
        http_send_missing_argument("from");
        return;
    }
    if (to == "") {
        to = from;
    }

    if (!color.startsWith("0x")) {
        color = "0x" + color;
    }
    uint32_t color_value = strtol(color.c_str(), nullptr, HEX);

    uint32_t start_index = strtol(from.c_str(), nullptr, DEC);
    uint32_t end_index = strtol(to.c_str(), nullptr, DEC);

    if (color_segment(color_value, start_index, end_index)) {
        http_send_response();
        // TODO: update device SEGMENT
        return;
    }

    http_send_error();
}

void http_save_preset() {
    String name = server.arg("name");
    if (name == "") {
        http_send_missing_argument("name");
    }

    if (save_preset(name)) {
        http_send_response();
        // TODO: update device PRESET
    }

    http_send_error();
}

void http_save_new_effect() {

}

void http_send_response() {
    String response1 = R"({"current_pattern": ")";
    String response2 = "\"}";
    String responseComplete = response1 + config.last_id + response2;
    server.send(200, "json", responseComplete);

    saveMainConfig(config);
}

void http_send_error() {
    server.send(418);
}

void http_send_missing_argument(const String &missing_argument) {
    String response1 = R"({"error": "missing argument: )";
    String response2 = "\"}";
    String responseComplete = response1 + missing_argument + response2;
    server.send(404, "json", responseComplete);
}

#endif //MAINCODE_HTTPCONTROLLER_H
