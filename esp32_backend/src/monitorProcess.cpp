#include <sys/cdefs.h>
#include <Battery/battery_level.h>
#include "Monitor/monitorProcess.h"
#include "Arduino.h"
#include "FileSystem/fileSystem.h"
#include "Monitor/anemometer.h"
#include "Monitor/lightningDetector.h"
#include "utils/utils.h"

using namespace Vortice;

TaskHandle_t monitorTask = NULL;
u_long LOG_INTERVAL = 5 * 1000;


void monitorSetup() {
    LOG_INTERVAL = (long) getNumberVal(doc["sensors"]["readFreq"]) * 60 * 1000;
    Vortice::printDiagnostic("LOG INTERVALS", String(LOG_INTERVAL));

    humidityStart();
    lightStart();
    startLightning();
    pressureStart();
    timeStart();
    xTaskCreatePinnedToCore(monitorLoop, "MONITOR_TSK", 50000,
                            NULL, 1, &monitorTask, 1);
}

[[noreturn]] void monitorLoop(void *param) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        String data = "";
        data += lightRead();
        data += pressureRead();
        data += humidityRead();
        data += nano::getWindValues();
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
//            vTaskDelayUntil(&xLastWakeTime, LOG_INTERVAL / portTICK_RATE_MS);
//            xLastWakeTime = xTaskGetTickCount();
//        }
    }
}
