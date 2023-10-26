//
// Created by lazt on 4/23/21.
//

#ifndef BACKEND_PRESSURESENSOR_H
#define BACKEND_PRESSURESENSOR_H

#include "Arduino.h"

#define MERCURY_MM_X_PASCAL_FACTOR 0.00750062f


namespace vortice {
namespace sensors {

typedef struct TPressureSensorData {
    int error_code;
    float pressure;
    float pressure_mercury;
    float altitude;
} TPressureSensorData;

void pressure_start();

TPressureSensorData pressure_get();

String pressure_read();

extern bool __sensor_enabled;
}
}

#endif //BACKEND_PRESSURESENSOR_H
