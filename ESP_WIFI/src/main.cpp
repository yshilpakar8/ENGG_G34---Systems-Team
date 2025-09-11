#include <Arduino.h>
#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "Vodafone2.4G-40284";
const char* password = "tzkdkZ73Mke9ZLq6";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Store LED state
String ledState = "off";

// Built-in LED pin (usually GPIO 2 on ESP32/ESP8266 boards)
const int ledPin = 21;  

// Timing variables
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  // Initialize the LED pin
  pinMode(ledPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(ledPin, LOW);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print IP and start server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  digitalWrite(LED_BUILTIN, HIGH);

  if (client) {                             
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          
    String currentLine = "";                

    while (client.connected() && currentTime - previousTime <= timeoutTime) {  
      currentTime = millis();
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        
        if (c == '\n') {                    
          if (currentLine.length() == 0) {
            // HTTP Response headers
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // LED control based on URL
            if (header.indexOf("GET /led/on") >= 0) {
              Serial.println("LED ON");
              ledState = "on";
              digitalWrite(ledPin, HIGH);
            } else if (header.indexOf("GET /led/off") >= 0) {
              Serial.println("LED OFF");
              ledState = "off";
              digitalWrite(ledPin, LOW);
            }
            
            // Modern HTML page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>");
            client.println("body { background: #f4f6f9; font-family: Arial, sans-serif; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }");
            client.println(".card { background: white; padding: 40px; border-radius: 16px; box-shadow: 0 4px 20px rgba(0,0,0,0.1); text-align: center; }");
            client.println("h1 { color: #333; margin-bottom: 20px; }");
            client.println(".state { font-size: 20px; margin: 20px 0; }");
            client.println(".button { display: inline-block; padding: 15px 40px; font-size: 20px; border: none; border-radius: 8px; cursor: pointer; transition: 0.3s; text-decoration: none; color: white; }");
            client.println(".on { background-color: #4CAF50; }");
            client.println(".on:hover { background-color: #45a049; }");
            client.println(".off { background-color: #f44336; }");
            client.println(".off:hover { background-color: #da190b; }");
            client.println("</style></head>");
            
            client.println("<body><div class=\"card\">");
            client.println("<h1>ESP32 LED Control</h1>");
            client.println("<p class=\"state\">Built-in LED is <strong>" + ledState + "</strong></p>");
            
            if (ledState == "off") {
              client.println("<p><a href=\"/led/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/led/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            
            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
