//
// Created by Lazaro O'Farrill on 26/10/2023.
//

#ifdef USE_PRESSURE

#include "Monitor/monitorProcess.h"

namespace vortice {
namespace sensors {
bool __sensor_enabled = false;

String pressure_read() {
    String vars = "";
    if (__sensor_enabled) {
        struct TPressureSensorData pressure_data = pressure_get();
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
}
}

#endif