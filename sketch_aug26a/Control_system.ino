#include <WiFi.h>
#include <ESP32Servo.h>

const int REDPIN1= 33;

const char* ssid = "ESP32_G34";
const char* password = "123456789";

WiFiServer Server(80);

String header;



unsigned long currentTime = millis();
unsigned long prevTime = 0;
const long timeout = 2000;

void setup() {
  
  Serial.begin(115200);
  pinMode(REDPIN1, OUTPUT);
  digitalWrite(REDPIN1,LOW);
  
  WiFi.mode(WIFI_AP);
  
  WiFi.softAP(ssid, password);
  Serial.print("Connecting to WiFi ..");

  Serial.println("IP Address");
  Serial.println(WiFi.softAPIP());

  

  Server.begin();
}

void loop () {
  WiFiClient client = Server.available();   // Listen for incoming clients

  x
  
} 
