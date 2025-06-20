#ifndef WIFI_H
#define WIFI_H

#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include "secrets.h"  // Contains your SSID, password, and API keys
#include "wifi.h"

bool setup_wifi();

#endif
