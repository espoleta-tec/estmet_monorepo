//
// Created by lazt on 4/23/21.
//

#include <FileSystem/fileSystem.h>
#include "pinout.h"
#include <WebServer/connection.h>
#include <Logger/logger.h>
#include "WebServer/webServerProcess.h"


TaskHandle_t serverTask;
TaskHandle_t resetTask;

boolean supplyState = 0;

void serverSetup() {
    initWifi();
    initWebServer();
    startLogs();

    xTaskCreatePinnedToCore(
            serverLoop,
            "webServer",
            50000,
            NULL,
            1,
            &serverTask,
            1
    );
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
    xTaskCreatePinnedToCore(
            resetLoop,
            "resetTask",
            8192,
            nullptr,
            1,
            &resetTask,
            1
    );
}

void resetLoop(void *) {
    while (true) {
        if (supplyState != digitalRead(POWER_PIN)) {
            ESP.restart();
        }

        if (!digitalRead(RESET_PIN)) {
            int i;
            for (i = 0; i < 10; i++) {
                delay(500);
                if (digitalRead(RESET_PIN)) {
                    break;
                }
            }
            if (i == 10) {
                triggerReset();
            }

        }
        delay(10);
    }
}
