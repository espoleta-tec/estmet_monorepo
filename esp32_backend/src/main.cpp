#include <Wire.h>
#include "Arduino.h"
#include "WebServer/webServerProcess.h"
#include "Monitor/monitorProcess.h"
#include "pinout.h"


void setup() {
    Serial.begin(9600);
    if (!Wire.begin()) {
        Serial.println("Failed to init i2c interface");
    }
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LOGO_LED, OUTPUT);
    pinMode(PRO_MINI_RESET, OUTPUT);


    Serial.println("update 0");
    initFileSystem();
    loadConf();
    initSDCard();
    supplyState = digitalRead(POWER_PIN);
    if (supplyState || true) {
        serverSetup();
        digitalWrite(LOGO_LED, LOW);
    } else {
        digitalWrite(LOGO_LED, HIGH);
    }
//    resetSetup();

    digitalWrite(PRO_MINI_RESET, LOW);
    delay(50);
    digitalWrite(PRO_MINI_RESET, HIGH);
    monitorSetup();
}

void loop() {
//    Serial.print(digitalRead(POWER_PIN));
//    Serial.print(" ");
//    Serial.println(digitalRead(LOGO_LED));
    delay(1000000);
}
