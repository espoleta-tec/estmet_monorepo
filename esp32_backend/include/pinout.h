//
// Created by lazt on 4/23/21.
//

#ifndef BACKEND_PINOUT_H
#define BACKEND_PINOUT_H

// DHT SENSORS

// humidity and temperature
#ifdef USE_DHT11
#define DHTPIN 16
#endif

#define SDA1 33
#define SCL1 32

// light
#define LIGHT_SENSOR_ADDRESS 0x23

#define POWER_PIN 15

// power
#include "driver/adc.h"

#define RESET_PIN 17
#define BATTERY_LEVEL ADC1_CHANNEL_4

// control pins
#define PRO_MINI_RESET 4

// lightning watcher
#define LIGHTNING_PIN 32

#endif // BACKEND_PINOUT_H
