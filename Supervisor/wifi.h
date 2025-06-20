#ifndef WIFI_H
#define WIFI_H

#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include "secrets.h"  // Contains your SSID, password, and API keys
#include "sensor.h"
#include "ldr.h"

bool setup_wifi();
bool thingspeak_upload_data(const SensorData &sensor_data, const LightData &light_data);

#endif