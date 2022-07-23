//
// Created by Niklas Koll on 16.07.22.
//

#include <FS.h>
#include <SD.h>
#include "Filehandler.h"
#include "Constants.h"

template<typename T>
bool FileHandler::get_json_dynamic_doc(const char *filename, size_t doc_size, T *obj) {
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
    file.close();
    if(obj) {
        *obj = doc.as<T>();
    }

    return true;
}

template<typename T>
bool FileHandler::get_json_static_doc(const char *filename, T *obj) {
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
    file.close();
    if(obj) {
        *obj = doc.as<T>();
    }

    return true;
}

template<typename TSource>
void FileHandler::save_json_doc(const char *filename, TSource doc) {
    File file = SD.open(filename, FILE_WRITE);
    if (!file) {
        Serial.println("File open failed!");
        return;
    }
    serializeJson(doc, file);
    file.close();
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