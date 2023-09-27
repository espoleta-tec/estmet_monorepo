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
  vars += "-";
  vars += now.month();
  vars += "-";
  if (now.day() < 10) {
    vars += "0";
  }
  vars += now.day();
  vars += " ";

  if (now.hour() < 10) {
    vars += "0";
  }
  vars += now.hour();
  vars += ":";

  if (now.minute() < 10) {
    vars += "0";
  }
  vars += now.minute();
  vars += ":";

  if (now.second() < 10) {
    vars += "0";
  }
  vars += now.second();
  vars += "Z";

  vars = ",date=\"" + vars + "\"";
  return vars;
}
