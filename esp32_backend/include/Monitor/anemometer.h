//
// Created by lazaroofarrill on 10/22/21.
//

#ifndef IAGRI_DATA_LOGGER_FIRMWARE_ANEMOMETER_H
#define IAGRI_DATA_LOGGER_FIRMWARE_ANEMOMETER_H

#include "Arduino.h"

#define BURST_DATA  0x23
#define PLUVIO_ACC  0x24
#define AVAILABLE   0x01
#define DELETE_BUF  0xAA

#define LAST_POS    0x21

#define MAX_BUFFER_DATA 12

extern uint8_t readingsBuffer[400];


namespace nano {
//    void IRAM_ATTR countPulse();

//    void anemometerStart();

//    String startReading();
//
//    void finishReading();


void readBurst();

uint16_t getCursor();

uint8_t avail();

String getWaterCount();

void deleteBuffer();

void readBuff();

String getWindValues();

String getLightnings();

typedef struct TWindData {
    float wind_speed;
} TWindData;
}


#endif //IAGRI_DATA_LOGGER_FIRMWARE_ANEMOMETER_H
