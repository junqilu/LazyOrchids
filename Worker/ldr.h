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

#endif
