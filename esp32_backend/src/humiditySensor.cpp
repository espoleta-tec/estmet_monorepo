//
// Created by lazt on 4/23/21.
//

#include "Monitor/humiditySensor.h"

//DHT dht(DHTPIN, DHTTYPE);
DHTesp dht;


void humidityStart() {
    dht.setup(DHTPIN, DHTesp::DHT22);
}

String humidityRead() {
        String vars = "";
    
      TempAndHumidity newValues = dht.getTempAndHumidity();
      if (dht.getStatus() != 0) {
		Serial.println("DHT11 error status: " + String(dht.getStatusString()));
        Serial.println("Failed to read from DHT sensor!");
		return vars;
	}
    double h = newValues.humidity;
    double t = newValues.temperature;


    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return vars;
    }

    vars += ",humidity=" + String(h);
    vars += ",temperature_c=" + String(t);

    double hic =dht.computeHeatIndex(newValues.temperature, newValues.humidity);

    vars += ",heatIndex_c=" + String(hic);


    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(F("Heat index: "));
    Serial.print(hic);
    Serial.println(F("°C "));

    return vars;
}
