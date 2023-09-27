//
// Created by lazt on 4/23/21.
//

#include "Monitor/pressureSensor.h"

#ifdef BMP280
#define MERCURY_MM_X_PASCAL_FACTOR 0.00750062f

#include "Adafruit_BMP280.h"
#include "utils/utils.h"

Adafruit_BMP280 bmp;

const float PRESSURE_B0 = -1;
const float PRESSURE_B1 = 1;

bool sensor_enabled = false;

void pressureStart() {
    const char *bmp280Label = "Pressure (BMP280)";

    if (!bmp.begin()) {
        Vortice::printDiagnostic(bmp280Label,
                                 Vortice::Status[Vortice::FAILED_TO_START],
                                 "Could not find a valid BMP280");
        return;
    } else {
        Vortice::printDiagnostic(bmp280Label, Vortice::Status[Vortice::OK]);
    }
    sensor_enabled = true;
}

String read_pressure() {
    String vars = "";
    if (sensor_enabled) {
        struct TPressureSensorData pressure_data = getPressure();
        float pressure = pressure_data.pressure;
        float pressure_mercury = pressure_data.pressure_mercury;
        vars += ",pressure=" + String(pressure);
        vars += ",pressure_mercury_mm=" + String(pressure_mercury);

        // Calculate altitude assuming 'standard' barometric
        // pressure of 1013.25 millibar = 101325 Pascal
        float altitude = pressure_data.altitude;
        vars += ",altitude=" + String(altitude);
    }
    return vars;
}

struct TPressureSensorData getPressure() {
    if (sensor_enabled) {
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

#endif
