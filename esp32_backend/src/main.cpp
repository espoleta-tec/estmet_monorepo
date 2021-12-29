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
//    ledcSetup(LED_CHANNEL, 5000, 8);

//    ledcAttachPin(LOGO_LED, LED_CHANNEL);
    digitalWrite(LOGO_LED, HIGH);


    Serial.println("update 0");
    initFileSystem();
    loadConf();
    resetSetup();
    if ((!batterySavingActivated && digitalRead(POWER_PIN)) || true) {
        serverSetup();
//        ledcWrite(LED_CHANNEL, int((float) 2 / (float) 5 * 255));
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