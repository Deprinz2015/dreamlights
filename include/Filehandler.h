//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_FILEHANDLER_H
#define MAINCODE_FILEHANDLER_H

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "ArduinoJson.h"
#include "Constants.h"

struct FileHandler {
    static bool get_json_dynamic_doc(const char *filename, JsonDocument &doc);

    static bool get_json_static_doc(const char *filename, StaticJsonDocument<STATIC_JSON_OBJ_SIZE> &doc);

    static String read_file(const char *filename);
    static bool write_file(const char* filename, String content);

    template<typename TSource>
    static void save_json_doc(const char *filename, TSource doc) {
        File file = SD.open(filename, FILE_WRITE);
        if (!file) {
            Serial.println("File open failed!");
            return;
        }
        serializeJson(doc, file);
        file.close();
    }
};

#endif //MAINCODE_FILEHANDLER_H
