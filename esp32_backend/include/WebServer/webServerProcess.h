//
// Created by lazt on 4/23/21.
//

#ifndef BACKEND_WEBSERVERPROCESS_H
#define BACKEND_WEBSERVERPROCESS_H

#include "Arduino.h"
#include "WiFi.h"
#include "WebServer.h"
#include "WiFiClient.h"
#include "WiFiAP.h"
#include "WebSocketsServer.h"

extern boolean supplyState;

void serverSetup();

void resetSetup();

[[noreturn]] void serverLoop(void *);

[[noreturn]] void resetLoop(void *);


#endif //BACKEND_WEBSERVERPROCESS_H
