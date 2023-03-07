//
// Created by lazt on 4/23/21.
//

#include "Monitor/realTimeClock.h"
#include "utils/utils.h"

RTC_DS1307 RTC;


void timeStart() {
    const char *rtcLabel = "RTC";
    RTC.begin();
    if (!RTC.isrunning()) {
        Vortice::printDiagnostic(rtcLabel, "NOT RUNNING");
    } else {
        Vortice::printDiagnostic(rtcLabel, Vortice::Status[Vortice::OK]);
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
