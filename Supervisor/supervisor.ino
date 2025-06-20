#include <Adafruit_SHT31.h>

#include "wifi.h"
#include "sensor.h"
#include "ldr.h"
#include "timer.h"



const int light_pin = A0;  // Which ever pin you defined for the LDR to detect light

NonBlockingTimer upload_timer(15000);  // 15-second interval. ThingSpeak allows you to send data once per 15 sec as a free tier user as the fastest speed
bool first_upload_flag = true; //Use a first_upload_flag so the first time upload can be run right away. Otherwise, you need to wait for 15 sec before the 1st upload



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
  SensorData sensor_data;
  LightData light_data;

  if (first_upload_flag || upload_timer.is_ready()) {
    if (read_sensor(sensor_data) && read_ldr(light_pin, light_data)) {
      thingspeak_upload_data(sensor_data, light_data);

      if (first_upload_flag) {
        upload_timer.reset();  // Start the interval after the first upload
        first_upload_flag = false;
      }
    }
  }
}