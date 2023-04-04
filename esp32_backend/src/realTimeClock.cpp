//
// Created by lazt on 4/23/21.
//

#include "Monitor/realTimeClock.h"
#include "utils/utils.h"

RTC_DS1307 RTC;


void timeStart() {
    const char *rtcLabel = "RTC";


    if (!RTC.begin()) {
        Vortice::printDiagnostic(rtcLabel, "NOT RUNNING");
    } else {
        Vortice::printDiagnostic(rtcLabel, Vortice::Status[Vortice::OK]);
        uint8_t status = RTC.readnvram(0);
        status = status & 0x7F;
        RTC.writenvram(0, status);
    }
}

String timeRead() {
    String vars = "";
    DateTime now = RTC.now();
    vars += now.year();
    vars += "/";
    vars += now.month();
    vars += "/";
    vars += now.day();
    vars += " ";
    vars += now.hour();
    vars += ":";
    vars += now.minute();
    vars += ":";
    vars += now.second();
    Serial.println(vars);
    Serial.println();

    vars = ",date=\"" + vars + "\"";
    return vars;
}
