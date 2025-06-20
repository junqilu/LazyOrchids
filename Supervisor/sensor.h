#ifndef SENSOR_H
#define SENSOR_H

#include <Adafruit_SHT31.h>
#include <ThingSpeak.h>
#include "secrets.h"
#include "timer.h"

struct SensorData {
  float temperature;
  float humidity; 
};

bool setup_sensor();
bool read_sensor(SensorData &sensor_data);
bool data_upload_sensor(NonBlockingTimer &sensor_timer, int sensor_field_temperature, int sensor_field_humidity);


#endif
