//
// Created by lazt on 4/23/21.
//

#ifndef BACKEND_PRESSURESENSOR_H
#define BACKEND_PRESSURESENSOR_H

#include "Arduino.h"

typedef struct TPressureSensorData {
    int error_code;
    float pressure;
    float pressure_mercury;
    float altitude;
} TPressureSensorData;

void pressureStart();

TPressureSensorData getPressure();

String pressureRead();

#endif //BACKEND_PRESSURESENSOR_H
