void setup_fan(int fan_pin){
  pinMode(fan_pin, OUTPUT);
}

void activate_fan(int fan_pin) {
  analogWrite(fan_pin, 100); // Drop to target speed
  // Some more powerful motor (like reduction motors) need higher speed; otherwise the fan doesn't rotate unless you rotate it first  
}

void deactivate_fan(int fan_pin) {
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
