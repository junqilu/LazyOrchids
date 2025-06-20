#ifndef LDR_H
#define LDR_H

#include <Arduino.h>

struct LightData {
  int light_reading;
};

bool read_ldr(int light_pin, LightData &data);

#endif
