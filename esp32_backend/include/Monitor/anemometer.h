//
// Created by lazaroofarrill on 10/22/21.
//

#ifndef IAGRI_DATA_LOGGER_FIRMWARE_ANEMOMETER_H
#define IAGRI_DATA_LOGGER_FIRMWARE_ANEMOMETER_H

#include "Arduino.h"


namespace anem {
    void IRAM_ATTR countPulse();

    void anemometerStart();

    String startReading();

    void finishReading();
}

#endif //IAGRI_DATA_LOGGER_FIRMWARE_ANEMOMETER_H
