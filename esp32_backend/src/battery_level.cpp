//
// Created by lazaroofarrill on 12/30/21.
//

#include <pinout.h>
#include "Battery/battery_level.h"

unsigned long getBatteryLevel() {
    return analogRead(BATTERY_LEVEL);
}

String getBatteryLevelString() {
    String data = "";
    data += ",battery_level=" + String(getBatteryLevel());
    Serial.println(data);
    return data;
}


