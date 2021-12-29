//
// Created by lazt on 4/23/21.
//

#ifndef BACKEND_PINOUT_H
#define BACKEND_PINOUT_H

//DHT SENSORS
#include "Adafruit_Sensor.h"
#include "DHTesp.h"

//humidity and temperature
#define DHTPIN 33
#define DHTTYPE DHT22

//anemometer
#define ANEMOMETER_PIN 15


//light
#define LIGHT_SENSOR_ADDRESS 0x23

#define POWER_PIN 36


#endif //BACKEND_PINOUT_H