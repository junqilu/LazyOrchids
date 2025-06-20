#include "sensor.h"

static Adafruit_SHT31 sht = Adafruit_SHT31();
static bool first_sensor_upload = true;

bool setup_sensor() {
  if (!sht.begin(0x44)) { // 0x44 is the default I2C address
    Serial.println("Couldn't find SHT3x sensor!");
    return false;
  }
  return true;
}

bool read_sensor(SensorData &sensor_data) {
  float raw_temperature = sht.readTemperature();
  float raw_humidity = sht.readHumidity();

  if (!isnan(raw_temperature) && !isnan(raw_humidity)) {
    sensor_data.temperature = raw_temperature;
    sensor_data.humidity = raw_humidity;

    Serial.println("");
    Serial.print("Temperature: ");
    Serial.print(raw_temperature);
    Serial.print(" °C | Humidity: ");
    Serial.print(raw_humidity);
    Serial.println(" %");

    return true;
  }else{
    Serial.println("Sensor reading failed (NaN values)");
    return false;
  }
}

bool data_upload_sensor(NonBlockingTimer &sensor_timer, int sensor_field_temperature, int sensor_field_humidity){
  if (first_sensor_upload || sensor_timer.is_ready()) {
    SensorData sensor_data;

    if (read_sensor(sensor_data)) {
      ThingSpeak.setField(sensor_field_temperature, sensor_data.temperature);
      ThingSpeak.setField(sensor_field_humidity, sensor_data.humidity);
      int result = ThingSpeak.writeFields(SECRET_CH_ID, SECRET_WRITE_APIKEY);

      if (result == 200) {
        Serial.println("Sensor data sent to ThingSpeak.");
      } else {
        Serial.print("Error sending sensor data: ");
        Serial.println(result);
      }

      if (first_sensor_upload) {
        sensor_timer.reset(); 
        first_sensor_upload = false;
      }
    }
    return true; 
  }else {
    return false;
  }
}


