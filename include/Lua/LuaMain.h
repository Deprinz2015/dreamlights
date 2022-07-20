//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_LUAMAIN_H
#define MAINCODE_LUAMAIN_H

#include "lua.hpp"
#include "CustomTypes/LED_Array.h"
#include "CustomTypes/Effect.h"

struct LuaHandler {
    lua_State *L;
    LED_Array led_array;

    void open_lua(CRGB *leds, uint16_t num_leds);
    void close_lua();
    bool load_lua_script(const Effect &fx);
    void luaopen_array(lua_State *L);
    void run_lua_pattern(const char* name);
};

#endif //MAINCODE_LUAMAIN_H
