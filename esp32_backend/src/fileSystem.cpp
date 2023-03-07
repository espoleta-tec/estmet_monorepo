//
// Created by lazt on 4/23/21.
//

#include "FileSystem/fileSystem.h"
#include "utils/utils.h"
#include "string"

DynamicJsonDocument doc = DynamicJsonDocument(1024);
using namespace Vortice;

void initFileSystem() {
    const char *spiffs = "SPIFFS";

    if (!SPIFFS.begin()) {
        Vortice::printDiagnostic(spiffs, Vortice::Status[Vortice::FAILED_TO_START]);
    }

    ulong total = SPIFFS.totalBytes();
    ulong used = SPIFFS.usedBytes();
    double free = (double) used / (double) total * 100.0;

    char *memoryUsage = (char *) malloc(sizeof(char) * 100);
    sprintf(memoryUsage, "%lu/%luKB used", used / 1000, total / 1000);
    printDiagnostic("SPIFFS", memoryUsage);
    std::free(memoryUsage);

//    Serial.printf("Loading SPIFFS filesystem: \n"
//                  "Capacity: %lu\n"
//                  "Used: %lu\n"
//                  "%.2f%% used\n",
//                  total, used, free);
}

void saveConfig() {
    File file = SPIFFS.open("/config.json", "w");
    if (!file) {
        Serial.println("couldn't open file");
        return;
    }

    serializeJsonPretty(doc, file);
    file.close();
}

void loadConf() {
    File file = SPIFFS.open("/config.json", "r");
    if (!file) {
        Serial.println("couldn't open file");
        return;
    }

    DeserializationError error = deserializeJson(doc, file);
    file.close();
    if (error) {
        Serial.println("error loading json file");
        Serial.println(error.c_str());
        return;
    }
}
