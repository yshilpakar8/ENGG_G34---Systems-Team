#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>

// Wifi Variables
const char* ssid = "ESP32_G34";
const char* password = "123456789";

WiFiServer Server(80);

String header;

unsigned long currentTime = millis();
unsigned long prevTime = 0;
const long timeout = 2000;

int webState = 0;


// === Traffic light colors ===
#define COLOR_RED     1 
#define COLOR_YELLOW  2
#define COLOR_GREEN   3

// sensor pins
const int trigPin1 = 13;   
const int echoPin1 = 35;   

const int trigPin2 = 16;   
const int echoPin2 = 34;   

// Boat led
const int redPinBOAT1    = 32;    
const int greenPinBOAT1  = 33;    
const int redPinBOAT2    = 26;    
const int greenPinBOAT2  = 27;    

// Car led 
const int redPinCAR1 = 25; 
const int greenPinCAR1 = 23;                         
const int redPinCAR2 = 22; 
const int greenPinCAR2 = 21; 

// LED States
int carLedState = COLOR_GREEN;
int boatLedState = COLOR_RED;

// Boom Gate Variables
Servo boomgate1;
Servo boomgate2;

boolean gateClosed = false;

int pinBoomGate1 = 4;
int pinBoomGate2 = 15;

// h-bridge pins
int IN1 = 12;  
int IN2 = 14;  

int state; // 1 = up 0 = down

// defines variables
long duration1 = 20;
int distance1 = 20;

long duration2 = 20;
int distance2 = 20;

const int threshold = 10;
const int liftTime = 1700;


int measureDistance(int trig, int echo, long dur) {
  // Measure first distance
  // Clears the trigPin
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  dur = pulseIn(echo, HIGH);
  // Calculating the distance
  return dur * 0.034 / 2;
}
void closeBoomGate() {
  Serial.println("=======Closing Gate========");
  for (int i = 90; i >= 0; i--) {
    boomgate1.write(i);
    boomgate2.write(i);
    delay(20);
  }
  gateClosed = true;
}

void openBoomGate() {
  Serial.println("=======Opening Gate========");
  for (int i = 0; i <= 90; i++) {
    boomgate1.write(i);
    boomgate2.write(i);
    delay(20);
  }

  gateClosed = false;
}
void liftBridge() {
  // Close road boom gate BEFORE lifting bridge
  closeBoomGate();
  // Serial Message indicating bridge is lifting
  Serial.println("======Lifting Bridge=======");
  // Rotate the Motor A clockwise
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  // Keep motor running until the bridge is fully lifted
  delay(liftTime);                    
  // Stop Motor A
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  delay(500);
  // Change state variable to reflect the bridge is lifted
  state = 1;
  Serial.println("=======Bridge Lifted=======");
}

void lowerBridge() {
  // Serial message indicating bridge is being lowered
  Serial.println("======Lowering Brdige======");
  // Rotates the Motor A counter-clockwise
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  // Keep motor running until the bridge is fully lowered
  delay(liftTime);                  
  // Stop Motor A
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  // Open road boom gate AFTER lowering bridge
  openBoomGate();
  delay(500);
  // Change state variable to reflect the bridge is lowered
  state = 0;
  Serial.println("=======Bridge Lowered=======");
}

void setBoatSignalLights(int color) {
  if (color == COLOR_RED) {
    boatLedState = COLOR_RED;
    digitalWrite(redPinBOAT1, HIGH);
    digitalWrite(redPinBOAT2, HIGH);
    digitalWrite(greenPinBOAT1, LOW);
    digitalWrite(greenPinBOAT2, LOW);
  } else if (color == COLOR_GREEN) {
    boatLedState = COLOR_GREEN;
    digitalWrite(greenPinBOAT1, HIGH);
    digitalWrite(greenPinBOAT2, HIGH); 
    digitalWrite(redPinBOAT1, LOW);
    digitalWrite(redPinBOAT2, LOW);
  }
}

void setCarSignalLights(int color) {
  if (color == COLOR_RED) {
    carLedState = COLOR_RED;
    digitalWrite(redPinCAR1, HIGH);
    digitalWrite(redPinCAR2, HIGH);
    digitalWrite(greenPinCAR1, LOW);
    digitalWrite(greenPinCAR2, LOW);
  } else if (color == COLOR_GREEN) {
    carLedState = COLOR_GREEN;
    digitalWrite(greenPinCAR1, HIGH);
    digitalWrite(greenPinCAR2, HIGH); 
    digitalWrite(redPinCAR1, LOW);
    digitalWrite(redPinCAR2, LOW);
  }
}

void setup() {
  // Motor Driver Pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Sensor Pins
  pinMode(trigPin1, OUTPUT); 
  pinMode(echoPin1, INPUT);

  pinMode(trigPin2, OUTPUT); 
  pinMode(echoPin2, INPUT);

  pinMode(redPinBOAT1, OUTPUT);
  pinMode(greenPinBOAT1, OUTPUT);

  pinMode(redPinBOAT2, OUTPUT);
  pinMode(greenPinBOAT2, OUTPUT);
  
  pinMode(redPinCAR1, OUTPUT);
  pinMode(greenPinCAR1, OUTPUT);

  pinMode(redPinCAR2, OUTPUT);
  pinMode(greenPinCAR2, OUTPUT);


  Serial.begin(115200); // Starts the serial communication with BAUD 9600
  state = 0;
  delay(2000);
  

  boomgate1.attach(pinBoomGate1);
  boomgate2.attach(pinBoomGate2);

  //motor is in 90 degree
  boomgate1.write(90); 
  boomgate2.write(90);
  
  WiFi.mode(WIFI_AP);
  
  WiFi.softAP(ssid, password);
  Serial.print("Connecting to WiFi ..");

  Serial.println("IP Address");
  Serial.println(WiFi.softAPIP());

  Server.begin();

}


void loop () {
  distance1 = measureDistance(trigPin1, echoPin1, duration1);
  distance2 = measureDistance(trigPin2, echoPin2, duration2);

  WiFiClient client = Server.available();   // Listen for incoming clients


  if (client) {                             // If a new client connects,
    currentTime = millis();
    prevTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - prevTime <= timeout) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {  


        if (webState == 1) {
            if (header.indexOf("GET /carled?state=1") >= 0) {
              setCarSignalLights(COLOR_RED);
            } else if (header.indexOf("GET /carled?state=0") >= 0) {
              setCarSignalLights(COLOR_GREEN);
            }
          }
          
          // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
           
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <title> Bridge Control Version 1.0 </title>");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
           
            //slider design
            client.println(".switch {position: relative; display: inline-block; width: 100px; height: 54px;}");
            client.println(".slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc;}");
            client.println(".slider:before {position: absolute; content: \"\"; height: 46px; width: 46px; left: 4px; bottom: 4px; background-color: white;}");
            client.println("input:checked + .slider {background-color: #2196F3;}");
            client.println("input:checked + .slider:before {transform: translateX(46px);}</style></head>");

            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");


            client.println("<body><h2> Control Switch </h2>");
            client.println("<label class=\"switch\"><input type=\"checkbox\" id=\"control\" onchange=\"ControlState()\" ");
            client.println("><span class=\"slider\"></span></label>");
                     
            // divider            
            client.println("<h4> The Motor is currently </h4><h4> The bridge is currently </h4></hr></section>");
            client.println("<section><hr><h2> Motor control </h2><label class=\"switch\"><input type=\"checkbox\"><span class=\"slider\"></hr></section>");
            client.println("");
            client.println("");
            client.println("");


            // Subheading
            client.println("<body><h2>Control system</h2>");
            client.println("<body><h3>Boat traffic light </h3>");

            client.print("<label class=\"switch\"><input type=\"checkbox\" id=\"boatCheck\" onchange=\"boatFunction()\" ");
            client.println("><span class=\"slider\"></span></label>");

            client.println("<body><h3>Car traffic light </h3>");
            client.println("<label class=\"switch\"> <input type=\"checkbox\"> <span class=\"slider\"></span></label>");

            client.print("<label class=\"switch\"><input type=\"checkbox\" id=\"carCheck\" onchange=\"carFunction()\" ");
            client.println("><span class=\"slider\"></span></label>");

            client.println("<script>function ControlState() { var xhr = new XMLHttpRequest(); ");
            client.println("var controlstate = document.getElementById(\"control\").checked?1:0;") ;
            client.println("xhr.open(\"GET\",\"/control?state=\"+controlstate,true);");
            client.println("xhr.send();}");
            client.println("</script>");


             if (header.indexOf("GET /control?state=1") >= 0) {
                webState = 1;
             } else if (header.indexOf("GET /control?state=0") >= 0) {
                webState = 0;
             }

            client.println("<script>function carFunction() { var xhr = new XMLHttpRequest(); ");
            client.println("var carledstate = document.getElementById(\"carCheck\").checked?1:0;") ;
            client.println("xhr.open(\"GET\",\"/carled?state=\"+carledstate,true);");
            client.println("xhr.send();}");
            client.println("</script>");
            client.println("</body></html>");

            client.println("<script>function boatFunction() { var xhr = new XMLHttpRequest(); ");
            client.println("var boatledstate = document.getElementById(\"boatCheck\").checked?1:0;") ;
            client.println("xhr.open(\"GET\",\"/boatled?state=\"+boatledstate,true);");
            client.println("xhr.send();}");
            client.println("</script>");
            client.println("</body></html>");
           
            // The HTTP response ends with another blank line
            client.println();

            // Break out of the while loop
            break;

          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection  
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

  if(webState == 0) {
    if(distance1 <= threshold && state == 0) {
      setBoatSignalLights(COLOR_RED);
      setCarSignalLights(COLOR_RED);
      liftBridge();
      setBoatSignalLights(COLOR_GREEN);
    } else if(state == 1 && distance1 > threshold && distance2 == threshold){
      setBoatSignalLights(COLOR_RED);
      delay(2000);// Delay lowering to allow ships to stop
      lowerBridge();
      setCarSignalLights(COLOR_GREEN);
    } else if(distance2 <= threshold && state == 0) {
      setBoatSignalLights(COLOR_RED);
      setCarSignalLights(COLOR_RED);
      liftBridge();
      setBoatSignalLights(COLOR_GREEN);
    } else if(state == 1 && distance2 > threshold && distance1 == threshold){
      setBoatSignalLights(COLOR_RED);
      delay(2000);
      lowerBridge();
      setCarSignalLights(COLOR_GREEN);
    } else if(state == 1) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      setBoatSignalLights(COLOR_GREEN);
      setCarSignalLights(COLOR_RED);
    } else {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      setBoatSignalLights(COLOR_RED);
      setCarSignalLights(COLOR_GREEN);
    }
  }

  // Prints the distance on the Serial Monitor
  Serial.println("");
  Serial.print("Distance1: ");
  Serial.println(distance1);
  Serial.print("Distance2: ");
  Serial.println(distance2);
  Serial.print("State: ");
  Serial.println(state);
  Serial.print("Manual Override: ");
  Serial.println(webState);
  Serial.print("Boom Gate Closed: ");
  Serial.println(gateClosed);
  delay(500);
}
