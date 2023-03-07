//
// Created by lazaroofarrill on 5/28/21.
//

#include "utils/utils.h"
#include "Monitor/realTimeClock.h"

using namespace Vortice;

void Vortice::printDiagnosticsHeader() {
    Serial.println();
    printDivider();
    printDiagnostic("Component", "Status", "Reason");
}

void Vortice::printDiagnostic(String component, String status, String reason) {
    Serial.printf("| %30s | %20s |\n", component.c_str(), status.c_str());
}

void Vortice::log(String data) {
    RTC.now();
    const auto timestamp = String(RTC.now().timestamp());
    Serial.println("Log - " + timestamp + " - " + data);
}

void printDiagnosticsFooter() {
    Vortice::printDivider();
}

void Vortice::printDivider() {
    String divisor = "";
    for (int i = 0; i < 57; i++) {
        divisor += "_";
    }
    Serial.println(divisor);
}
