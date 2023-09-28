//
// Created by lazt on 4/23/21.
//

#include "Monitor/realTimeClock.h"
#include "sys/_stdint.h"
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

String left_pad(int val, uint16_t min_length, char character_to_fill = '0') {
  String newVal = "";
  int difference = min_length - String(val).length();

  if (difference > 0) {
    for (int i = 0; i < difference; i++) {
      newVal += character_to_fill;
    }
  }

  newVal += val;

  return newVal;
}

String timeRead() {
  if (!RTC.isrunning()) {
    return ",date=ERROR";
  }

  String vars = "";
  DateTime now = RTC.now();
  vars += left_pad(now.year(), 2);
  vars += "-";
  vars += left_pad(now.month(), 2);
  vars += "-";
  vars += left_pad(now.day(), 2);
  vars += "T";
  vars += left_pad(now.hour(), 2);
  vars += ":";
  vars += left_pad(now.minute(), 2);
  vars += ":";
  vars += left_pad(now.second(), 2);
  vars += "Z";

  vars = ",date=\"" + vars + "\"";
  return vars;
}
