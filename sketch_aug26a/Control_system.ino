
#include <WiFi.h>

const char* ssid = "network_here"
const char* password = "password_here"

WifiServer server(80);

String header;

unsigned long currentTime = millis();
unsigned long prevTime = 0;
const long timeout = 2000;

void setup() {
  Serial.begin(115200);
  pinMode(5,OUTPUT); //may not need
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }

  Serial.println("IP Address");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  
}