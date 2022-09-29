//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_HTTP_API_H
#define MAINCODE_HTTP_API_H

#include "Arduino.h"

struct HTTP_API {
    static void load_preset();
    static void save_preset();

    static void color_segment();

    static void play_effect();
    static void save_new_effect();
    static void save_effect();

    static void display_solid_color();
    static void change_brightness();
    static void set_power();

    static void send_response(int code, const String &message);

private:
    static bool getBody(String &value);
    static bool getUrlArgument(const String &arg, String &value);
    static String getUrlArgumentWithoutCheck(const String &arg);
};

#endif //MAINCODE_HTTP_API_H
