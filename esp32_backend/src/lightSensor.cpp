//
// Created by lazt on 4/23/21.
//

#include "Monitor/lightSensor.h"
#include "utils/utils.h"

BH1750 lightMeter(LIGHT_SENSOR_ADDRESS);

bool lightReady = false;

void lightStart() {
    const char *bh1750Label = "Light (BH1750)";


    lightReady = lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);

    if (lightReady) {
        Vortice::printDiagnostic(bh1750Label, Vortice::Status[Vortice::OK]);
    } else {
        Vortice::printDiagnostic(bh1750Label, Vortice::Status[Vortice::FAILED_TO_START]);
    }
}

String lightRead() {
    String vars = "";
    if (lightReady && lightMeter.measurementReady()) {
        float lux = lightMeter.readLightLevel();
        vars += ",light=" + String(lux);
        Serial.print("Light: ");
        Serial.print(lux);
        Serial.println(" lx");
    }
    return vars;
}
