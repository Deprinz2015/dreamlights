//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_HOMESPANHANDLER_H
#define MAINCODE_HOMESPANHANDLER_H

#include "LED_API.h"

struct HomeSpanHandler {
    static void wificonnect();
    static void setup_homespan(void(*)(), LED_API *ledApi);
    static void homespan_loop();
};

#endif //MAINCODE_HOMESPANHANDLER_H