//
// Created by lazt on 4/23/21.
//

#include "Monitor/pressureSensor.h"

#ifdef USE_BMP_280
#define MERCURY_MM_X_PASCAL_FACTOR 0.00750062f

#include "Adafruit_BMP280.h"
#include "utils/utils.h"

Adafruit_BMP280 bmp;

namespace vortice {
namespace sensors {

const float PRESSURE_B0 = -1;
const float PRESSURE_B1 = 1;

void pressure_start() {
    const char *bmp280Label = "Pressure (BMP280)";

    if (!bmp.begin()) {
        Vortice::printDiagnostic(bmp280Label,
                                 Vortice::Status[Vortice::FAILED_TO_START],
                                 "Could not find a valid BMP280");
        return;
    } else {
        Vortice::printDiagnostic(bmp280Label, Vortice::Status[Vortice::OK]);
    }
    __sensor_enabled = true;
}

struct TPressureSensorData pressure_get() {
    if (__sensor_enabled) {
        auto pressure = PRESSURE_B0 + PRESSURE_B1 * bmp.readPressure();
        float pressure_in_mercury_mm = pressure * MERCURY_MM_X_PASCAL_FACTOR;
        auto altitude = bmp.readAltitude();

        return {.error_code = 0,
                .pressure = pressure,
                .pressure_mercury = pressure_in_mercury_mm,
                .altitude = altitude};

    } else {
        return {.error_code =1};
    }
}
}
}


#endif
