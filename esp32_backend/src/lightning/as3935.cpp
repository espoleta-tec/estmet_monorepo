//
// Created by Lazaro O'Farrill on 01/03/2023.
//
#include "Monitor/lightningDetector.h"
#include "SparkFun_AS3935.h"

#define AS3935_ADDR 0x03
#define INDOOR 0x12
#define OUTDOOR 0xE
#define DISTURBER_INT 0x04
#define NOISE_INT 0x01

SparkFun_AS3935 lightning(AS3935_ADDR);

void Vortice::startLightning() {
    if (lightning.begin()) {
        Serial.println("Lightning sensor started correctly.");
    } else { Serial.println("Error starting lightning sensor."); }
}