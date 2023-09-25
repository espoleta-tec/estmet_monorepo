//
// Created by lazt on 4/23/21.
//

#include "WebServer/webServerProcess.h"
#include "HardwareSerial.h"
#include "esp32-hal-gpio.h"
#include "pinout.h"
#include "utils/utils.h"
#include <FileSystem/fileSystem.h>
#include <Logger/logger.h>
#include <WebServer/connection.h>

TaskHandle_t serverTask;
TaskHandle_t resetTask;

boolean supplyState = 0;

void serverSetup() {
    initWifi();
    initWebServer();
    startLogs();

    xTaskCreatePinnedToCore(serverLoop, "webServer", 50000, NULL, 1, &serverTask,
                            1);
}

[[noreturn]] void serverLoop(void *param) {
    while (true) {
        server.handleClient();
        ws.loop();
        sessionWatch();
        delay(500);
    }
}

void resetSetup() {
    pinMode(RESET_PIN, INPUT);
    Vortice::printDiagnostic("RESET PIN",
                             digitalRead(RESET_PIN) ? "HIGH" : "LOW");
    xTaskCreatePinnedToCore(resetLoop, "resetTask", 8192, nullptr, 1, &resetTask,
                            1);
}

void resetLoop(void *) {
    while (true) {
        //        if (supplyState != digitalRead(POWER_PIN)) {
        //            ESP.restart();
        //        }

        if (!digitalRead(RESET_PIN)) {
            int i;
            for (i = 0; i < 10; i++) {
                delay(500);
                if (digitalRead(RESET_PIN)) {
                    break;
                }
            }
            if (i == 10) {
                Serial.println("Performing reset");
                triggerReset();
            }
        }
        delay(10);
    }
}
