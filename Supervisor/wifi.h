#ifndef WIFI_H
#define WIFI_H

#include "sensor.h"
#include "ldr.h"

bool setup_wifi();
bool thingspeak_upload_data(const SensorData &sensor_data, const LightData &light_data);

#endif