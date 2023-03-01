//
// Created by Lazaro O'Farrill on 01/03/2023.
//


#include "Monitor/humiditySensor.h"

#ifdef USE_DHT20
using namespace Vortice;

#include "DHT20.h"

DHT20 dht20;

void HumidityReader::humidityStart() {
    if (dht20.isConnected()) {
        Serial.println("DHT20 is connected correctly.");
    } else {
        Serial.printf("Fail to connect to DHT20 at %x", dht20.getAddress());
    }
}

TempAndHumidity HumidityReader::getTemperatureAndHumidity() {

    if (millis() - dht20.lastRead() >= 1000) {
        dht20.read();
    }
    TempAndHumidity tempAndHumidity = {
            dht20.getTemperature(), dht20.getHumidity()};
    return tempAndHumidity;
}

#endif
