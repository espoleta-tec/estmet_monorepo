#include <Wire.h>
#include <Battery/batterySaving.h>
#include "Arduino.h"
#include "WebServer/webServerProcess.h"
#include "Monitor/monitorProcess.h"


void setup() {
    Serial.begin(115200);
    if (!Wire.begin()) {
        Serial.println("Failed to init i2c interface");
    }
    pinMode(LED_BUILTIN, OUTPUT);


    Serial.println("update 0");
    initFileSystem();
    loadConf();
    if ((!batterySavingActivated && digitalRead(POWER_PIN)) || true) {
//        serverSetup();
    }
    monitorSetup();
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(10);
    digitalWrite(LED_BUILTIN, LOW);
    delay(10);
}