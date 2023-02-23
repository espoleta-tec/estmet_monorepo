#include "Arduino.h"

#include <SPI.h>
#include <Wire.h>
#include "SparkFun_AS3935.h"
#include "i2cScanner.h"

#define INDOOR 0x12
#define OUTDOOR 0xE
#define LIGHTNING_INT 0x08
#define DISTURBER_INT 0x04
#define NOISE_INT 0x01

SparkFun_AS3935 lightning = SparkFun_AS3935(defAddr);

// Interrupt pin for lightning detection
const int lightningInt = 2;
int spiCS = 5; //SPI chip select pin

// This variable holds the number representing the lightning or non-lightning
// event issued by the lightning detector.
int intVal = 0;
int noise = 2; // Value between 1-7
int disturber = 2; // Value between 1-10

void setup() {
    // When lightning is detected the interrupt pin goes HIGH.
    pinMode(lightningInt, INPUT);
    Wire.begin();

    Serial.begin(115200);

    i2cScan();

    Serial.println("AS3935 Franklin Lightning Detector");

    if (lightning.begin()) {
        Serial.println("Lightning detector started");
    } else {
        Serial.println("Fail to start lightning detector");
    }

    // The lightning detector defaults to an indoor setting at
    // the cost of less sensitivity, if you plan on using this outdoors
    // uncomment the following line:
    lightning.setIndoorOutdoor(OUTDOOR);
}

void loop() {
    // Hardware has alerted us to an event, now we read the interrupt register
    if (digitalRead(lightningInt) == HIGH) {
        intVal = lightning.readInterruptReg();
        if (intVal == NOISE_INT) {
            Serial.println("Noise.");
            // Too much noise? Uncomment the code below, a higher number means better
            // noise rejection.
            //lightning.setNoiseLevel(noise);
        } else if (intVal == DISTURBER_INT) {
            Serial.println("Disturber.");
            // Too many disturbers? Uncomment the code below, a higher number means better
            // disturber rejection.
            //lightning.watchdogThreshold(disturber);
        } else if (intVal == LIGHTNING_INT) {
            Serial.println("Lightning Strike Detected!");
            // Lightning! Now how far away is it? Distance estimation takes into
            // account any previously seen events in the last 15 seconds.
            byte distance = lightning.distanceToStorm();
            Serial.print("Approximately: ");
            Serial.print(distance);
            Serial.println("km away!");
        }
    }
    delay(100); // Slow it down.
}

