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

namespace vortice {

namespace sensors {

namespace vane {

AMS_5600 ams_5600;

static bool ams_ready = false;

void start() {
  if (ams_5600.detectMagnet() == 0) {
    for (int i = 0; i < 10; i++) {
      if (ams_5600.detectMagnet() == 1) {
        Vortice::printDiagnostic("AS5600", String(ams_5600.getMagnitude()));
        break;
      }

      Vortice::printDiagnostic("AS5600", "PENDING");
      delay(1000);
    }
  }
}

String getAngle() {
  if (ams_5600.detectMagnet() == 0) {
    Vortice::printDiagnostic("AS5600", "DOWN", "Failure to read sensor");
    return "";
  }

  word raw_angle = ams_5600.getRawAngle();
  uint8_t average_direction = uint8_t(raw_angle * 8 / 4096) + 1;
  String val;
  val += ",wind_direction_min=" +
         String(average_direction); // Legacy to be removed in next iteration
  val += ",wind_direction_max=" +
         String(average_direction); // Legacy to be removed in next iteration
  val += ",wind_direction_average=" + String(average_direction);

  Serial.printf("wind_direction: %d\n", average_direction);

  return val;
}

} // namespace vane

} // namespace sensors

} // namespace vortice
