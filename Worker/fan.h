#ifndef SENSOR_H
#define SENSOR_H

extern unsigned long last_fan_run;
extern unsigned long fan_duration;
extern bool fan_forced_on;


void setup_fan(int fan_pin);
void activate_fan(int fan_pin);
void deactivate_fan(int fan_pin);
void routine_fan_run(int fan_pin, int mister_pin, const TreatmentPlan &treatment_plan); 

#endif