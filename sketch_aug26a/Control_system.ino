
#include <WiFi.h>

const char* ssid = "network_here"
const char* password = "password_here"

void setup() {
  Serial.begin();
  pinMode(5,OUTPUT); //may not need
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void loop() {

}