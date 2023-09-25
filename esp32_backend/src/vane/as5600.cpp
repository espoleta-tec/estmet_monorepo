#include "AS5600.h"
#include "Arduino.h"
#include "HardwareSerial.h"
#include "Monitor/vane.h"
#include "WString.h"
#include "Wire.h"
#include "esp32-hal.h"
#include "sys/_stdint.h"
#include "utils/utils.h"
#include <string>

namespace Vortice {

namespace sensors {

namespace vane {

AMS_5600 ams_5600;

void start() {
    const char *as5600_label = "AS5600";

    if (ams_5600.detectMagnet() == 0) {
        for (int i = 0; i < 10; i++) {
            if (ams_5600.detectMagnet() == 1) {
                Vortice::printDiagnostic(as5600_label, String(ams_5600.getMagnitude()));
                break;
            }

            Vortice::printDiagnostic(as5600_label, "PENDING");
            delay(1000);
        }
    } else {
        Vortice::printDiagnostic(as5600_label, String(ams_5600.getMagnitude()));
    }
}

String getAngle() {
    if (ams_5600.detectMagnet() == 0) {
        Vortice::printDiagnostic("AS5600", "DOWN", "Failure to read sensor");
        return "";
    }

    word raw_angle = ams_5600.getRawAngle();
    //  uint8_t average_direction = uint8_t(raw_angle * 8 / 4096) + 1;

    Serial.printf("Raw angle is %d\n", raw_angle);
    float average_direction = float(raw_angle) / 4096.0 * 360.0;
    float wind_direction_seconds = average_direction * 0.006;

    String val;
    val += ",wind_direction_min=" +
           String(average_direction); // Legacy to be removed in next iteration
    val += ",wind_direction_max=" +
           String(average_direction); // Legacy to be removed in next iteration
    val += ",wind_direction_average=" + String(average_direction);

    val += ",wind_direction_sec=" +
           String(wind_direction_seconds);

    Serial.printf("wind_direction: %f\n", average_direction);

    return val;
}

} // namespace vane

} // namespace sensors

} // namespace vortice
