//
// Created by Lazaro O'Farrill on 23/02/2023.
//

#include "Arduino.h"

#define LED 13

void setup() {
    pinMode(LED, OUTPUT);
}

void loop() {
    digitalWrite(LED, HIGH);
    delay(1000);

    digitalWrite(LED, LOW);
    delay(1000);
}