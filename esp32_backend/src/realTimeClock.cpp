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
    }

    if (!RTC.isrunning()) {
        RTC.adjust(DateTime((uint32_t) 0));
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
