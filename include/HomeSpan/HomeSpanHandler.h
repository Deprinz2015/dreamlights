//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_HOMESPANHANDLER_H
#define MAINCODE_HOMESPANHANDLER_H

#include "HomeSpan.h"
#include "HomeSpan/DEV_LEDStrip.h"
#include "HomeSpan/DEV_Identify.h"
#include "Constants.h"
#include "Globals.h"

struct HomeSpanHandler {
    static void wificonnect();
    static void setup_homespan(void(*)());
    static void homespan_loop();
};

#endif //MAINCODE_HOMESPANHANDLER_H