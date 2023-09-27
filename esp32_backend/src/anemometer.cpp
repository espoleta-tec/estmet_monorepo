
#include "Monitor/anemometer.h"
#include "Arduino.h"
#include "cmath"
#include <Wire.h>

/**
 * @brief commands for send to ATTiny85 slave i2c
 *
 */
#define BURST_DATA 0x23
#define PLUVIO_ACC 0x24
#define AVAILABLE 0x01
#define DELETE_BUF 0xAA

#define LAST_POS 0x21

#define LIGHT_DATA 0xC0

#define MAX_BUFFER_DATA 12

#define ANEMOMETER_RADIUS 0.065

#define LINEAL_WIND_FACTOR ANEMOMETER_RADIUS * 2 * M_PI

#define A

const byte SLAVE_ADDR = 0x04;

const byte NUM_BYTES = 1;

const double WIND_B0 = 0.9695617251;
const double WIND_B1 = 1.1569123591;

byte data[NUM_BYTES] = {0};

byte bytesReceived = 0;

unsigned long timeNow = millis();

int a = 0;

uint8_t lightningCount;
uint16_t waterCount;

uint8_t readingsBuffer[400] = {0};

[[noreturn]] void tickerTask(void *param) {
  while (true) {
    digitalWrite(2, !digitalRead(2));
    digitalWrite(4, !digitalRead(4));
    delay(50);
  }
}

uint16_t nano::getCursor() {
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(LAST_POS);
  Wire.endTransmission();

  Wire.requestFrom(SLAVE_ADDR, 2u);

  uint8_t smallEnd, bigEnd;
  smallEnd = Wire.read();
  bigEnd = Wire.read();
  return bigEnd << 8 | smallEnd;
}

void nano::readBurst() {
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(BURST_DATA);
  Wire.endTransmission();

  Wire.requestFrom(SLAVE_ADDR, 5u);

  uint8_t smallEnd, bigEnd;
  Wire.read();
  Wire.read();
  smallEnd = Wire.read();
  bigEnd = Wire.read();
  waterCount = bigEnd << 8 | smallEnd;
  lightningCount = Wire.read();
}

uint8_t nano::avail() {
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(AVAILABLE);
  Wire.endTransmission();

  Wire.requestFrom(SLAVE_ADDR, 1u);

  uint8_t mm = Wire.read();

  return mm;
}

void nano::deleteBuffer() {
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(DELETE_BUF);
  Wire.endTransmission();

  Wire.requestFrom(SLAVE_ADDR, 1u);

  Wire.read();
  for (unsigned char &i : readingsBuffer) {
    i = 0;
  }
}

void nano::readBuff() {
  for (uint8_t j = 0; j < 30; j++) {

    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(0xC0 + j);
    Wire.endTransmission();

    Wire.requestFrom(SLAVE_ADDR, 12u);

    for (uint8_t i = 0; i < 12; i++) {
      readingsBuffer[12 * j + i] = Wire.read();
    }
  }
}

String nano::get_wind_speed() {
  String val = "";
  readBuff();

  double average, reducer;
  uint8_t min = 255, max = 0;
  uint16_t reducerCount = 0;

  reducer = 0;
  reducerCount = 0;
  for (int i = 0; i < 360; i += 2) {

    if (readingsBuffer[i] == 0xFF)
      continue;
    if (readingsBuffer[i] > max) {
      max = readingsBuffer[i];
    }
    if (readingsBuffer[i] < min) {
      min = readingsBuffer[i];
    }
    reducer += readingsBuffer[i];
    reducerCount++;
  }

  average = (double)reducer / reducerCount;

  double average_wind_speed = (average * LINEAL_WIND_FACTOR);
  double minimum_wind_speed = (min * LINEAL_WIND_FACTOR);
  double max_wind_speed = (max * LINEAL_WIND_FACTOR);
  TWindData wind_speed_data = {.wind_speed = (float)average_wind_speed};

  if (average_wind_speed > 0) {
    average_wind_speed = average_wind_speed * WIND_B1 + WIND_B0;
  }
  if (minimum_wind_speed > 0) {
    minimum_wind_speed = minimum_wind_speed * WIND_B1 + WIND_B0;
  }
  if (max_wind_speed > 0) {
    max_wind_speed = max_wind_speed * WIND_B1 + WIND_B0;
  }

  val += ",wind_speed_average=" + String(average_wind_speed);
  val += ",wind_speed_min=" + String(minimum_wind_speed);
  val += ",wind_speed_max=" + String(max_wind_speed);

  return val;
}

String nano::getWaterCount() {
  readBurst();
  if (waterCount == 0xFFFF)
    return "";

  return ",water_count=" + String(waterCount * 0.02); // mm of water
}

String nano::getLightnings() {
  String val = "";

  readBurst();
  if (lightningCount == 0xFF)
    return "";
  val += ",lightningCount=" + String(lightningCount);
  return val;
}
