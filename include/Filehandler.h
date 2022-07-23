//
// Created by Niklas Koll on 16.07.22.
//

#ifndef MAINCODE_FILEHANDLER_H
#define MAINCODE_FILEHANDLER_H

#include "ArduinoJson.h"

struct FileHandler {
    template<typename T = JsonObject>
    static bool get_json_dynamic_doc(const char *filename, size_t doc_size, T *obj);

    template<typename T = JsonObject>
    static bool get_json_static_doc(const char *filename, T *obj);

    static String read_file(const char *filename);

    template<typename TSource>
    static void save_json_doc(const char *filename, TSource doc);
};

#endif //MAINCODE_FILEHANDLER_H
