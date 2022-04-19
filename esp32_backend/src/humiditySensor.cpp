//
// Created by lazt on 4/23/21.
//

#include "Monitor/humiditySensor.h"

//DHT dht(DHTPIN, DHTTYPE);
DHTesp dht;

const double TEMP_B0 = -2.4722766403;
const double TEMP_B1 = 1.0547831582;
const double HUMIDITY_B0 = -14.1345353691;
const double HUMIDITY_B1 = 1.2579750347;


void humidityStart() {
    dht.setup(DHTPIN, DHTesp::DHT11);
}

String humidityRead() {
    String vars = "";

    TempAndHumidity newValues = dht.getTempAndHumidity();
    if (dht.getStatus() != 0) {
        Serial.println("DHT11 error status: " + String(dht.getStatusString()));
        Serial.println("Failed to read from DHT sensor!");
        return vars;
    }
    double h = HUMIDITY_B0 + HUMIDITY_B1 * newValues.humidity;
    double t = TEMP_B0 + TEMP_B1 * newValues.temperature;


    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return vars;
    }

    vars += ",humidity=" + String(h);
    vars += ",temperature_c=" + String(t);

    double hic = dht.computeHeatIndex(newValues.temperature, newValues.humidity);
    double dewPoint = dht.computeDewPoint(t, h);


    vars += ",heatIndex_c=" + String(hic);
    vars += ",dew_point_c=" + String(dewPoint);


    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(F("Heat index: "));
    Serial.print(hic);
    Serial.print("dewPoint: ");
    Serial.print(dewPoint);
    Serial.println(F("°C "));


    return vars;
}
