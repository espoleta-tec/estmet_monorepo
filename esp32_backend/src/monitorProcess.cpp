#include "Monitor/monitorProcess.h"
#include "Arduino.h"
#include "FileSystem/fileSystem.h"
#include "Monitor/anemometer.h"
#include "Monitor/lightningDetector.h"
#include "Monitor/vane.h"
#include "utils/utils.h"
#include <Battery/battery_level.h>

using namespace Vortice;

TaskHandle_t monitorTask = nullptr;
u_long LOG_INTERVAL = 5 * 1000;

void monitorSetup() {
    LOG_INTERVAL = (long) getNumberVal(doc["sensors"]["readFreq"]) * 60 * 1000;
    Vortice::printDiagnostic("LOG INTERVALS", String(LOG_INTERVAL));

    humidityStart();
//  vortice::sensors::vane::start();
    // lightStart(); //Don't start light
    startLightning();
    pressureStart();
    timeStart();
    xTaskCreatePinnedToCore(monitorLoop, "MONITOR_TSK", 50000, NULL, 1,
                            &monitorTask, 1);
}

[[noreturn]] void monitorLoop(void *param) {
    while (true) {
        String data = "";
//         data += lightRead(); //Light disabled
        data += pressureRead();
        data += humidityRead();
        data += nano::getWindValues();
//       data += Vortice::sensors::vane::getAngle();
        data += nano::getWaterCount();
        data += nano::getLightnings();
        data += getBatteryLevelString();
        nano::deleteBuffer();

        data += timeRead();
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
