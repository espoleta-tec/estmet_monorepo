//
// Created by lazaroofarrill on 10/22/21.
//

#include <pinout.h>
#include "Monitor/anemometer.h"
#include "math.h"
#include "HardwareSerial.h"

#define TIMEOUT 1000

#define TIME_GAP 5

int pulses;
int lastread = 0;

#define MILLIS_TO_SECOND 1000
#define READING_TIME 5
#define ANEMOMETER_RADIUS 0.06

//void anem::countPulse() {
//    if (millis() - lastread >= 200) {
//        pulses++;
//        lastread = millis();
//    }
//}
//
//void anem::startReading() {
//    pulses = 0;
//    attachInterrupt(digitalPinToInterrupt(ANEMOMETER_PIN), countPulse, FALLING);
//    delay(READING_TIME * MILLIS_TO_SECOND);
//    finishReading();
//}
//
//void anem::finishReading() {
//    detachInterrupt(ANEMOMETER_PIN);
//    double w = pulses / READING_TIME;
//    double linearSpeed = w * PI * ANEMOMETER_RADIUS;
//    Serial.printf("pulses: %d\n", pulses);
//    Serial.printf("linearspeed: %f\n", linearSpeed);
//}
//
//void anem::anemometerStart() {
//    pinMode(ANEMOMETER_PIN, INPUT_PULLUP);
//}

void anem::anemometerStart() {
    Serial2.begin(9600);
}

String anem::startReading() {
    String message = "", data = "";
    ulong timestamp = millis();
    Serial2.print('s');
    while (!Serial2.available() && (millis() - timestamp < TIMEOUT)) {

    }
    if (Serial2.available()) {
        timestamp = millis();
        while ((message.indexOf("end") == -1) && (millis() - timestamp < TIMEOUT)) {
            if (Serial2.available()) {
                message = Serial2.readStringUntil('\r');
                if (message.toInt()) {
                    pulses = message.toInt();
                    Serial.printf("%d pulses where counted\n", pulses);

                    double w = (double) pulses / (double) READING_TIME;
                    double linearSpeed = w * PI * ANEMOMETER_RADIUS;
                    Serial.printf("linearspeed: %f\n", linearSpeed);
                    data = "windSpeed=" + String(linearSpeed);
                }
                Serial.println(message);
            }
        }
    }
    return data;
}