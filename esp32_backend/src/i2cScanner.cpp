//
// Created by Lazaro O'Farrill on 23/02/2023.
//

#include "utils/i2cScanner.h"
#include "Wire.h"

#define SLAVE_ADDRESS 0x05

uint8_t command = 0;

void onRecive(int num_bytes) { command = Wire.read(); }

void onRequest() { Wire.write(0x01); }

void scanI2C(bool useOne) {
  int nDevices = 0;

  TwoWire *wireInstance = useOne ? &Wire1 : &Wire;

  Serial.printf("Scanning on Wire%d\n", useOne);

  for (byte address = 1; address < 127; ++address) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    wireInstance->beginTransmission(address);
    byte error = wireInstance->endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println("  !");

      ++nDevices;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }
  delay(5000); // Wait 5 seconds for next scan
}

void i2cSlave() {}
