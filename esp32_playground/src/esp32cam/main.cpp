//
// Created by Lazaro O'Farrill on 23/02/2023.
//
#include "Arduino.h"

void setup() {
    pinMode(33, OUTPUT);
}

void loop() {
    digitalWrite(33, HIGH);
    delay(1000);
    digitalWrite(33, LOW);
    delay(1000);
}