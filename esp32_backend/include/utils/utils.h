//
// Created by lazaroofarrill on 5/28/21.
//

#ifndef BACKEND_UTILS_H
#define BACKEND_UTILS_H

#include "Arduino.h"

namespace Vortice {

enum ComponentState { OK, FAILED_TO_START, ON, OFF };
const String Status[] = {"OK", "FAILED_TO_START", "ON", "OFF"};

void printDiagnosticsHeader();

void printDiagnosticsFooter();

void printDiagnostic(String component, String status, String reason = "");

void log(String data);

void printDivider();
} // namespace Vortice

#endif // BACKEND_UTILS_H
