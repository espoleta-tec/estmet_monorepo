//
// Created by lazt on 4/23/21.
//

#ifndef BACKEND_MONITORPROCESS_H
#define BACKEND_MONITORPROCESS_H

#include "DHTesp.h"
#include "Logger/logger.h"
#include "humiditySensor.h"
#include "lightSensor.h"
#include "pressureSensor.h"
#include "realTimeClock.h"

extern TaskHandle_t monitorTask;

typedef struct TScanData {
    vortice::sensors::TPressureSensorData pressure_data;
    TempAndHumidity humidity_data;
} TScanData;

void monitorSetup();

String getReadings();

[[noreturn]] void monitorLoop(void *param);

[[noreturn]] void lightningLoop(void *param);

#endif // BACKEND_MONITORPROCESS_H
