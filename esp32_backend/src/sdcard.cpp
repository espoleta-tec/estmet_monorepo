//
// Created by lazt on 4/23/21.
//

#include "FileSystem/sdcard.h"
#include "utils/utils.h"

bool cardMounted = false;

#define SD_PIN 5

void initSDCard() {
  cardMounted = SD.begin(SD_PIN);
  const char *sdCardLabel = "SD Card Status";
  if (!cardMounted) {
    Vortice::printDiagnostic(sdCardLabel, "FAILED TO MOUNT");
    return;
  }

  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Vortice::printDiagnostic(sdCardLabel, "NOT ATTACHED");
    return;
  }
  const char *sdCardTypeLabel = "SD Card Type";
  String sdCardType = "UNKNOWN";

  if (cardType == CARD_MMC) {
    sdCardType = "MMC";
  } else if (cardType == CARD_SD) {
    sdCardType = "SDSC";
  } else if (cardType == CARD_SDHC) {
    sdCardType = "SDHC";
  }
  Vortice::printDiagnostic(sdCardTypeLabel, sdCardType);

  const char *sdCardSizeLabel = "SD Card Size";

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  uint64_t cardUsed = SD.usedBytes() / (1024 * 1024);

  char *label = (char *)malloc(sizeof(char) * 20);
  sprintf(label, "%lu/%luMB used", cardUsed, cardSize);
  Vortice::printDiagnostic(sdCardSizeLabel, label);
  free(label);
}
