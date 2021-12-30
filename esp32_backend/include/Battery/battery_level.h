//
// Created by lazaroofarrill on 12/30/21.
//

#ifndef ESP32_BACKEND_BATTERY_LEVEL_H
#define ESP32_BACKEND_BATTERY_LEVEL_H

#include "Arduino.h"

#define RESOLUTION  4096
#define M_VOL  3.3f
#define MAX_BATTERY  0.40/M_VOL*RESOLUTION

#define MIN_BATTERY  0.3/M_VOL*RESOLUTION


unsigned long getBatteryLevel();

String getBatteryLevelString();

#endif //ESP32_BACKEND_BATTERY_LEVEL_H
