//
// Created by Lazaro O'Farrill on 01/03/2023.
//

#include "Monitor/humiditySensor.h"

#ifdef USE_DHT20
using namespace Vortice;

#include "DHT20.h"
#include "utils/utils.h"

DHT20 dht20;

void HumidityReader::humidityStart() {
  const char *dht20Label = "Temp and Humidity (DHT20)";

  if (dht20.isConnected()) {
    Vortice::printDiagnostic(dht20Label, Status[OK]);
  } else {
    Vortice::printDiagnostic(dht20Label, Status[FAILED_TO_START]);
  }
}

TempAndHumidity HumidityReader::get_temperature_and_humidity() {

  if (millis() - dht20.lastRead() >= 1000) {
    dht20.read();
  }
  TempAndHumidity tempAndHumidity = {dht20.getTemperature(),
                                     dht20.getHumidity()};
  return tempAndHumidity;
}

#endif
