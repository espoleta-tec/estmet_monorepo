
#include <Arduino.h>

#define I2C_SLAVE_ADDRESS 0x04

#include "Wire.h"

#define BURST_DATA 0x23
#define PLUVIO_ACC 0x24
#define AVAILABLE 0x01
#define DELETE_BUF 0xAA

#define LAST_POS 0x21

#define LIGHT_DATA 0xC0

#define WIND_SPEED_PIN 5
#define WATER_COUNT_PIN 2
#define LIGHTNING_PIN 3
#define WIND_DIRECTION_PIN A3

#define AXIS_1 6
#define AXIS_2 7
#define AXIS_3 8
#define AXIS_4 9

int flag_axis_1 = 0;
int flag_axis_2 = 0;
int flag_axis_3 = 0;
int flag_axis_4 = 0;

uint8_t command = 0;

uint8_t buffer_sensors[360];

uint8_t wind_vel = 0;
uint8_t wind_dir = 0;
uint8_t last_wind_dir = 0;
uint8_t second_last_wind_dir = 0;
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

unsigned long lastLightning = 0;

void onLightning()
{
    if (millis() - lastLightning >= 50)
    {
        lightnings++;
        lastLightning = millis();
    }
}

void onWindSpeed()
{
    wind_vel++;
}

void onWaterCount()
{
    pluv_acc++;
}

void watchDirection();

void read_sensors();

void readWindDirection();

void requestEvent()
{
    if ((command & 0xC0) == LIGHT_DATA)
    {
        for (uint8_t i = 0; i < 12; i++)
            Wire.write(buffer_sensors[(command & 0x3F) * 12 + i]);
    }

    if ((command) == BURST_DATA)
    {
        Wire.write(wind_vel);
        Wire.write(wind_dir);
        Wire.write((uint8_t)(pluv_acc & 0xFF));
        Wire.write(pluv_acc >> 8);
        Wire.write(lightnings);
    }

    if ((command) == PLUVIO_ACC)
    {
        Wire.write((uint8_t)(pluv_acc & 0xFF));
        Wire.write(pluv_acc >> 8);
    }

    if ((command) == LAST_POS)
    {
        Wire.write((uint8_t)(lastPosition & 0xFF));
        Wire.write(lastPosition >> 8);
    }

    if ((command) == AVAILABLE)
    {
        Wire.write(isAvailableForRead);
    }

    if ((command) == DELETE_BUF)
    {
        for (unsigned char &buffer_sensor : buffer_sensors)
            buffer_sensor = 0xFF;

        pluv_acc = 0;
        counter = 0;
        lightnings = 0;

        Wire.write(DELETE_BUF);
    }
}

void receiveEvent(int num_bytes)
{
    command = Wire.read();
    // buffer_sensors[0] =command;
}

void setup()
{
    Serial.begin(9600);

    Wire.begin(I2C_SLAVE_ADDRESS);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);

    for (unsigned char &buffer_sensor : buffer_sensors)
        buffer_sensor = 0xFF;

    attachInterrupt(digitalPinToInterrupt(LIGHTNING_PIN), onLightning, RISING);

    pinMode(WIND_SPEED_PIN, INPUT);
    pinMode(WATER_COUNT_PIN, INPUT);
    pinMode(AXIS_1, INPUT);
    pinMode(AXIS_2, INPUT);
    pinMode(AXIS_3, INPUT);
    pinMode(AXIS_4, INPUT);

    tForRead = millis();
}

void loop()
{

    watchDirection();
    if (millis() - tForRead >= 5000)
    {
        isAvailableForRead = 0;
        buffer_sensors[counter] = wind_vel;
        buffer_sensors[counter + 1] = wind_dir;
        if (counter < 360)
        {
            counter += 2;
            lastPosition = counter;
        }
        else
            counter = 0;

        tForRead = millis();
        isAvailableForRead = 1;
        wind_vel = 0;
    }

    read_sensors();
    //    Serial.println(lightnings);
    Serial.println(wind_dir);
    // buffer_sensors[0]=0x45;
    // buffer_sensors[200]=0x23;
    // buffer_sensors[300]=0x12;
    // buffer_sensors[359]=0x78;

    // tws_delay(4000);
    /**
     * This is the only way we can detect stop condition (http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&p=984716&sid=82e9dc7299a8243b86cf7969dd41b5b5#984716)
     * it needs to be called in a very tight loop in order not to miss any (REMINDER: Do *not* use delay() anywhere, use tws_delay() instead).
     * It will call the function registered via Wire.onReceive(); if there is data in the buffer on stop.
     */
}

void read_sensors()
{
    lastMillis = millis();
    long lastPluvio = millis();

    flag = 0;
    count = 0;
    flag1 = digitalRead(WATER_COUNT_PIN);
    count1 = 0;

    while ((millis() - lastMillis) <= 1000)
    {
        watchDirection();

        if (digitalRead(WIND_SPEED_PIN) == 1)
        {
            if (flag == 0)
                flag = 1;
        }
        if (digitalRead(WATER_COUNT_PIN) == 1)
        {
            if (flag1 == 0 && (millis() - lastPluvio > 50))
            {
                lastPluvio = millis();
                count1++;
                flag1 = 1;
            }
        }

        if (digitalRead(WIND_SPEED_PIN) == 0)
        {
            if (flag == 1)
            {
                flag = 0;
                count++;
            }
        }
        if (digitalRead(WATER_COUNT_PIN) == 0)
        {
            if (flag1 == 1 && (millis() - lastPluvio > 50))
            {
                lastPluvio = millis();
                flag1 = 0;
                count1++;
            }
        }
    }

    wind_vel = count;
    pluv_acc += count1;
    // pluv_acc=0x1234;
}

void readWindDirection()
{
    temp = analogRead(WIND_DIRECTION_PIN);
    if (temp >= 77 && temp <= 82)
        wind_dir = 1;
    if (temp >= 47 && temp <= 52)
        wind_dir = 2;
    if (temp >= 37 && temp <= 42)
        wind_dir = 3;
    if (temp >= 17 && temp <= 22)
        wind_dir = 4;
}

void watchDirection()
{
    //    if (digitalRead(AXIS_1) == 1) wind_dir = 1;
    //    if (digitalRead(AXIS_2) == 1) wind_dir = 2;
    //    if (digitalRead(AXIS_3) == 1) wind_dir = 3;
    //    if (digitalRead(AXIS_4) == 1) wind_dir = 4;

    uint8_t halls[4] = {0};

    halls[0] = digitalRead(AXIS_1);
    halls[1] = digitalRead(AXIS_2);
    halls[2] = digitalRead(AXIS_3);
    halls[3] = digitalRead(AXIS_4);

    uint8_t value = (halls[0] << 3) | (halls[1] << 2) | (halls[2] << 1) | halls[3];
    // Serial.print(halls[0]);
    // Serial.print(" ");
    // Serial.print(halls[1]);
    // Serial.print(" ");
    // Serial.print(halls[2]);
    // Serial.print(" ");
    // Serial.println(halls[3]);
    // Serial.println(value, HEX);

    uint8_t cardinalPoints[8] = {0xC, 0x1, 0x6, 0x8, 0x3, 0x4, 0x9, 0x2};

    for (int i = 0; i < sizeof(cardinalPoints); i++) {
        if (cardinalPoints[i] == value) {
            wind_dir = i+1;
        }
    }


    // if (digitalRead(AXIS_1) == 1 && flag_axis_1 == 0)
    // {
    //     second_last_wind_dir = last_wind_dir;
    //     last_wind_dir = wind_dir;
    //     wind_dir = 1;
    //     flag_axis_1 = 1;
    // }
    // else if (digitalRead(AXIS_2) == 1 && flag_axis_2 == 0)
    // {
    //     second_last_wind_dir = last_wind_dir;
    //     last_wind_dir = wind_dir;
    //     wind_dir = 3;
    //     flag_axis_2 = 1;
    // }
    // else if (digitalRead(AXIS_3) == 1 && flag_axis_3 == 0)
    // {
    //     second_last_wind_dir = last_wind_dir;
    //     last_wind_dir = wind_dir;
    //     wind_dir = 5;
    //     flag_axis_3 = 1;
    // }
    // else if (digitalRead(AXIS_4) == 1 && flag_axis_4 == 0)
    // {
    //     second_last_wind_dir = last_wind_dir;
    //     last_wind_dir = wind_dir;
    //     wind_dir = 7;
    //     flag_axis_4 = 1;
    // }
    // else if ((!digitalRead(AXIS_1) &&
    //           !digitalRead(AXIS_2) &&
    //           !digitalRead(AXIS_3) &&
    //           !digitalRead(AXIS_4)) &&
    //          wind_dir % 2 != 0)
    // {
    //     digitalWrite(12, !digitalRead(12));
    //     if (wind_dir == 1 && (last_wind_dir == 8 || last_wind_dir == 7))
    //     {
    //         last_wind_dir = wind_dir;
    //         wind_dir = 2;
    //     }
    //     else if (wind_dir == 1 && (last_wind_dir == 2 || last_wind_dir == 3))
    //     {
    //         last_wind_dir = wind_dir;
    //         wind_dir = 8;
    //     }
    //     else if (wind_dir > last_wind_dir)
    //     {
    //         last_wind_dir = wind_dir;
    //         wind_dir++;
    //     }
    //     else if (wind_dir < last_wind_dir)
    //     {
    //         last_wind_dir = wind_dir;
    //         wind_dir--;
    //     }
    // }

    // if (digitalRead(AXIS_1) == 0)
    // {
    //     flag_axis_1 = 0;
    // }
    // if (digitalRead(AXIS_2) == 0)
    // {
    //     flag_axis_2 = 0;
    // }
    // if (digitalRead(AXIS_3) == 0)
    // {
    //     flag_axis_3 = 0;
    // }
    // if (digitalRead(AXIS_4) == 0)
    // {
    //     flag_axis_4 = 0;
    // }
}
