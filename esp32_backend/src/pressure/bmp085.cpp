//
// Created by lazt on 4/23/21.
//

#include <WString.h>
#include "Monitor/pressureSensor.h"

#ifdef USE_BMP_085

#include "Adafruit_BMP085.h"
#include "utils/utils.h"

namespace vortice {
namespace sensors {

Adafruit_BMP085 bmp;

const double PRESSURE_B0 = -1;
const double PRESSURE_B1 = 1;

bool ok = false;

void pressure_start() {
    const char *bmp085Label = "Pressure (BMP085)";

    if (!bmp.begin()) {
        Vortice::printDiagnostic(bmp085Label,
                                 Vortice::Status[Vortice::FAILED_TO_START],
                                 "Could not find a valid BMP085");
        return;
    } else {
        Vortice::printDiagnostic(bmp085Label,
                                 Vortice::Status[Vortice::OK]);
    }
    ok = true;
}

struct TPressureSensorData pressure_get() {
    if (__sensor_enabled) {
        auto pressure = PRESSURE_B0 + PRESSURE_B1 * bmp.readPressure();
        float pressure_in_mercury_mm = (float) pressure * MERCURY_MM_X_PASCAL_FACTOR;
        auto altitude = bmp.readAltitude();

        return {.error_code = 0,
                .pressure = (float) pressure,
                .pressure_mercury = pressure_in_mercury_mm,
                .altitude = altitude};

    } else {
        return {.error_code =1};
    }
}
}
}

#endif