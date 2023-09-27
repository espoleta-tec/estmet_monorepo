//
// Created by Lazaro O'Farrill on 01/03/2023.
//

#include "Monitor/humiditySensor.h"

using namespace Vortice;

String HumidityReader::humidityRead() {
    String vars = "";

    TempAndHumidity newValues = get_temperature_and_humidity();

    double h = newValues.humidity;
    double t = newValues.temperature;


    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return vars;
    }

    vars += ",humidity=" + String(h);
    vars += ",temperature_c=" + String(t);

    float hic = get_heat_index(newValues.temperature, newValues.humidity);
    float dewPoint = this->get_dew_point(newValues.temperature, newValues.humidity);

    vars += ",heatIndex_c=" + String(hic);
    vars += ",dew_point_c=" + String(dewPoint);


    return vars;
}

float HumidityReader::get_heat_index(float temperature_in_celsius, float relative_humidity) {
    auto heatIndex =
            0.5 *
            (temperature_in_celsius + 61.0 + ((temperature_in_celsius - 68.0) * 1.2) + (relative_humidity * 0.094));

    if (heatIndex > 79) {
        heatIndex = -42.379 +
                    2.04901523 * temperature_in_celsius +
                    10.14333127 * relative_humidity +
                    -0.22475541 * temperature_in_celsius * relative_humidity +
                    -0.00683783 * pow(temperature_in_celsius, 2) +
                    -0.05481717 * pow(relative_humidity, 2) +
                    0.00122874 * pow(temperature_in_celsius, 2) * relative_humidity +
                    0.00085282 * temperature_in_celsius * pow(relative_humidity, 2) +
                    -0.00000199 * pow(temperature_in_celsius, 2) * pow(relative_humidity, 2);

        if ((relative_humidity < 13) && (temperature_in_celsius >= 80.0) && (temperature_in_celsius <= 112.0))
            heatIndex -=
                    ((13.0 - relative_humidity) * 0.25) * sqrt((17.0 - abs(temperature_in_celsius - 95.0)) * 0.05882);

        else if ((relative_humidity > 85.0) && (temperature_in_celsius >= 80.0) && (temperature_in_celsius <= 87.0))
            heatIndex += ((relative_humidity - 85.0) * 0.1) * ((87.0 - temperature_in_celsius) * 0.2);
    }
    return heatIndex;
}

float HumidityReader::get_dew_point(float temperature_in_celsius, float relative_humidity) {
    // reference: http://wahiduddin.net/calc/density_algorithms.htm
    double A0 = 373.15 / (273.15 + (double) temperature_in_celsius);
    double SUM = -7.90298 * (A0 - 1);
    SUM += 5.02808 * log10(A0);
    SUM += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / A0))) - 1);
    SUM += 8.1328e-3 * (pow(10, (-3.49149 * (A0 - 1))) - 1);
    SUM += log10(1013.246);
    double VP = pow(10, SUM - 3) * (double) relative_humidity;
    double Td = log(VP / 0.61078); // temp var
    Td = (241.88 * Td) / (17.558 - Td);
    return Td;
}

HumidityReader reader;

void Vortice::humidityStart() {
    reader.humidityStart();
}

String Vortice::read_humidity() {
    return reader.humidityRead();
}

