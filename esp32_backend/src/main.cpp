#include "Wire.h"
#include "Arduino.h"
#include "WebServer/webServerProcess.h"
#include "Monitor/monitorProcess.h"
#include "pinout.h"
#include "utils/utils.h"
#include "utils/i2cScanner.h"


using namespace Vortice;

void setup() {
    Serial.begin(9600);
    Serial.flush();
    Vortice::printDiagnosticsHeader();
    Wire.begin(SDA, SCL, 1e5);


    const char *i2cLabel = "I2C";

    if (!Wire.begin()) {
        printDiagnostic(i2cLabel, Status[FAILED_TO_START]);
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
    resetSetup();

    if (supplyState) {
        serverSetup();
        digitalWrite(LOGO_LED, LOW);
    } else {
        digitalWrite(LOGO_LED, HIGH);
    }

    digitalWrite(PRO_MINI_RESET, LOW);
    delay(50);
    digitalWrite(PRO_MINI_RESET, HIGH);

    monitorSetup();
    Vortice::printDivider();
}

void loop() {
    delay(1000000);
}
