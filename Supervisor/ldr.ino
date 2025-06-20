#include "ldr.h"

bool read_ldr(int light_pin, LightData &data) {
  int raw_light = analogRead(light_pin);  // Read analog value from LDR

  if (!isnan(raw_light)){
    data.light_reading = raw_light;

    Serial.print("Light reading: ");
    Serial.println(raw_light);

    return true;
  }else{
    Serial.println("Light reading failed (NaN values)");
    return false;
  }
}
