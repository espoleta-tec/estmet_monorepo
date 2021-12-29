//
// Created by lazt on 4/23/21.
//

#include <FileSystem/fileSystem.h>
#include <WebServer/connection.h>
#include <Logger/logger.h>
#include "WebServer/webServerProcess.h"

#define RESET_PIN 1

TaskHandle_t serverTask;
TaskHandle_t resetTask;

void serverSetup() {
    initWifi();
    if (USE_SDCARD) {
        initSDCard();
    }
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
    xTaskCreatePinnedToCore(
            resetLoop,
            "resetTask",
            512,
            nullptr,
            1,
            &resetTask,
            1
    );
}

void resetLoop(void *) {
    while (true) {
        if (digitalRead(RESET_PIN)) {
            int i;
            for (i = 1; i <= 10; i++) {
                delay(500);
                if (!digitalRead(RESET_PIN)) {
                    break;
                }
            }
            if (i == 10) {
                resetConfig();
            }
        }
    }
}
