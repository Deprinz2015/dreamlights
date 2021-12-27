//
// Created by Niklas Koll on 19.08.21.
//

#ifndef MAINCODE_LUACONTROLLER_H
#define MAINCODE_LUACONTROLLER_H

#include "lua.hpp"
#include "lualib.h"
#include "lauxlib.h"

#define LUA_LED_ARRAY_METATABLE "led_array.mt"
#define LUA_LED_ARRAY_LIB_NAME "leds"

struct LED_Array {
    size_t size;
    CRGB *leds;
};

lua_State *L;

LED_Array led_array;

static void dumpstack() {
    int top = lua_gettop(L);
    for (int i = 1; i <= top; i++) {
        Serial.printf("%d\t%s\t", i, luaL_typename(L, i));
        switch (lua_type(L, i)) {
            case LUA_TNUMBER:
                Serial.printf("%g\n", lua_tonumber(L, i));
                break;
            case LUA_TSTRING:
                Serial.printf("%s\n", lua_tostring(L, i));
                break;
            case LUA_TBOOLEAN:
                Serial.printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
                break;
            case LUA_TNIL:
                Serial.printf("%s\n", "nil");
                break;
            default:
                Serial.printf("%p\n", lua_topointer(L, i));
                break;
        }
    }
}

LED_Array *check_led_array(lua_State *l) {
    LED_Array *a = *(LED_Array **) luaL_checkudata(l, 1, LUA_LED_ARRAY_METATABLE);
    luaL_argcheck(l, a != nullptr, 1, "`led_array' expected");
    return a;
}

static int l_setLed(lua_State *l) {
    LED_Array *a = check_led_array(l);
    int index = luaL_checkinteger(l, 2);
    luaL_checktype(l, 3, LUA_TTABLE);
    luaL_argcheck(l, 1 <= index && index <= a->size, 2,
                  "index out of range");

    index -= 1;

    CHSV hsv{0, 255, 255};
    CRGB rgb{0, 0, 0};
    bool useHSV = false;
    lua_pushnil(l);
    while (lua_next(l, 3) != 0) {
        const char *key = lua_tostring(l, -2);
        int value = luaL_checkinteger(l, -1);
        if (strcmp(key, "r") == 0) {
            rgb.r = value;
        } else if (strcmp(key, "g") == 0) {
            rgb.g = value;
        } else if (strcmp(key, "b") == 0) {
            rgb.b = value;
        } else if (strcmp(key, "h") == 0) {
            hsv.h = value;
            useHSV = true;
        } else if (strcmp(key, "s") == 0) {
            hsv.s = value;
        } else if (strcmp(key, "v") == 0) {
            hsv.v = value;
        }

        lua_pop(L, 1);
    }
    if (useHSV) {
        a->leds[index] = hsv;
    } else {
        a->leds[index] = rgb;
    }

    return 0;
}

static int l_setHSV(lua_State *l) {
    LED_Array *a = check_led_array(l);
    int index = luaL_checkinteger(l, 2);
    luaL_argcheck(l, 1 <= index && index <= a->size, 2,
                  "index out of range");
    int h = luaL_checkinteger(l, 3);
    int s = luaL_checkinteger(l, 4);
    int v = luaL_checkinteger(l, 5);

    a->leds[index - 1].setHSV(h, s, v);

    return 0;
}

static int l_setRGB(lua_State *l) {
    LED_Array *a = check_led_array(l);
    int index = luaL_checkinteger(l, 2);
    luaL_argcheck(l, 1 <= index && index <= a->size, 2,
                  "index out of range");
    int r = luaL_checkinteger(l, 3);
    int g = luaL_checkinteger(l, 4);
    int b = luaL_checkinteger(l, 5);

    a->leds[index - 1].setRGB(r, g, b);

    return 0;
}

static int l_setHue(lua_State *l) {
    LED_Array *a = check_led_array(l);
    int index = luaL_checkinteger(l, 2);
    luaL_argcheck(l, 1 <= index && index <= a->size, 2,
                  "index out of range");
    int value = luaL_checkinteger(l, 3);
    a->leds[index - 1].setHue(value);
    return 0;
}

static int l_setRed(lua_State *l) {
    LED_Array *a = check_led_array(l);
    int index = luaL_checkinteger(l, 2);
    luaL_argcheck(l, 1 <= index && index <= a->size, 2,
                  "index out of range");
    int value = luaL_checkinteger(l, 3);
    a->leds[index - 1].r = value;
    return 0;
}

static int l_setGreen(lua_State *l) {
    LED_Array *a = check_led_array(l);
    int index = luaL_checkinteger(l, 2);
    luaL_argcheck(l, 1 <= index && index <= a->size, 2,
                  "index out of range");
    int value = luaL_checkinteger(l, 3);
    a->leds[index - 1].g = value;
    return 0;
}

static int l_setBlue(lua_State *l) {
    LED_Array *a = check_led_array(l);
    int index = luaL_checkinteger(l, 2);
    luaL_argcheck(l, 1 <= index && index <= a->size, 2,
                  "index out of range");
    int value = luaL_checkinteger(l, 3);
    a->leds[index - 1].b = value;
    return 0;
}

static int l_addHue(lua_State *l) {
    LED_Array *a = check_led_array(l);
    int index = luaL_checkinteger(l, 2);
    luaL_argcheck(l, 1 <= index && index <= a->size, 2,
                  "index out of range");
    uint8_t value = luaL_checkinteger(l, 3);
    a->leds[index - 1] += CHSV{value, 255, 255};
    return 0;
}

static int l_addRed(lua_State *l) {
    LED_Array *a = check_led_array(l);
    int index = luaL_checkinteger(l, 2);
    luaL_argcheck(l, 1 <= index && index <= a->size, 2,
                  "index out of range");
    int value = luaL_checkinteger(l, 3);
    a->leds[index - 1].r += value;
    return 0;
}

static int l_addGreen(lua_State *l) {
    LED_Array *a = check_led_array(l);
    int index = luaL_checkinteger(l, 2);
    luaL_argcheck(l, 1 <= index && index <= a->size, 2,
                  "index out of range");
    int value = luaL_checkinteger(l, 3);
    a->leds[index - 1].g += value;
    return 0;
}

static int l_addBlue(lua_State *l) {
    LED_Array *a = check_led_array(l);
    int index = luaL_checkinteger(l, 2);
    luaL_argcheck(l, 1 <= index && index <= a->size, 2,
                  "index out of range");
    int value = luaL_checkinteger(l, 3);
    a->leds[index - 1].b += value;
    return 0;
}

static int l_getArray(lua_State *l) {
    LED_Array **a = (LED_Array **) lua_newuserdata(l, sizeof(LED_Array));
    *a = &led_array;
    luaL_setmetatable(l, LUA_LED_ARRAY_METATABLE);
    return 1;
}

static int l_getLed(lua_State *l) {
    LED_Array *a = check_led_array(l);
    int index = luaL_checkinteger(l, 2);

    luaL_argcheck(l, 1 <= index && index <= a->size, 2,
                  "index out of range");

    lua_pushinteger(l, a->leds[index - 1].r);
    lua_pushinteger(l, a->leds[index - 1].g);
    lua_pushinteger(l, a->leds[index - 1].b);
    return 3;
}

static int l_getSize(lua_State *l) {
    LED_Array *a = check_led_array(l);
    lua_pushinteger(l, a->size);
    return 1;
}

int l_copyLed(lua_State *l) {
    LED_Array *a = check_led_array(l);
    int targetindex = luaL_checkinteger(l, 2);
    int sourceindex = luaL_checkinteger(l, 3);
    a->leds[targetindex - 1] = a->leds[sourceindex - 1];
    return 0;
}

int l_fadeToBlack(lua_State *l) {
    LED_Array *a = check_led_array(l);
    int index = luaL_checkinteger(l, 2);
    int fadingSpeed = luaL_checkinteger(l, 3);
    a->leds[index - 1].fadeToBlackBy(fadingSpeed);

    return 0;
}

// -------------------------------LED UTIL-------------------------------
int l_fadeAllToBlack(lua_State *l) {
    LED_Array *a = check_led_array(l);
    int fadingSpeed = luaL_checkinteger(L, 2);
    fadeToBlackBy(a->leds, a->size, fadingSpeed);

    return 0;
}

int l_randomIndex(lua_State *l) {
    LED_Array *a = check_led_array(l);
    uint16_t index = random16(0, (uint16_t) (a->size)) + 1;
    lua_pushinteger(l, index);

    return 1;
}

int l_random8(lua_State *l) {
    int firstLim = luaL_optinteger(l, 1, -1);
    int secondLim = luaL_optinteger(l, 2, -1);
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

    lua_pushinteger(l, random);

    return 1;
}

int l_random16(lua_State *l) {
    int firstLim = luaL_optinteger(l, 1, -1);
    int secondLim = luaL_optinteger(l, 2, -1);
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

    lua_pushinteger(l, random);
    return 1;
}

int l_randomBool(lua_State *l) {
    int prob = luaL_optinteger(l, 1, 50);
    bool result = random8(100) < prob;
    lua_pushboolean(l, result);
    return 1;
}

int l_showLeds(lua_State *l) {
    FastLED.show();

    return 0;
}
// -------------------------------END LED UTIL-------------------------------

void runLuaPattern(const char *name) {
    //last_moment = millis();

    lua_getglobal(L, name);
    lua_pcall(L, 0, 0, 0);

    //current_moment = millis();
    //Serial.println(current_moment - last_moment);
}

// Object methods
static const struct luaL_Reg led_array_lib_m[] = {
        {"set",            l_setLed},
        {"setHue",         l_setHue},
        {"setRed",         l_setRed},
        {"setGreen",       l_setGreen},
        {"setBlue",        l_setBlue},
        {"addHue",         l_addHue},
        {"addRed",         l_addRed},
        {"addGreen",       l_addGreen},
        {"addBlue",        l_addBlue},
        {"setHSV",         l_setHSV},
        {"setRGB",         l_setRGB},
        {"get",            l_getLed},
        {"size",           l_getSize},
        {"fadeToBlack",    l_fadeToBlack},
        {"fadeAllToBlack", l_fadeAllToBlack},
        {"randomIndex",    l_randomIndex},
        {"copyLed",        l_copyLed},
        {NULL, NULL}
};

// static functions
static const struct luaL_Reg led_array_lib_f[] = {
        {"new",           l_getArray},
        {"random8",       l_random8},
        {"random16",      l_random16},
        {"randomBoolean", l_randomBool},
        {"showLeds",      l_showLeds},
        {NULL, NULL}
};

int luaopen_array(lua_State *l) {
    luaL_newmetatable(l, LUA_LED_ARRAY_METATABLE);

    lua_pushstring(l, "__index");
    lua_pushvalue(l, -2);
    lua_settable(l, -3);

    luaL_setfuncs(l, led_array_lib_m, 0);
    luaL_setfuncs(l, led_array_lib_f, 0);
    lua_setglobal(l, LUA_LED_ARRAY_LIB_NAME);

    return 1;
}

void open_lua(CRGB *leds, size_t num_leds) {
    L = luaL_newstate();
    luaopen_base(L);
    luaopen_math(L);
    luaopen_string(L);
    luaopen_table(L);
    luaopen_array(L);

    led_array.leds = leds;
    led_array.size = num_leds;
}

void close_lua() {
    lua_close(L);
}

bool load_lua_script(const Effect &fx) {
    String scriptString = read_lua_script(fx);

    return luaL_loadstring(L, scriptString.c_str()) || lua_pcall(L, 0, 0, 0);
}

#endif //MAINCODE_LUACONTROLLER_H
