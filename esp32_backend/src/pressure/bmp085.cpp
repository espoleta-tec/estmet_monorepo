//
// Created by lazt on 4/23/21.
//

#include "Monitor/pressureSensor.h"

#ifdef BMP085
#include "Adafruit_BMP085.h"
#include "utils/utils.h"

using namespace Vortice;
Adafruit_BMP085 bmp;

const double PRESSURE_B0 = -1;
const double PRESSURE_B1 = 1;

bool ok = false;

void pressureStart() {
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

String pressureRead() {
    String vars = "";
    if (ok) {
        auto temperature_c = bmp.readTemperature();

        auto pressure = PRESSURE_B0 + PRESSURE_B1 * bmp.readPressure();
        vars += ",pressure=" + String(pressure);

        printDiagnostic("Pressure", String(pressure));

        auto altitude = bmp.readAltitude();
        vars += ",altitude=" + String(altitude);
        printDiagnostic("Altitude", String(altitude));

        auto seaLevelPressure = bmp.readSealevelPressure();
        vars += ",pressure_sl=" + String(seaLevelPressure);
        printDiagnostic("Sea Level Pressure", String(seaLevelPressure));
    }
    return vars;
}
#endif