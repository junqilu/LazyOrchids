#include "ldr.h"

bool get_treatment_plan(int light_pin, TreatmentPlan &treatment_plan) { // For a set frequency determined by the check interval etc, treatment_plan will be updated based on the light_pin's (aka photocell's) reading
  if (millis() - last_light_check >= light_check_interval || last_light_check == 0) {
    int light_value = analogRead(light_pin); // Should be an integer from 0 to 1023
    // Serial.println(light_value);

    if (light_value > day_night_light_threshold) { // The threshold for light_value is determined by the supervisor's LDR data over 3 days. The different humidities below follow the ranges in Chiang Mai, Thailand
      // Day time
      treatment_plan.is_day = true;
      treatment_plan.sleep_mode = false;
      Serial.println("Day time");

      // Thresholds (adjust as needed). Notice that humidity_min < humidity_max
      treatment_plan.humidity_min = day_humidity_min;
      treatment_plan.humidity_max = day_humidity_max;

      // Fan schedule for the day time 
      treatment_plan.fan_interval = day_fan_interval_min * 60UL * 1000UL;  
      treatment_plan.fan_duration = day_fan_duration_min * 60UL * 1000UL;   
      
    } else {
      // Night time
      treatment_plan.is_day = false;

      if (light_value > night_sleep_light_threshold) { // 20 is a good value to use when you only open your table lamps. Once you turn on the overhang lights, light_value will be about 35-40 
        treatment_plan.sleep_mode = false;

        Serial.println("Night time");
      }else { // Enter sleep mode when it's super dark. Under sleep mode, the fan will be banned
        treatment_plan.sleep_mode = true;

        Serial.println("Sleep mode"); 
      }
      
      // Thresholds (adjust as needed). Notice that humidity_min < humidity_max
      treatment_plan.humidity_min = night_humidity_min;
      treatment_plan.humidity_max = night_humidity_max;

      // Fan schedule for the night time 
      treatment_plan.fan_interval = night_fan_interval_min * 60UL * 1000UL;  
      treatment_plan.fan_duration = night_fan_duration_min * 60UL * 1000UL;  
    }
    last_light_check = millis();
  }
  return true;
}
