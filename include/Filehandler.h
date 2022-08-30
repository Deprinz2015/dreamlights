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
    template<typename T = JsonObject>
    static bool get_json_dynamic_doc(const char *filename, size_t doc_size, T &obj) {
        File file = SD.open(filename);
        if (!file) {
            Serial.println("File open failed!");
            return false;
        }
        DynamicJsonDocument doc(doc_size);
        DeserializationError error = deserializeJson(doc, file);
        if (error) {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return false;
        }
        obj = doc.as<T>();
        file.close();

        return true;
    }

    template<typename T = JsonObject>
    static bool get_json_static_doc(const char *filename, T &obj) {
        File file = SD.open(filename);
        if (!file) {
            Serial.println("File open failed!");
            return false;
        }
        StaticJsonDocument<STATIC_JSON_OBJ_SIZE> doc;
        DeserializationError error = deserializeJson(doc, file);
        if (error) {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return false;
        }
        obj = doc.as<T>();
        file.close();

        return true;
    }

    static String read_file(const char *filename) {
        File file = SD.open(filename);
        if (!file) {
            Serial.println("File open failed!");
            return "";
        }
        String content = "";
        while (file.available()) {
            content += (char) file.read();
        }
        file.close();
        return content;
    }

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
