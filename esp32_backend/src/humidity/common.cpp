//
// Created by Lazaro O'Farrill on 01/03/2023.
//

#include "Monitor/humiditySensor.h"

using namespace Vortice;

String HumidityReader::humidityRead() {
    String vars = "";

    TempAndHumidity newValues = getTemperatureAndHumidity();

    double h = newValues.humidity;
    double t = newValues.temperature;


    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return vars;
    }

    vars += ",humidity=" + String(h);
    vars += ",temperature_c=" + String(t);

    auto hic = getHeatIndex(newValues.temperature, newValues.humidity);
    auto dewPoint = this->getDewPoint(newValues.temperature, newValues.humidity);


    vars += ",heatIndex_c=" + String(hic);
    vars += ",dew_point_c=" + String(dewPoint);


    Serial.printf("Humidity: %f\t", h);
    Serial.printf("Temperature: %f\t°C", t);
    Serial.printf("Heat index: %f\t", hic);
    Serial.printf("Dew point: %f\t°C", dewPoint);
    Serial.println();


    return vars;
}

float HumidityReader::getHeatIndex(float temperatureInCelsius, float relativeHumidity) {
    auto heatIndex =
            0.5 * (temperatureInCelsius + 61.0 + ((temperatureInCelsius - 68.0) * 1.2) + (relativeHumidity * 0.094));

    if (heatIndex > 79) {
        heatIndex = -42.379 +
                    2.04901523 * temperatureInCelsius +
                    10.14333127 * relativeHumidity +
                    -0.22475541 * temperatureInCelsius * relativeHumidity +
                    -0.00683783 * pow(temperatureInCelsius, 2) +
                    -0.05481717 * pow(relativeHumidity, 2) +
                    0.00122874 * pow(temperatureInCelsius, 2) * relativeHumidity +
                    0.00085282 * temperatureInCelsius * pow(relativeHumidity, 2) +
                    -0.00000199 * pow(temperatureInCelsius, 2) * pow(relativeHumidity, 2);

        if ((relativeHumidity < 13) && (temperatureInCelsius >= 80.0) && (temperatureInCelsius <= 112.0))
            heatIndex -= ((13.0 - relativeHumidity) * 0.25) * sqrt((17.0 - abs(temperatureInCelsius - 95.0)) * 0.05882);

        else if ((relativeHumidity > 85.0) && (temperatureInCelsius >= 80.0) && (temperatureInCelsius <= 87.0))
            heatIndex += ((relativeHumidity - 85.0) * 0.1) * ((87.0 - temperatureInCelsius) * 0.2);
    }
    return heatIndex;
}

float HumidityReader::getDewPoint(float tempInCelsius, float relativeHumidity) {
    // reference: http://wahiduddin.net/calc/density_algorithms.htm
    double A0 = 373.15 / (273.15 + (double) tempInCelsius);
    double SUM = -7.90298 * (A0 - 1);
    SUM += 5.02808 * log10(A0);
    SUM += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / A0))) - 1);
    SUM += 8.1328e-3 * (pow(10, (-3.49149 * (A0 - 1))) - 1);
    SUM += log10(1013.246);
    double VP = pow(10, SUM - 3) * (double) relativeHumidity;
    double Td = log(VP / 0.61078); // temp var
    Td = (241.88 * Td) / (17.558 - Td);
    return Td;
}

HumidityReader reader;

void Vortice::humidityStart() {
    reader.humidityStart();
}

String Vortice::humidityRead() {
    return reader.humidityRead();
}

