#include "wifi.h"

static WiFiClient client;

bool setup_wifi(){
  WiFi.begin(SECRET_SSID, SECRET_PASS);
  Serial.print("Connecting to WiFi: ");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("| Connected to WiFi with IP Address ");
  Serial.print(WiFi.localIP());

  ThingSpeak.begin(client);

  return true;
}

bool thingspeak_upload_data(const SensorData &sensor_data, const LightData &light_data){
  
  ThingSpeak.setField(1, sensor_data.temperature);
  ThingSpeak.setField(2, sensor_data.humidity);
  ThingSpeak.setField(3, light_data.light_reading);

  int result = ThingSpeak.writeFields(SECRET_CH_ID, SECRET_WRITE_APIKEY);

  if (result == 200) {
    Serial.println("Data sent to ThingSpeak.");
    return true;
  } else {
    Serial.print("Error sending data: ");
    Serial.println(result);

    return false;
  }
}