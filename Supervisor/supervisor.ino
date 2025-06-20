#include <Adafruit_SHT31.h>



#include "wifi.h"
#include "timer.h"
#include "sensor.h"
#include "ldr.h"



// Hardware configuration 
const int light_pin = A0;  // Which ever pin you defined for the LDR to detect light 

// Upload configuration
const int sensor_field_temperature = 1;
const int sensor_field_humidity = 2;
const unsigned long sensor_upload_interval = 16000; // 15-second interval. ThingSpeak allows you to send data once per 15 sec as a free tier user as the fastest speed
NonBlockingTimer sensor_timer(sensor_upload_interval);

const int ldr_field_light = 3;
const unsigned long ldr_upload_interval = 32000;
NonBlockingTimer ldr_timer(ldr_upload_interval);



void setup() {
  Serial.begin(9600);
  if (Serial) { // This blocks ensure that nano can upload data even if it's connected to a power supply rather than your laptop 
    while (!Serial);
  }

  setup_wifi();

  if (!setup_sensor()) {
    while (1); // halt if sensor setup failed
  }
}

void loop() {
  data_upload_sensor(sensor_timer, sensor_field_temperature, sensor_field_humidity);
  data_upload_ldr(light_pin, ldr_timer, ldr_field_light);
}
