//
// Created by lazt on 4/23/21.
//

#include "Logger/logger.h"

void startLogs() {
    if (!cardMounted) return;
    if (!SD.mkdir("/logs")) {
        Serial.println("couldn't create logs directory");
    };
}

void logData(String data) {
    ws.broadcastTXT(data);

    DateTime now = RTC.now();
    String todayLogsPath = "/logs/";
    todayLogsPath += now.year();
    todayLogsPath += "-";
    todayLogsPath += now.month();
    todayLogsPath += "-";
    todayLogsPath += now.day();

    String globalLogsPath = "/logs/global";

    File todayLogs = SD.open(todayLogsPath, FILE_APPEND);
    if (!todayLogs) {
        Serial.println("couldn't open file for writing");
        return;
    }

    File allLogs = SD.open(globalLogsPath, FILE_APPEND);
    if (!todayLogs || !allLogs) {
        Serial.println("couldn't open file for writing");
        return;
    }
    todayLogs.println(data);
    allLogs.println(data);
    Serial.println("data logged successfully");
    todayLogs.close();
    allLogs.close();
}

