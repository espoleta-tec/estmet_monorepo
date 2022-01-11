//
// Created by lazaroofarrill on 12/30/21.
//

#include <pinout.h>
#include "Battery/battery_level.h"
#include "driver/adc.h"

unsigned long getBatteryLevel() {
    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(BATTERY_LEVEL, ADC_ATTEN_DB_11);
    return adc1_get_raw(BATTERY_LEVEL);
}

String getBatteryLevelString() {
    String data = "";
    data += ",battery_level=" + String(getBatteryLevel());
    Serial.println(data);
    return data;
}


