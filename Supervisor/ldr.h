#ifndef LDR_CONTROL_H
#define LDR_CONTROL_H

struct LightData {
  int light_reading;
};

bool read_ldr(int light_pin, LightData &data);

#endif
