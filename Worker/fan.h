#ifndef FAN_H
#define FAN_H


extern bool fan_forced_on;
extern bool fan_current_on; 
unsigned long last_fan_run = 0;
unsigned long fan_duration = 0;
bool fan_forced_on = false; // It tracks whether the fan is running due to the periodic fan routine, not the humidity balancing logic
bool fan_current_on = false; // It tracks whether the fan is currently running, no matter it's because of the periodic fan routine or humidity balancing logic

void setup_fan(int fan_pin);
void activate_fan(int fan_pin);
void deactivate_fan(int fan_pin);
void routine_fan_run(int fan_pin, int mister_pin, const TreatmentPlan &treatment_plan); 

#endif