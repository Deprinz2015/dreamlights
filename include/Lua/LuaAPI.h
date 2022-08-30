//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_LUAAPI_H
#define MAINCODE_LUAAPI_H

#include "lua.hpp"
#include "CustomTypes/LED_Array.h"

struct Lua_Methods {
    static int setLed(lua_State *L);

    static int setHue(lua_State *L);

    static int setRed(lua_State *L);

    static int setGreen(lua_State *L);

    static int setBlue(lua_State *L);

    static int addHue(lua_State *L);

    static int addRed(lua_State *L);

    static int addGreen(lua_State *L);

    static int addBlue(lua_State *L);

    static int setHSV(lua_State *L);

    static int setRGB(lua_State *L);

    static int getLed(lua_State *L);

    static int getSize(lua_State *L);

    static int fadeToBlack(lua_State *L);

    static int fadeAllToBlack(lua_State *L);

    static int randomIndex(lua_State *L);

    static int copyLed(lua_State *L);
};

struct Lua_Functions {
    static int getArray(lua_State *L);

    static int l_random8(lua_State *L);

    static int l_random16(lua_State *L);

    static int l_randomBool(lua_State *L);

    static int showLeds(lua_State *L);
};

static const struct luaL_Reg led_array_lib_m[] = {
        {"setLed",         Lua_Methods::setLed},
        {"setHue",         Lua_Methods::setHue},
        {"setRed",         Lua_Methods::setRed},
        {"setGreen",       Lua_Methods::setGreen},
        {"setBlue",        Lua_Methods::setBlue},
        {"addHue",         Lua_Methods::addHue},
        {"addRed",         Lua_Methods::addRed},
        {"addGreen",       Lua_Methods::addGreen},
        {"addBlue",        Lua_Methods::addBlue},
        {"setHSV",         Lua_Methods::setHSV},
        {"setRGB",         Lua_Methods::setRGB},
        {"getLed",         Lua_Methods::getLed},
        {"getSize",        Lua_Methods::getSize},
        {"fadeToBlack",    Lua_Methods::fadeToBlack},
        {"fadeAllToBlack", Lua_Methods::fadeAllToBlack},
        {"randomIndex",    Lua_Methods::randomIndex},
        {"copyLed",        Lua_Methods::copyLed},
        {NULL, NULL}
};

static const struct luaL_Reg led_array_lib_f[] = {
        {"new",        Lua_Functions::getArray},
        {"random8",    Lua_Functions::l_random8},
        {"random16",   Lua_Functions::l_random16},
        {"randomBool", Lua_Functions::l_randomBool},
        {"showLeds",   Lua_Functions::showLeds},
        {NULL, NULL}
};

#endif //MAINCODE_LUAAPI_H
