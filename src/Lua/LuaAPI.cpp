//
// Created by Niklas Koll on 16.07.22.
//

#include "Lua/LuaAPI.h"
#include "Globals.h"
#include "Constants.h"
#include "FastLED.h"

int Lua_Functions::getArray(lua_State *L) {
    auto **a = (LED_Array **) lua_newuserdata(L, sizeof(LED_Array));
    *a = &lua.led_array;
    luaL_setmetatable(L, LUA_LED_ARRAY_METATABLE);
    return 1;
}

int Lua_Functions::l_random8(lua_State *L) {
    int firstLim = luaL_optinteger(L, 1, -1);
    int secondLim = luaL_optinteger(L, 2, -1);
    uint8_t random;

    if (secondLim < 0) {
        if (firstLim < 0) {
            random = random8();
        } else {
            random = random8(firstLim);
        }
    } else {
        random = random8(firstLim, secondLim);
    }

    lua_pushinteger(L, random);

    return 1;
}

int Lua_Functions::l_random16(lua_State *L) {
    int firstLim = luaL_optinteger(L, 1, -1);
    int secondLim = luaL_optinteger(L, 2, -1);
    uint16_t random;

    if (secondLim < 0) {
        if (firstLim < 0) {
            random = random16();
        } else {
            random = random16(firstLim);
        }
    } else {
        random = random16(firstLim, secondLim);
    }

    lua_pushinteger(L, random);

    return 1;
}

int Lua_Functions::l_randomBool(lua_State *L) {
    uint8_t prob = luaL_optinteger(L, 1, 50);
    bool result = random8(100) < prob;
    lua_pushboolean(L, result);
    return 1;
}

int Lua_Functions::showLeds(lua_State *L) {
    FastLED.show();
    return 0;
}

//------------------------------------------------------------------------------
LED_Array *check_led_array(lua_State *L) {
    LED_Array *a = *(LED_Array **) luaL_checkudata(L, 1, LUA_LED_ARRAY_METATABLE);
    luaL_argcheck(L, a != nullptr, 1, "'led_array' expected");
    return a;
}

uint16_t check_led_index(lua_State *L, LED_Array *a, int index) {
    uint16_t ledIndex = luaL_checkinteger(L, index);
    luaL_argcheck(L, 1 <= ledIndex && ledIndex <= a->size, index, "Index out of bounds");
    return ledIndex - 1;
}

int Lua_Methods::setHSV(lua_State *L) {
    LED_Array *a = check_led_array(L);
    uint16_t index = check_led_index(L, a, 2);
    uint8_t h = luaL_checkinteger(L, 3);
    uint8_t s = luaL_checkinteger(L, 4);
    uint8_t v = luaL_checkinteger(L, 5);

    a->leds[index].setHSV(h, s, v);

    return 0;
}

int Lua_Methods::setRGB(lua_State *L) {
    LED_Array *a = check_led_array(L);
    uint16_t index = check_led_index(L, a, 2);
    uint8_t r = luaL_checkinteger(L, 3);
    uint8_t g = luaL_checkinteger(L, 4);
    uint8_t b = luaL_checkinteger(L, 5);

    a->leds[index].setRGB(r, g, b);

    return 0;
}

int Lua_Methods::setHue(lua_State *L) {
    LED_Array *a = check_led_array(L);
    uint16_t index = check_led_index(L, a, 2);
    uint8_t h = luaL_checkinteger(L, 3);

    a->leds[index].setHue(h);

    return 0;
}

int Lua_Methods::setRed(lua_State *L) {
    LED_Array *a = check_led_array(L);
    uint16_t index = check_led_index(L, a, 2);
    uint8_t r = luaL_checkinteger(L, 3);

    a->leds[index].r = r;

    return 0;
}

int Lua_Methods::setGreen(lua_State *L) {
    LED_Array *a = check_led_array(L);
    uint16_t index = check_led_index(L, a, 2);
    uint8_t g = luaL_checkinteger(L, 3);

    a->leds[index].g = g;

    return 0;
}

int Lua_Methods::setBlue(lua_State *L) {
    LED_Array *a = check_led_array(L);
    uint16_t index = check_led_index(L, a, 2);
    uint8_t b = luaL_checkinteger(L, 3);

    a->leds[index].b = b;

    return 0;
}

int Lua_Methods::addHue(lua_State *L) {
    LED_Array *a = check_led_array(L);
    uint16_t index = check_led_index(L, a, 2);
    uint8_t h = luaL_checkinteger(L, 3);

    a->leds[index] += CHSV{h, 255, 255};

    return 0;
}

int Lua_Methods::addRed(lua_State *L) {
    LED_Array *a = check_led_array(L);
    uint16_t index = check_led_index(L, a, 2);
    uint8_t r = luaL_checkinteger(L, 3);

    a->leds[index].r += r;

    return 0;
}

int Lua_Methods::addGreen(lua_State *L) {
    LED_Array *a = check_led_array(L);
    uint16_t index = check_led_index(L, a, 2);
    uint8_t g = luaL_checkinteger(L, 3);

    a->leds[index].g += g;

    return 0;
}

int Lua_Methods::addBlue(lua_State *L) {
    LED_Array *a = check_led_array(L);
    uint16_t index = check_led_index(L, a, 2);
    uint8_t b = luaL_checkinteger(L, 3);

    a->leds[index].b += b;

    return 0;
}

int Lua_Methods::getLed(lua_State *L) {
    LED_Array *a = check_led_array(L);
    uint16_t index = check_led_index(L, a, 2);

    lua_pushinteger(L, a->leds[index].r);
    lua_pushinteger(L, a->leds[index].g);
    lua_pushinteger(L, a->leds[index].b);
    return 3;
}

int Lua_Methods::getSize(lua_State *L) {
    LED_Array *a = check_led_array(L);
    lua_pushinteger(L, a->size);
    return 1;
}

int Lua_Methods::fadeToBlack(lua_State *L) {
    LED_Array *a = check_led_array(L);
    uint16_t index = check_led_index(L, a, 2);
    uint8_t fade = luaL_checkinteger(L, 3);
    a->leds[index].fadeToBlackBy(fade);
    return 0;
}

int Lua_Methods::fadeAllToBlack(lua_State *L) {
    LED_Array *a = check_led_array(L);
    uint8_t fade = luaL_checkinteger(L, 2);
    fadeToBlackBy(a->leds, a->size, fade);
    return 0;
}

int Lua_Methods::randomIndex(lua_State *L) {
    LED_Array *a = check_led_array(L);
    uint16_t index = random16(0, (uint16_t) (a->size)) + 1;
    lua_pushinteger(L, index);
    return 1;
}

int Lua_Methods::copyLed(lua_State *L) {
    LED_Array *a = check_led_array(L);
    uint16_t targetindex = luaL_checkinteger(L, 2);
    uint16_t sourceindex = luaL_checkinteger(L, 3);
    a->leds[targetindex - 1] = a->leds[sourceindex - 1];
    return 0;
    
}
