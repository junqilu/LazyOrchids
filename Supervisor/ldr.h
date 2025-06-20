#ifndef LDR_H
#define LDR_H

#include <Arduino.h>
#include <ThingSpeak.h>
#include "secrets.h"
#include "timer.h"

struct LightData {
  int light_reading;
};


bool read_ldr(int light_pin, LightData &light_data);
bool data_upload_ldr(int light_pin, NonBlockingTimer &ldr_timer, int ldr_field_light); 


#endif
