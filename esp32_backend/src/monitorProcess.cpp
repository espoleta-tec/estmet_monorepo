#include <sys/cdefs.h>
#include <Battery/battery_level.h>
#include "Monitor/monitorProcess.h"
#include "Arduino.h"
#include "FreeRTOS.h"
#include "FileSystem/fileSystem.h"
#include "Monitor/anemometer.h"
#include "pinout.h"

TaskHandle_t monitorTask = NULL;
TaskHandle_t lightningTask = NULL;
u_long LOG_INTERVAL = 0.5 * 60 * 1000;

int lightningFlag = 0;
unsigned long lightningCounter = 0;

void monitorSetup() {
    LOG_INTERVAL = (long) getNumberVal(doc["sensors"]["readFreq"]) * 60 * 1000;
    Serial.print("Read freq is");
    Serial.println(LOG_INTERVAL);
    humidityStart();
    lightStart();
    pressureStart();
    timeStart();
    xTaskCreatePinnedToCore(monitorLoop, "MONITOR_TSK", 50000,
                            NULL, 1, &monitorTask, 1);
//    xTaskCreatePinnedToCore(lightningLoop, "LIGHTNING_TSK", 8192,
//                            NULL, 1, &lightningTask, 1);
}

[[noreturn]] void monitorLoop(void *param) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        String data = "";
        data += humidityRead();
        data += lightRead();
        data += pressureRead();
        data += anem::getWindValues();
        data += anem::getWaterCount();
        data += anem::getLightnings();
//        data += ",lightningCount=" + String(lightningCounter);
//        Serial.println(",lightningCount=" + String(lightningCounter));
//        lightningCounter = 0;

//        data += anem::getLightnings();
        data += getBatteryLevelString();
        anem::dele();


        data += timeRead();
        logData(data);
        Serial.println(millis() / 1000);
        delay(30e3);
//        if (batterySavingActivated && !digitalRead(POWER_PIN)) {
//            esp_deep_sleep(LOG_INTERVAL * 1000);
//        } else {
//            vTaskDelayUntil(&xLastWakeTime, LOG_INTERVAL / portTICK_RATE_MS);
//            xLastWakeTime = xTaskGetTickCount();
//        }
    }
}

void lightningLoop(void *param) {
    while (true) {
        if (digitalRead(LIGHTNING_PIN) == 1) {
            if (lightningFlag == 0) {
                lightningFlag = 1;
            }
        }
        if (digitalRead(LIGHTNING_PIN) == 0) {
            if (lightningFlag == 1) {
                lightningFlag = 0;
                lightningCounter++;
            }
        }
        delay(1);
    }
}
