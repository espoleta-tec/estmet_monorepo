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

    virtual TempAndHumidity get_temperature_and_humidity();

    float get_heat_index(float temperature_in_celsius, float relative_humidity);

    float get_dew_point(float temperature_in_celsius, float relative_humidity);
};

void humidityStart();

String humidityRead();
} // namespace Vortice

#endif // BACKEND_HUMIDITYSENSOR_H
