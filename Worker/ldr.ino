#include "ldr.h"

TreatmentPlan treatment_plan = {true, false, 85, 90, 20UL * 60UL * 1000UL, 3UL * 60UL * 1000UL}; // default settings
unsigned long lastLightCheck = 0;
const unsigned long lightCheckIntervalSeconds = 1800; // 30 min in seconds is 1800 here
const unsigned long lightCheckInterval = lightCheckIntervalSeconds * 1000UL;

bool get_treatment_plan(int light_pin, TreatmentPlan &treatment_plan) { // For a set frequency determined by the check interval etc, treatment_plan will be updated based on the light_pin's (aka photocell's) reading
  if (millis() - lastLightCheck >= lightCheckInterval || lastLightCheck == 0) {
    int light_value = analogRead(light_pin); // Should be an integer from 0 to 1023
    // Serial.println(light_value);

    if (light_value > 500) { // The threshold for light_value is determined by the supervisor's LDR data over 3 days. The different humidities below follow the ranges in Chiang Mai, Thailand
      // Day time
      treatment_plan.is_day = true;
      treatment_plan.sleep_mode = false;
      Serial.println("Day time");

      // Thresholds (adjust as needed). Notice that humidity_min < humidity_max
      treatment_plan.humidity_min = 85;
      treatment_plan.humidity_max = 90;

      // In days, run the fan 3 min per 20 min 
      treatment_plan.fan_interval = 20UL * 60UL * 1000UL;  // 20 min
      treatment_plan.fan_duration = 3UL * 60UL * 1000UL;   // 3 min
      
    } else {
      // Night time
      treatment_plan.is_day = false;

      if (light_value > 20) { // 20 is a good value to use when you only open your table lamps. Once you turn on the overhang lights, light_value will be about 35-40 
        treatment_plan.sleep_mode = false;

        Serial.println("Night time");
      }else { // Enter sleep mode when it's super dark. Under sleep mode, the fan will be banned
        treatment_plan.sleep_mode = true;

        Serial.println("Sleep mode"); 
      }
      
      // Thresholds (adjust as needed). Notice that humidity_min < humidity_max
      treatment_plan.humidity_min = 90;
      treatment_plan.humidity_max = 95;

      // At nights, run the fan 3 min per 20 min 
      treatment_plan.fan_interval = 30UL * 60UL * 1000UL;  // 30 min
      treatment_plan.fan_duration = 2UL * 60UL * 1000UL;   // 2 min
    }
    lastLightCheck = millis();
  }
  return true;
}
