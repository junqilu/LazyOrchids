#ifndef LDR_H
#define LDR_H

struct TreatmentPlan {
  bool is_day;
  bool sleep_mode; // I have the sleep mode because the fan motor is a bit loud when running and thus, I don't want it to run when I'm sleeping (aka very dark and can be determined by the LDR)

  float humidity_min;
  float humidity_max;
  
  unsigned long fan_interval;
  unsigned long fan_duration;
};

bool get_treatment_plan(int light_pin, TreatmentPlan &treatment_plan);

TreatmentPlan treatment_plan = {true, false, 85, 90, 20UL * 60UL * 1000UL, 3UL * 60UL * 1000UL}; // default settings
unsigned long last_light_check = 0;
extern const unsigned long light_check_interval_seconds; 
const unsigned long light_check_interval = light_check_interval_seconds * 1000UL;

// Refer to the global variables provided in the Worker.ino
extern const int day_humidity_min;
extern const int day_humidity_max;
extern const unsigned long day_fan_interval_min;
extern const unsigned long day_fan_duration_min;

extern const int day_night_light_threshold;

extern const int night_humidity_min;
extern const int night_humidity_max;
extern const unsigned long night_fan_interval_min;
extern const unsigned long night_fan_duration_min;

extern const int night_sleep_light_threshold;


#endif
