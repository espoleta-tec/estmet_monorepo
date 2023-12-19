//
// Created by Lazaro O'Farrill on 01/03/2023.
//
#include "Monitor/lightningDetector.h"
#include "SparkFun_AS3935.h"
#include "utils/utils.h"

#ifdef USE_LIGHTNING

#ifdef AS3935_AT_03
#define AS3935_ADDR 0x03
#else
#define AS3935_ADDR 0x02
#endif

#define INDOOR 0x12
#define OUTDOOR 0xE
#define DISTURBER_INT 0x04
#define NOISE_INT 0x01

SparkFun_AS3935 lightning(AS3935_ADDR);

void Vortice::startLightning() {
  const char *lightningLabel = "Lightning (AS3935)";
  if (lightning.begin()) {
    Vortice::printDiagnostic(lightningLabel, Vortice::Status[OK]);
  } else {
    Vortice::printDiagnostic(lightningLabel,
                             Vortice::Status[Vortice::FAILED_TO_START]);
  }
}
#endif
