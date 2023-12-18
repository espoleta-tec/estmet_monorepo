//
// Created by lazt on 4/23/21.
//
#include "Monitor/humiditySensor.h"

#include "DHTesp.h"
#include "pinout.h"
#include "utils/utils.h"

#ifdef USE_DHT11

namespace Vortice {

DHTesp dht11;

const double TEMP_B0 = -2.4722766403;
const double TEMP_B1 = 1.0547831582;
const double HUMIDITY_B0 = -14.1345353691;
const double HUMIDITY_B1 = 1.2579750347;

#ifdef USE_DHT22
#define DHT_TYPE DHTesp::DHT22
#else
#define DHT_TYPE DHTesp::DHT11
#endif

void HumidityReader::humidityStart() {
  dht11.setup(DHTPIN, DHT_TYPE);
  float temp = dht11.getTemperature();
#ifdef USE_DHT22
  const char *dhtLabel = "DHT22";
#else
  const char *dhtLabel = "DHT11";
#endif

  if (temp == NAN) {
    printDiagnostic(dhtLabel, Vortice::Status[FAILED_TO_START]);
  } else {
    printDiagnostic(dhtLabel, Vortice::Status[OK]);
  }
}

static unsigned long last_read = 0;
static TempAndHumidity tah = {.temperature = 0, .humidity = 0};

TempAndHumidity HumidityReader::get_temperature_and_humidity() {

  if (millis() - last_read >= 3000) {
    last_read = millis();
    return tah;
  }

  tah.temperature = dht11.getTemperature();
  tah.humidity = dht11.getHumidity();
  return tah;
}
} // namespace Vortice
#endif
