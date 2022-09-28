//
// Created by Niklas Koll on 17.07.22.
//

#include "LEDServer.h"
#include "Globals.h"
#include "Constants.h"
#include "HTTP_API.h"

void LEDServer::setup() {
    server.on(ROUTE_EFFECT, HTTP_GET, HTTP_API::play_effect);
    server.on(ROUTE_CHANGE_COLOR, HTTP_GET, HTTP_API::display_solid_color);
    server.on(ROUTE_POWER, HTTP_GET, HTTP_API::set_power);
    server.on(ROUTE_CHANGE_BRIGHTNESS, HTTP_GET, HTTP_API::change_brightness);
    server.on(ROUTE_CHANGE_SEGMENT, HTTP_GET, HTTP_API::color_segment);
    server.on(ROUTE_SAVE_PRESET, HTTP_GET, HTTP_API::save_preset);
    server.on(ROUTE_LOAD_PRESET, HTTP_GET, HTTP_API::load_preset);
    server.on(ROUTE_SAVE_NEW_EFFECT, HTTP_POST, HTTP_API::save_new_effect);
}

void LEDServer::begin() {
    server.begin();
}

void LEDServer::handle() {
    server.handleClient();
}