#include <Wire.h>
#include "Adafruit_SHT31.h"
#include "ldr.h"
#include "sensor.h"
#include "fan.h"

const int fan_pin = 5; // Connect to D5 
const int mister_pin = 3; // Connect to D3 
const int light_pin = A0;  // LDR

unsigned long last_fan_run = 0;
unsigned long fan_duration = 0;
bool fan_forced_on = false; // It tracks whether the fan is running due to the periodic fan routine, not the humidity balancing logic


void setup() {
  Serial.begin(9600);

  if (!setup_sensor()) {
    while (1); // halt if sensor setup failed
  }

  setup_fan(fan_pin);
  setup_mister(mister_pin);


  deactivate_fan(fan_pin);
  deactivate_mister(mister_pin);
}



void balance_humidity(const SensorData &sensor_data, const TreatmentPlan &treatment_plan) {
  if (!fan_forced_on) { // Avoid running fan and mister at the same time 
    if (sensor_data.humidity < treatment_plan.humidity_min) {
      deactivate_fan(fan_pin);

      Serial.println("Mister activated!");
      activate_mister(mister_pin);
    } else if (sensor_data.humidity > treatment_plan.humidity_max) {
      deactivate_mister(mister_pin);
      
      if (treatment_plan.sleep_mode){
        Serial.println("Fan supressed due to sleep mode!");
        deactivate_fan(fan_pin);
      }else{
        Serial.println("Fan activated!");
        activate_fan(fan_pin);
      }  
    } else {
      Serial.println("Humidity OK. Everything off.");

      deactivate_mister(mister_pin);
      deactivate_fan(fan_pin);
    }
  } else {
    deactivate_mister(mister_pin);
  }
}


void loop() {
  TreatmentPlan treatment_plan;
  SensorData sensor_data;

  get_treatment_plan(light_pin, treatment_plan);
  read_sensor(sensor_data);

  routine_fan_run(fan_pin, mister_pin, treatment_plan);
  balance_humidity(sensor_data, treatment_plan);

  delay(5000);  // Check humidity every 5 seconds
}
