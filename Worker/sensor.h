#ifndef SENSOR_H
#define SENSOR_H

#include <Adafruit_SHT31.h>

struct SensorData {
    float temperature;
    float humidity;
};

bool setup_sensor();
bool read_sensor(SensorData &sensor_data);


#endif