#include "Arduino.h"
#include "Monitor/monitorProcess.h"
#include "Monitor/vane.h"
#include "WebServer/webServerProcess.h"
#include "Wire.h"
#include "constants.h"
#include "esp32-hal-gpio.h"
#include "pinout.h"
#include "utils/i2cScanner.h"
#include "utils/utils.h"

using namespace Vortice;

void setup() {
  Serial.begin(9600);
  Serial.flush();
  Vortice::printDiagnosticsHeader();
  Vortice::printDiagnostic("Firmware Version", String(FIRMWARE_VERSION));
  Wire.begin(SDA, SCL, 1e5);
  Wire1.begin(SDA1, SCL1, 1e5);

  const char *i2cLabel = "I2C";

  if (!(Wire.begin() && Wire1.begin())) {
    printDiagnostic(i2cLabel, Status[Vortice::FAILED_TO_START]);
  } else {
    printDiagnostic(i2cLabel, Status[Vortice::OK]);
  }

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LOGO_LED, OUTPUT);
  pinMode(PRO_MINI_RESET, OUTPUT);
  pinMode(POWER_PIN, INPUT);
  pinMode(RESET_PIN, INPUT_PULLUP);

  initFileSystem();
  loadConf();
  initSDCard();
  supplyState = digitalRead(POWER_PIN);
  Vortice::printDiagnostic("Power Supply", supplyState ? "ON" : "OFF");

#ifndef DISABLE_RESET
  resetSetup();
#endif

  digitalWrite(PRO_MINI_RESET, LOW);
  delay(50);
  digitalWrite(PRO_MINI_RESET, HIGH);

  monitorSetup();
  Vortice::printDivider();
#ifdef DEBUG
  serverSetup();
  digitalWrite(LOGO_LED, HIGH);
#else
  if (supplyState) {
    digitalWrite(LOGO_LED, LOW);
    serverSetup();
  } else {
    digitalWrite(LOGO_LED, HIGH);
  }
#endif
}

void loop() { delay(1000000); }
