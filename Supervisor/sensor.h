#ifndef SENSOR_H
#define SENSOR_H

struct SensorData {
  float temperature;
  float humidity; 
};

bool setup_sensor();
bool read_sensor(SensorData &sensor_data);


#endif