
#include "Monitor/anemometer.h"
#include "Arduino.h"
#include "math.h"
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

#define ANEMOMETER_RADIUS 0.060

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
  Serial.print(Wire.read(), HEX);
  Serial.print("  ");
  Serial.print(Wire.read(), HEX);
  Serial.print("  ");
  smallEnd = Wire.read();
  Serial.print(smallEnd, HEX);
  Serial.print("  ");
  bigEnd = Wire.read();
  Serial.print(bigEnd, HEX);
  Serial.print("  ");
  waterCount = bigEnd << 8 | smallEnd;
  Serial.print(waterCount);
  Serial.print(" ");
  lightningCount = Wire.read();
  Serial.print(lightningCount, HEX);
  Serial.println();
}

uint8_t nano::avail() {
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(AVAILABLE);
  Wire.endTransmission();

  Wire.requestFrom(SLAVE_ADDR, 1u);

  uint8_t mm = Wire.read();
  // Serial.println(mm ,HEX);

  return mm;
}

void nano::deleteBuffer() {
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(DELETE_BUF);
  Wire.endTransmission();

  Wire.requestFrom(SLAVE_ADDR, 1u);

  Serial.println(Wire.read(), HEX);
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

String nano::getWindValues() {
  String val = "";
  readBuff();

  double average, reducer;
  uint8_t min = 255, max = 0;
  uint16_t mostCommonDirections[8] = {0}, averageDirection = 0,
           maxDirectionCount = 0, reducerCount = 0;
  byte minDir = -1, maxDir = -1;

  reducer = 0;
  reducerCount = 0;
  for (int i = 0; i < 360; i += 2) {

    if (readingsBuffer[i] == 0xFF)
      continue;
    if (readingsBuffer[i] > max) {
      max = readingsBuffer[i];
      maxDir = readingsBuffer[i + 1];
      Serial.print("new max: ");
      Serial.print(readingsBuffer[i]);
      Serial.print("position: ");
      Serial.print(i);
      Serial.println();
    }
    if (readingsBuffer[i] < min) {
      min = readingsBuffer[i];
      minDir = readingsBuffer[i + 1];
    }
    reducer += readingsBuffer[i];
    reducerCount++;
    mostCommonDirections[readingsBuffer[i + 1] - 1]++;
  }
  for (int i = 0;
       i < sizeof(mostCommonDirections) / sizeof(*mostCommonDirections); i++) {
    if (mostCommonDirections[i] > maxDirectionCount) {
      maxDirectionCount = mostCommonDirections[i];
      averageDirection = i + 1;
    }
  }
  average = (double)reducer / reducerCount;

  double averageWindSpeed = (average * LINEAL_WIND_FACTOR);
  if (averageWindSpeed) {
    averageWindSpeed = averageWindSpeed * WIND_B1 + WIND_B0;
  }
  double minWindSpeed = (min * LINEAL_WIND_FACTOR);
  if (minWindSpeed) {
    minWindSpeed = minWindSpeed * WIND_B1 + WIND_B0;
  }
  double maxWindSpeed = (max * LINEAL_WIND_FACTOR);
  if (maxWindSpeed) {
    maxWindSpeed = maxWindSpeed * WIND_B1 + WIND_B0;
  }

  val += ",wind_speed_average=" + String(averageWindSpeed);
  val += ",wind_speed_min=" + String(minWindSpeed);
  val += ",wind_speed_max=" + String(maxWindSpeed);
  //    val += ",wind_direction_max=" + String(maxDir);
  //    val += ",wind_direction_min=" + String(minDir);
  //    val += ",wind_direction_average=" + String(averageDirection);

  Serial.printf("average: %.4f\n", average);
  Serial.printf("speed min: %d\n", min);
  Serial.printf("speed max: %d\n", max);

  return val;
}

String nano::getWaterCount() {
  readBurst();
  if (waterCount == 0xFFFF)
    return "";

  Serial.println("water count" + String(waterCount));
  return ",water_count=" + String(waterCount * 0.02); // mm of water
}

String nano::getLightnings() {
  String val = "";

  readBurst();
  if (lightningCount == 0xFF)
    return "";
  Serial.println("lightningCount" + String(lightningCount));
  val += ",lightningCount=" + String(lightningCount);
  return val;
}
