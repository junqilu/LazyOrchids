#include "ldr.h"


static bool first_ldr_upload = true;



bool read_ldr(int light_pin, LightData &light_data) {
  int raw_light = analogRead(light_pin);  // Read analog value from LDR

  if (!isnan(raw_light)){
    light_data.light_reading = raw_light;

    Serial.print("Light reading: ");
    Serial.println(raw_light);

    return true; // Always returns true; could add error checking later
  }else{
    Serial.println("Light reading failed (NaN values)");
    return false;
  }
}

bool data_upload_ldr(int light_pin, NonBlockingTimer &ldr_timer, int ldr_field_light) {
  if (first_ldr_upload || ldr_timer.is_ready()) {
    LightData light_data;

    if (read_ldr(light_pin, light_data)) {
      ThingSpeak.setField(ldr_field_light, light_data.light_reading);
      int result = ThingSpeak.writeFields(SECRET_CH_ID, SECRET_WRITE_APIKEY);

      if (result == 200) {
        Serial.println("Light data sent to ThingSpeak.");
      } else {
        Serial.print("Error sending light data: ");
        Serial.println(result);
      }

      if (first_ldr_upload) {
        ldr_timer.reset();
        first_ldr_upload = false;
      }
    }
    return true;
  }else {
    return false;
  }
}
