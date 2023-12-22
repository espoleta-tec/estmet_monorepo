#include "Monitor/monitorProcess.h"
#include "Arduino.h"
#include "FileSystem/fileSystem.h"
#include "Logger/logger.h"
#include "Monitor/anemometer.h"
#include "Monitor/lightningDetector.h"
#include "Monitor/vane.h"
#include "WString.h"
#include "esp32-hal.h"
#include "utils/utils.h"
#include <Battery/battery_level.h>
#include <map>
#include <string>

using namespace Vortice;

TaskHandle_t monitorTask = nullptr;
u_long LOG_INTERVAL = 5 * 1000;

void monitorSetup() {
  LOG_INTERVAL = (long)getNumberVal(doc["sensors"]["readFreq"]) * 60 * 1000;
  Vortice::printDiagnostic("LOG INTERVALS", String(LOG_INTERVAL));

#ifdef USE_HUMIDITY
  humidityStart();
#endif

#ifdef USE_VANE
  Vortice::sensors::vane::start();
#endif
#ifdef USE_LIGHT
  lightStart(); // Don't start light
#endif
#ifdef USE_LIGHTNING
  startLightning();
#endif
#ifdef USE_PRESSURE
  vortice::sensors::pressure_start();
#endif
#ifdef USE_TIME
  timeStart();
#endif
  xTaskCreatePinnedToCore(monitorLoop, "MONITOR_TSK", 50000, NULL, 1,
                          &monitorTask, 1);
}

static bool f_ReadInProgress = false;
String lastReading = "";

String getReadings() {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantConditionsOC"
  if (f_ReadInProgress) {
    return lastReading;
  }
#pragma clang diagnostic pop
  f_ReadInProgress = true; // Prevent concurrent calls in here
  String data = "";
#ifdef USE_LIGHT
  data += lightRead(); // Light disabled
#endif

#ifdef USE_PRESSURE
  data += vortice::sensors::pressure_read();
#endif

#ifdef USE_HUMIDITY
  data += read_humidity();
#endif
  data += nano::get_wind_speed();
#ifdef USE_VANE
  data += Vortice::sensors::vane::getAngle();
#endif

#ifdef ARDUINO_CONNECTED
#ifdef USE_PLUVIOMETER
  data += nano::getWaterCount();
#endif

#ifdef USE_LIGHTNING
  data += nano::getLightnings();
#endif
  nano::deleteBuffer();
#endif

  //        data += getBatteryLevelString(); //TODO change pin
#ifdef USE_TIME
  data += timeRead();
#endif
  f_ReadInProgress = false;
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

#ifdef DEBUG
    Serial.println(data);
#endif

    logData(data);

    delay(5e3);
    //        if (batterySavingActivated && !digitalRead(POWER_PIN)) {
    //            esp_deep_sleep(LOG_INTERVAL * 1000);
    //        } else {
    //            vTaskDelayUntil(&xLastWakeTime, LOG_INTERVAL /
    //            portTICK_RATE_MS); xLastWakeTime = xTaskGetTickCount();
    //        }
  }
}
