#include "Arduino.h"
#include "Monitor/monitorProcess.h"
#include "WebServer/webServerProcess.h"
#include "Wire.h"
#include "constants.h"
#include "pinout.h"
#include "utils/i2cScanner.h"
#include "utils/utils.h"
#include "Monitor/vane.h"

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
//    resetSetup();

    digitalWrite(PRO_MINI_RESET, LOW);
    delay(50);
    digitalWrite(PRO_MINI_RESET, HIGH);

//    timeStart();
//    Vortice::sensors::vane::start();
//    while (true) {
//        Serial.println(Vortice::sensors::vane::getAngle());
//        Serial.println(timeRead());
//        delay(1000);
//    }
    monitorSetup();
    Vortice::printDivider();

    if (supplyState || true) {
        digitalWrite(LOGO_LED, LOW);
        serverSetup();
    } else {
        digitalWrite(LOGO_LED, HIGH);
    }
}

void loop() { delay(1000000); }
