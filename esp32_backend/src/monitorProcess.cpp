#include "Monitor/monitorProcess.h"
#include "Arduino.h"
#include "FileSystem/fileSystem.h"
#include "Logger/logger.h"
#include "Monitor/anemometer.h"
#include "Monitor/lightningDetector.h"
#include "Monitor/vane.h"
#include "esp32-hal.h"
#include "utils/utils.h"
#include <Battery/battery_level.h>

using namespace Vortice;

TaskHandle_t monitorTask = nullptr;
u_long LOG_INTERVAL = 5 * 1000;

void monitorSetup() {
  LOG_INTERVAL = (long)getNumberVal(doc["sensors"]["readFreq"]) * 60 * 1000;
  Vortice::printDiagnostic("LOG INTERVALS", String(LOG_INTERVAL));

  humidityStart();
#ifdef USE_VANE
  Vortice::sensors::vane::start();
#endif
  //    lightStart(); //Don't start light
  startLightning();
  pressureStart();
  timeStart();
  xTaskCreatePinnedToCore(monitorLoop, "MONITOR_TSK", 50000, NULL, 1,
                          &monitorTask, 1);
}

static bool ReadingDataFlag = false;
String lastReading = "";
String getReadings() {
  if (ReadingDataFlag) {
    return lastReading;
  }
  ReadingDataFlag = true; // Prevent concurrent calls in here
  String data = "";
  //         data += lightRead(); //Light disabled
  data += read_pressure();
  data += read_humidity();
  data += nano::get_wind_speed();
#ifdef USE_VANE
  data += Vortice::sensors::vane::getAngle();
#endif
  data += nano::getWaterCount();
  data += nano::getLightnings();
  //        data += getBatteryLevelString(); //TODO change pin
  nano::deleteBuffer();

  data += timeRead();
  ReadingDataFlag = false;
  lastReading = data;
  return data;
}

[[noreturn]] void monitorLoop(void *param) {
  while (true) {
    String data = "";

    do {
      data = getReadings();
      vTaskDelay(1);
    } while (data.length() == 0);

    logData(data);

#ifdef DEBUG
    Serial.println(data);
#endif
    delay(5e3);
    //        if (batterySavingActivated && !digitalRead(POWER_PIN)) {
    //            esp_deep_sleep(LOG_INTERVAL * 1000);
    //        } else {
    //            vTaskDelayUntil(&xLastWakeTime, LOG_INTERVAL /
    //            portTICK_RATE_MS); xLastWakeTime = xTaskGetTickCount();
    //        }
  }
}
