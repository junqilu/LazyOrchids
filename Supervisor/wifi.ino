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
