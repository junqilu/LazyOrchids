#include "sensor.h"

static Adafruit_SHT31 sht = Adafruit_SHT31();

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
