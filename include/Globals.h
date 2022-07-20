//
// Created by Niklas Koll on 17.07.22.
//

#ifndef MAINCODE_GLOBALS_H
#define MAINCODE_GLOBALS_H

#include "WiFiManager.h"
#include "CustomTypes/Main_Config.h"
#include "LEDServer.h"
#include "LED_API.h"
#include "Lua/LuaMain.h"

WebServer server(80);
WiFiManager wm;

MainConfig config;

LuaHandler lua;

#endif //MAINCODE_GLOBALS_H
