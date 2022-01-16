#include <Wire.h>
#include <Battery/batterySaving.h>
#include "Arduino.h"
#include "WebServer/webServerProcess.h"
#include "Monitor/monitorProcess.h"
#include "pinout.h"


#define LED_CHANNEL 0

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
    }
//    resetSetup();
    digitalWrite(PRO_MINI_RESET, LOW);
    delay(50);
    digitalWrite(PRO_MINI_RESET, HIGH);
    monitorSetup();
}

void loop() {
//    digitalWrite(LED_BUILTIN, HIGH);
//    digitalWrite(4, HIGH);
//    delay(50);
//    digitalWrite(LED_BUILTIN, LOW);
//    digitalWrite(4, LOW);
//    delay(50);
//    humidityRead();
//    delay(1000);
}