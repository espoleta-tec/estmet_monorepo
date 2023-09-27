//
// Created by lazt on 4/23/21.
//

#include "Monitor/realTimeClock.h"
#include "utils/utils.h"

RTC_DS1307 RTC;

void timeStart() {
  const char *rtcLabel = "RTC";

  if (!RTC.begin(&Wire1)) {
    Vortice::printDiagnostic(rtcLabel, "DISCONNECTED");
  } else {
    if (!RTC.isrunning()) {
      Vortice::printDiagnostic(rtcLabel, "NOT RUNNING");
      RTC.adjust(DateTime("1971-01-01"));
    } else {
      Vortice::printDiagnostic(rtcLabel, Vortice::Status[Vortice::OK]);
    }
    //        uint8_t status = RTC.readnvram(0);
    //        status = status & 0x7F; // enable oscillator bit
    //        RTC.writenvram(0, status);
  }
}

String timeRead() {
  if (!RTC.isrunning()) {
    return ",date=ERROR";
  }
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

  vars = ",date=\"" + vars + "\"";
  return vars;
}
