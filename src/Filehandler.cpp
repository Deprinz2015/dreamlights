//
// Created by Niklas Koll on 31.08.22.
//

#include "Filehandler.h"

bool FileHandler::get_json_dynamic_doc(const char *filename, JsonDocument &doc) {
    File file = SD.open(filename);
    if (!file) {
        Serial.println("File open failed!");
        return false;
    }
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return false;
    }
    file.close();

    return true;
}

bool FileHandler::get_json_static_doc(const char *filename, StaticJsonDocument<STATIC_JSON_OBJ_SIZE> &doc) {
    File file = SD.open(filename);
    if (!file) {
        Serial.println("File open failed!");
        return false;
    }
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return false;
    }
    file.close();

    return true;
}

String FileHandler::read_file(const char *filename) {
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

bool FileHandler::write_file(const char *filename, String &content) {
    File file = SD.open(filename, "w");
    if (!file) {
        Serial.println("File open failed!");
        return "";
    }

    bool success = file.print(content);

    file.close();
    return success;
}