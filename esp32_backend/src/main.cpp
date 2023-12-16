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

const char *build_types[] = {"rel", "dbg"};

void setup() {
  Serial.begin(9600);
  Serial.flush();
  Vortice::printDiagnosticsHeader();
  Vortice::printDiagnostic("Firmware Version", FIRMWARE_VERSION);
  Wire.begin(SDA, SCL, 1e5);
  Wire1.begin(SDA1, SCL1, 1e5);

  const char *i2cLabel = "I2C";

  if (!(Wire.begin() && Wire1.begin())) {
    printDiagnostic(i2cLabel, Status[Vortice::FAILED_TO_START]);
  } else {
    printDiagnostic(i2cLabel, Status[Vortice::OK]);
  }

  pinMode(LED_BUILTIN, OUTPUT);
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

#ifdef ARDUINO_CONNECTED
  digitalWrite(PRO_MINI_RESET, LOW);
  delay(50);
  digitalWrite(PRO_MINI_RESET, HIGH);
#endif

  monitorSetup();
  Vortice::printDivider();

#ifdef USE_SERVER
#ifdef DEBUG
  serverSetup();
#else
  if (supplyState) {
    serverSetup();
  }
#endif
#endif
}

void loop() { delay(1000000); }
