#include <Wire.h>
#include <Battery/batterySaving.h>
#include "Arduino.h"
#include "WebServer/webServerProcess.h"
#include "Monitor/monitorProcess.h"

#define PRO_MINI_RESET 4
#define LOGO_LED 2

#define LED_CHANNEL 0

void setup() {
    Serial.begin(9600);
    if (!Wire.begin()) {
        Serial.println("Failed to init i2c interface");
    }
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LOGO_LED, OUTPUT);
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);


    Serial.println("update 0");
    initFileSystem();
    loadConf();
    resetSetup();
    initSDCard();
    if (digitalRead(POWER_PIN)) {
        serverSetup();
        digitalWrite(LOGO_LED, HIGH);
    }
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