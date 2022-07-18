//
// Created by Niklas Koll on 17.07.22.
//

#include "LEDServer.h"
#include "Globals.h"

void LEDServer::setup() {
    // TODO Insert all Server Routes here
}

void LEDServer::begin() {
    server.begin();
}

void LEDServer::handle() {
    server.handleClient();
}