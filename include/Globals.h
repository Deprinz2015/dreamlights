//
// Created by Niklas Koll on 17.07.22.
//

#ifndef MAINCODE_GLOBALS_H
#define MAINCODE_GLOBALS_H

#include "WebServer.h"
#include "CustomTypes/Main_Config.h"
#include "Lua/LuaMain.h"
#include "LED_API.h"

extern WebServer server;

extern MainConfig config;

extern LuaHandler lua;

extern LED_API leds;

#endif //MAINCODE_GLOBALS_H
