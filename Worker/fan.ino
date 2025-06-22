#include "fan.h"


void setup_fan(int fan_pin){
  pinMode(fan_pin, OUTPUT);

  // Set Timer2 to fast PWM with no prescaler → ~31.25 kHz (inaudible). This will largely reduce the noises. Please ensure that you're using D3 for the fan and your sketch doesn't use tone()--Timer2 controls D3 and D11 and tone()
  TCCR2B = TCCR2B & 0b11111000 | 0x01;
}

void activate_fan(int fan_pin) {
  // Some more powerful motor (like reduction motors) need higher speed; otherwise the fan doesn't rotate unless you rotate it first  
  if (!fan_current_on) { // If the fan is currently already running, then no need for the kick start 
    analogWrite(fan_pin, 200); // Give a quick kick start. Even with the 2-pin motor, a speed = 100 can still sometimes not start the fan properly 
    delay(100);

    fan_current_on = true; 
  }  
  
  analogWrite(fan_pin, 100); // This is the speed that the fan will run
}

void deactivate_fan(int fan_pin) {
  if (fan_current_on){
    fan_current_on = false;
  }
  
  analogWrite(fan_pin, 0);
}

void routine_fan_run(int fan_pin, int mister_pin, const TreatmentPlan &treatment_plan) {
  if (treatment_plan.sleep_mode){
    Serial.println("Fan skipped due to sleep mode");
    return; // Ban the fan under sleep mode because the fan is pretty loud
  }else{
    unsigned long now = millis();
    unsigned long interval = treatment_plan.fan_interval;

    if (!fan_forced_on && digitalRead(mister_pin) == LOW) { // If the fan routine haven't run the fan and the mister is deactivated 
      if (now - last_fan_run >= interval) {
        Serial.println("Scheduled fan on!");

        activate_fan(fan_pin);
        fan_forced_on = true;
        fan_duration = treatment_plan.fan_duration;
        last_fan_run = now;
      }
    } else if (fan_forced_on && now - last_fan_run >= fan_duration) {
      Serial.println("Scheduled fan off!");

      deactivate_fan(fan_pin);
      fan_forced_on = false;
    }else {
      
    }
  }
}
