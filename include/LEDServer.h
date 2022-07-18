//
// Created by Niklas Koll on 17.07.22.
//

#ifndef MAINCODE_LEDSERVER_H
#define MAINCODE_LEDSERVER_H

struct LEDServer {
    static void setup();
    static void begin();
    static void handle();
};

#endif //MAINCODE_LEDSERVER_H
