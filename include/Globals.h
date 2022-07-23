//
// Created by Niklas Koll on 17.07.22.
//

#ifndef MAINCODE_GLOBALS_H
#define MAINCODE_GLOBALS_H

#include "WebServer.h"
#include "WiFiManager.h"
#include "CustomTypes/Main_Config.h"
#include "Lua/LuaMain.h"

extern WebServer server;
extern WiFiManager wm;

extern MainConfig config;

extern LuaHandler lua;

#endif //MAINCODE_GLOBALS_H
