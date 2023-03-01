//
// Created by lazt on 4/23/21.
//

#ifndef BACKEND_HUMIDITYSENSOR_H
#define BACKEND_HUMIDITYSENSOR_H

#include "Arduino.h"

namespace Vortice {
    typedef struct TempAndHumidity {
        float temperature;
        float humidity;
    } TempAndHumidity;

    class HumidityReader {
    public:
        virtual void humidityStart();

        String humidityRead();

    private:
        virtual TempAndHumidity getTemperatureAndHumidity();

        float getHeatIndex(float temperatureInCelsius, float relativeHumidity);

        float getDewPoint(float tempInCelsisu, float relativeHumidity);
    };

    void humidityStart();

    String humidityRead();
}

#define USE_DHT20
//#define USE_DHT11

#endif //BACKEND_HUMIDITYSENSOR_H
