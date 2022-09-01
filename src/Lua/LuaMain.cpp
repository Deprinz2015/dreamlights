//
// Created by Niklas Koll on 16.07.22.
//

#include "Lua/LuaMain.h"
#include "Lua/LuaAPI.h"
#include "Constants.h"
#include "Config.h"

void LuaHandler::open_lua(CRGB *leds, uint16_t num_leds) {
    L = luaL_newstate();
    luaopen_base(L);
    luaopen_math(L);
    luaopen_string(L);
    luaopen_table(L);
    luaopen_array(L);

    led_array.leds = leds;
    led_array.size = num_leds;
}

void LuaHandler::close_lua() const {
    lua_close(L);
}

void LuaHandler::load_lua_script(const Effect &fx) const {
    String script = Config::read_lua_script(fx);
    luaL_loadstring(L, script.c_str());
    if(lua_pcall(L, 0, 0, 0) != 0) {
        Serial.print("Error on prime call for lua script: ");
        Serial.println(lua_tostring(L, -1));
    }
}

void LuaHandler::luaopen_array(lua_State *L) {
    luaL_newmetatable(L, LUA_LED_ARRAY_METATABLE);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);

    luaL_setfuncs(L, led_array_lib_m, 0);
    luaL_setfuncs(L, led_array_lib_f, 0);
    lua_setglobal(L, LUA_LED_ARRAY_LIB_NAME);
}

void LuaHandler::run_lua_pattern(const char *name) const {
    lua_getglobal(L, name);
    if(lua_pcall(L, 0, 0, 0) != 0) {
        Serial.print("Error when called Lua Function ");
        Serial.print(name);
        Serial.print("(): ");
        Serial.println(lua_tostring(L, -1));
    }
}
