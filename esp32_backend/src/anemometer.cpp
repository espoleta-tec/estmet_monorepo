
#include <Wire.h>
#include "Arduino.h"
#include "Monitor/anemometer.h"



/**
 * @brief commands for send to ATTiny85 slave i2c
 *
 */
#define BURST_DATA  0x23
#define PLUVIO_ACC  0x24
#define AVAILABLE   0x01
#define DELETE_BUF  0xAA

#define LAST_POS    0x21

#define LIGHT_DATA  0xC0

#define MAX_BUFFER_DATA 12

#define ANEMOMETER_RADIUS 0.060


const byte SLAVE_ADDR = 0x04;

const byte NUM_BYTES = 1;

byte data[NUM_BYTES] = {0};

byte bytesReceived = 0;

unsigned long timeNow = millis();

int a = 0;

uint8_t lightningCount;
uint16_t waterCount;


uint8_t readingsBuffer[400] = {0};


[[noreturn]] void tickerTask(void *param) {
    while (true) {
        digitalWrite(2, !digitalRead(2));
        digitalWrite(4, !digitalRead(4));
        delay(50);
    }
}


//void loop() {
//
////    readBurst();
////    getCursor();
//    if (avail() == 1) {
//
//        readBuff();
//    }
//
//    //if(a>=5)dele();
//    delay(3000);
//    //a++;
//    //Serial.println("KK");
//}

uint16_t anem::getCursor() {
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(LAST_POS);
    Wire.endTransmission();

    Wire.requestFrom(SLAVE_ADDR, 2u);

    uint8_t smallEnd, bigEnd;
    smallEnd = Wire.read();
    bigEnd = Wire.read();
//    Serial.print(Wire.read(), HEX);
//    Serial.print("  ");
//    Serial.print(Wire.read(), HEX);
//    Serial.print(smallEnd);
//    Serial.print(bigEnd);
    return bigEnd << 8 | smallEnd;
}

void anem::readBurst() {
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(BURST_DATA);
    Wire.endTransmission();

    Wire.requestFrom(SLAVE_ADDR, 5u);

    uint8_t smallEnd, bigEnd;
    Serial.print(Wire.read(), HEX);
    Serial.print("  ");
    Serial.print(Wire.read(), HEX);
    Serial.print("  ");
    smallEnd = Wire.read();
    Serial.print(smallEnd, HEX);
    Serial.print("  ");
    bigEnd = Wire.read();
    Serial.print(bigEnd, HEX);
    Serial.print("  ");
    waterCount = bigEnd << 8 | smallEnd;
    Serial.print(waterCount);
    Serial.print(" ");
    lightningCount = Wire.read();
    Serial.print(lightningCount, HEX);
    Serial.println();
}

uint8_t anem::avail() {
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(AVAILABLE);
    Wire.endTransmission();

    Wire.requestFrom(SLAVE_ADDR, 1u);

    uint8_t mm = Wire.read();
    //Serial.println(mm ,HEX);

    return mm;
}

void anem::dele() {
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(DELETE_BUF);
    Wire.endTransmission();

    Wire.requestFrom(SLAVE_ADDR, 1u);

    Serial.println(Wire.read(), HEX);
    for (unsigned char &i: readingsBuffer) {
        i = 0;
    }
}

void anem::readBuff() {
    for (uint8_t j = 0; j < 30; j++) {

        Wire.beginTransmission(SLAVE_ADDR);
        Wire.write(0xC0 + j);
        Wire.endTransmission();

        Wire.requestFrom(SLAVE_ADDR, 12u);


        for (uint8_t i = 0; i < 12; i++) {
            readingsBuffer[12 * j + i] = Wire.read();
            if (i % 8 == 0) {
                Serial.println(" ");
                Serial.print(j);
                Serial.print("--");
            } else Serial.print(" ");
            Serial.print(readingsBuffer[12 * j + i]);
        }

    }
//    for (int i = 0; i < 360; i++) {
//        Serial.printf("%d\t", readingsBuffer[i]);
//    }
    //Serial.print(readingsBuffer[10]);
    Serial.println(" ");
    //Serial.print(readingsBuffer[0xBF]);
    Serial.println("------");

}

String anem::getWindValues() {
    String val = "";
    readBuff();

    double average, reducer;
    uint8_t min = 255, max = 0;
    uint16_t mostCommonDirections[8] = {0}, averageDirection = 0, maxDirectionCount = 0, reducerCount = 0;
    byte minDir = -1, maxDir = -1;

    reducer = 0;
    reducerCount = 0;
    for (int i = 0; i < 360; i += 2) {

        if (readingsBuffer[i] == 0xFF) continue;
        if (readingsBuffer[i] > max) {
            max = readingsBuffer[i];
            maxDir = readingsBuffer[i + 1];
            Serial.print("new max: ");
            Serial.print(readingsBuffer[i]);
            Serial.print("position: ");
            Serial.print(i);
            Serial.println();
        }
        if (readingsBuffer[i] < min) {
            min = readingsBuffer[i];
            minDir = readingsBuffer[i + 1];
        }
        reducer += readingsBuffer[i];
        reducerCount++;
        mostCommonDirections[readingsBuffer[i + 1] - 1]++;
    }
    for (int i = 0; i < sizeof(mostCommonDirections) / sizeof(*mostCommonDirections); i++) {
        Serial.println(mostCommonDirections[i]);
        if (mostCommonDirections[i] > maxDirectionCount) {
            maxDirectionCount = mostCommonDirections[i];
            averageDirection = i + 1;
        }
    }
    average = (double) reducer / reducerCount;

    val += ",wind_speed_average=" + String(average * ANEMOMETER_RADIUS);
    val += ",wind_speed_min=" + String(min * ANEMOMETER_RADIUS);
    val += ",wind_direction_min=" + String(minDir);
    val += ",wind_speed_max=" + String(max * ANEMOMETER_RADIUS);
    val += ",wind_direction_max=" + String(maxDir);
    val += ",wind_direction_average=" + String(averageDirection);

    Serial.printf("average: %.4f\n", average);
    Serial.printf("speed min: %d\n", min);
    Serial.printf("speed max: %d\n", max);
    Serial.printf("direction: %d\n\n", averageDirection);

    return val;
}

String anem::getWaterCount() {
    readBurst();

    Serial.println("water count" + String(waterCount));
    return ",water_count=" + String(waterCount * 20);
}

String anem::getLightnings() {
    String val = "";


    readBurst();
    Serial.println("lightningCount" + String(lightningCount));
    val += ",lightningCount=" + String(lightningCount);
    return val;
}
