
#include <Arduino.h>


#define I2C_SLAVE_ADDRESS 0x04

#include "Wire.h"


#define BURST_DATA  0x23
#define PLUVIO_ACC  0x24
#define AVAILABLE   0x01
#define DELETE_BUF  0xAA

#define LAST_POS    0x21


#define LIGHT_DATA  0xC0

#define WIND_SPEED_PIN 3
#define WATER_COUNT_PIN 2
#define LIGHTNING_PIN 5
#define WIND_DIRECTION_PIN A3


uint8_t command = 0;

uint8_t buffer_sensors[360];

uint8_t wind_vel = 0;
uint8_t wind_dir = 0;
uint16_t pluv_acc = 0;

uint8_t isAvailableForRead = 0;
long long lastMillis = 0;
unsigned long tForRead = 0;
uint16_t counter = 0;
uint16_t lastPosition = 0;


uint8_t flag = 0;
uint8_t flag1 = 0;
uint8_t count = 0;
uint8_t count1 = 0;
uint8_t asdf = 0;

uint16_t temp = 0;
byte lightnings = 0;

void onLightning() {
    lightnings++;
}

void onWindSpeed() {
    wind_vel++;
}

void onWaterCount() {
    pluv_acc++;
}

void read_sensors();

void requestEvent() {
    if ((command & 0xC0) == LIGHT_DATA) {
        for (uint8_t i = 0; i < 12; i++)
            Wire.write(buffer_sensors[(command & 0x3F) * 12 + i]);
    }

    if ((command) == BURST_DATA) {
        Wire.write(wind_vel);
        Wire.write(wind_dir);
        Wire.write((uint8_t) (pluv_acc & 0xFF));
        Wire.write(pluv_acc >> 8);
        Wire.write(lightnings);
    }

    if ((command) == PLUVIO_ACC) {
        Wire.write((uint8_t) (pluv_acc & 0xFF));
        Wire.write(pluv_acc >> 8);
    }

    if ((command) == LAST_POS) {
        Wire.write((uint8_t) (lastPosition & 0xFF));
        Wire.write(lastPosition >> 8);
    }

    if ((command) == AVAILABLE) {
        Wire.write(isAvailableForRead);
    }

    if ((command) == DELETE_BUF) {
        for (unsigned char &buffer_sensor: buffer_sensors)
            buffer_sensor = 0xFF;

        pluv_acc = 0;
        counter = 0;
        lightnings = 0;

        Wire.write(DELETE_BUF);
    }
}

void receiveEvent(int num_bytes) {
    command = Wire.read();
    //buffer_sensors[0] =command;
}


void setup() {

    Wire.begin(I2C_SLAVE_ADDRESS);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);

    for (unsigned char &buffer_sensor: buffer_sensors)
        buffer_sensor = 0xFF;

    attachInterrupt(digitalPinToInterrupt(LIGHTNING_PIN), onLightning, RISING);

    pinMode(WIND_SPEED_PIN, INPUT);
    pinMode(WATER_COUNT_PIN, INPUT);

    tForRead = millis();
}

void loop() {

    if (millis() - tForRead >= 5000) {
        isAvailableForRead = 0;
        buffer_sensors[counter] = wind_vel;
        buffer_sensors[counter + 1] = wind_dir;
        if (counter < 360) {
            counter += 2;
            lastPosition = counter;
        } else counter = 0;

        tForRead = millis();
        isAvailableForRead = 1;
        wind_vel = 0;
    }

    read_sensors();
    // buffer_sensors[0]=0x45;
    // buffer_sensors[200]=0x23;
    // buffer_sensors[300]=0x12;
    // buffer_sensors[359]=0x78;

    //tws_delay(4000);
    /**
     * This is the only way we can detect stop condition (http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&p=984716&sid=82e9dc7299a8243b86cf7969dd41b5b5#984716)
     * it needs to be called in a very tight loop in order not to miss any (REMINDER: Do *not* use delay() anywhere, use tws_delay() instead).
     * It will call the function registered via Wire.onReceive(); if there is data in the buffer on stop.
     */
}


void read_sensors() {

    temp = analogRead(WIND_DIRECTION_PIN);
    if (temp >= 70 && temp <= 116) wind_dir = 1;
    if (temp >= 163 && temp <= 209) wind_dir = 2;
    if (temp >= 318 && temp <= 364) wind_dir = 3;
    if (temp >= 489 && temp <= 535) wind_dir = 4;

    lastMillis = millis();

    flag = 0;
    count = 0;
    flag1 = 0;
    count1 = 0;
    asdf = 0;

    while ((millis() - lastMillis) <= 1000) {
        asdf++;

        if (digitalRead(WIND_SPEED_PIN) == 1) {
            if (flag == 0)
                flag = 1;
        }
        if (digitalRead(WATER_COUNT_PIN) == 1) {
            if (flag1 == 0)
                flag1 = 1;
        }


        if (digitalRead(WIND_SPEED_PIN) == 0) {
            if (flag == 1) {
                flag = 0;
                count++;
            }
        }
        if (digitalRead(WATER_COUNT_PIN) == 0) {
            if (flag1 == 1) {
                flag1 = 0;
                count1++;
            }
        }
    }

    wind_vel = count;
    pluv_acc += count1;
    //pluv_acc=0x1234;

}