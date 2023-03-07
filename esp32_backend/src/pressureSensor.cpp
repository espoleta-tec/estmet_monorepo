//
// Created by lazt on 4/23/21.
//

#include "Monitor/pressureSensor.h"
#include "Adafruit_BMP280.h"
#include "utils/utils.h"

Adafruit_BMP280 bmp;

const double PRESSURE_B0 = -1;
const double PRESSURE_B1 = 1;

bool ok = false;

void pressureStart() {
    const char *bmp280Label = "Pressure (BMP280)";

    if (!bmp.begin()) {
        Vortice::printDiagnostic(bmp280Label,
                                 Vortice::Status[Vortice::FAILED_TO_START],
                                 "Could not find a valid BMP280");
        return;
    } else {
        Vortice::printDiagnostic(bmp280Label,
                                 Vortice::Status[Vortice::OK]);
    }
    ok = true;
}

String pressureRead() {
    String vars = "";
    if (ok) {
        auto temperature_c = bmp.readTemperature();

        auto pressure = PRESSURE_B0 + PRESSURE_B1 * bmp.readPressure();
        vars += ",pressure=" + String(pressure);
        Serial.print("Pressure = ");
        Serial.print(pressure);
        Serial.print(" Pa ");

        // Calculate altitude assuming 'standard' barometric
        // pressure of 1013.25 millibar = 101325 Pascal
        auto altitude = bmp.readAltitude();
        vars += ",altitude=" + String(altitude);
        Serial.print("Altitude = ");
        Serial.print(altitude);
        Serial.print(" meters ");

        auto seaLevelPressure = bmp.readPressure();
        vars += ",pressure_sl=" + String(seaLevelPressure);
        Serial.print("Pressure at sealevel (calculated) = ");
        Serial.print(seaLevelPressure);
        Serial.println(" Pa ");
    }
    return vars;
}
