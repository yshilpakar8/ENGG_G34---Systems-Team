#include <ESP32Servo.h> 

const int trigPin = 14;   
const int echoPin = 34;   

//for boats 
const int REDPIN1= 32;
const int GREENPIN1= 33; 
const int REDPIN2 = 18;
const int GREENPIN2 = 27;

//for cars 
const int REDPIN3 = 12;
const int GREENPIN3 = 13;                
const int REDPIN4 = 25;
const int GREENPIN4 = 26;

Servo boomgate1;
Servo boomgate2;

boolean gateClosed = false;

void setup() {

  pinMode(REDPIN1, OUTPUT);
  pinMode(GREENPIN1, OUTPUT);

  pinMode(REDPIN3, OUTPUT);
  pinMode(GREENPIN3, OUTPUT);

  pinMode(REDPIN2, OUTPUT);
  pinMode(GREENPIN2, OUTPUT);

  pinMode(REDPIN4, OUTPUT);
  pinMode(GREENPIN4, OUTPUT);

  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
 
  boomgate1.attach(26);
  boomgate2.attach(25);

  //motor is in 90 degree
  boomgate1.write(90); 
  boomgate2.write(90);
  

  // initial lights condition when gate is open:
  // cars green on, red off 
  digitalWrite(GREENPIN3, HIGH);
  digitalWrite(REDPIN3, LOW);
  digitalWrite(GREENPIN4, HIGH);
  digitalWrite(REDPIN4, LOW);

  // boats red on, green off
  digitalWrite(REDPIN1, HIGH);
  digitalWrite(GREENPIN1, LOW);
  digitalWrite(REDPIN2, HIGH);
  digitalWrite(GREENPIN2, LOW);

   Serial.begin(115200);
  pinMode(5,OUTPUT); //may not need
  WiFi.mode(WIFI_AP);
  
  WiFi.softAP(ssid, password);
  Serial.print("Connecting to WiFi ..");

  Serial.println("IP Address");
  Serial.println(WiFi.softAPIP());

  Server.begin();
}

void loop() {

   digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure pulse length on ECHO
  long duration = pulseIn(echoPin, HIGH);

  //Serial.print("Duration: ");
  //Serial.print(duration);
  //Serial.println(" microseconds");

  // Convert to distance in cm
  float distance = duration * 0.0343 / 2;

  //Print result
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // if something is close to the sensor
  boolean isClose = (distance <= 15 && distance > 0);

   // if there is an object change trafficlights
  if (isClose) {
    // cars red on, green off 
    digitalWrite(REDPIN3, HIGH);
    digitalWrite(GREENPIN3, LOW);
    digitalWrite(REDPIN4, HIGH);
    digitalWrite(GREENPIN4, LOW);

    // boats green on, red off 
    digitalWrite(GREENPIN1, HIGH);
    digitalWrite(REDPIN1, LOW);
    digitalWrite(GREENPIN2, HIGH);
    digitalWrite(REDPIN2, LOW);
  } else {
    // cars green on, red off 
    digitalWrite(GREENPIN3, HIGH);
    digitalWrite(REDPIN3, LOW);
    digitalWrite(GREENPIN4, HIGH);
    digitalWrite(REDPIN4, LOW);

    // boats red on, green off
    digitalWrite(REDPIN1, HIGH);
    digitalWrite(GREENPIN1, LOW);
    digitalWrite(REDPIN2, HIGH);
    digitalWrite(GREENPIN2, LOW);
  }

  if (isClose) {
    
    // check if there is object then close the gate
    if (gateClosed == false) {
      for (int i = 90; i >= 0; i--) {
        boomgate1.write(i);
        boomgate2.write(i);
        delay(20);
      }
      
      gateClosed = true;
    }
  } else {
    
    // if gate was closed and no object open it
    if (gateClosed == true) {
      for (int i = 0; i <= 90; i++) {
        boomgate1.write(i);
        boomgate2.write(i);
        delay(20);
      }
      
      gateClosed = false;
    }
  }

  delay(200);

  WiFiClient client = Server.available();  
   // Listen for incoming clients

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
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // // turns the GPIOs on and off - if pins not in use delete ( shows link on website)
            // if (header.indexOf("GET /26/on") >= 0) {
            //   Serial.println("GPIO 26 on");
            //   //output26State = "on";
            //   digitalWrite(output26, HIGH);
            // } else if (header.indexOf("GET /26/off") >= 0) {
            //   Serial.println("GPIO 26 off");
            //   output26State = "off";
            //   digitalWrite(output26, LOW);
            // } else if (header.indexOf("GET /27/on") >= 0) {
            //   Serial.println("GPIO 27 on");
            //   output27State = "on";
            //   digitalWrite(output27, HIGH);
            // } else if (header.indexOf("GET /27/off") >= 0) {
            //   Serial.println("GPIO 27 off");
            //   output27State = "off";
            //   digitalWrite(output27, LOW);
            // }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <title> Bridge Control Version 1.0 </title>");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            //client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            //client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            //client.println(".button2 {background-color: #555555;}</style></head>");

            //slider design
            client.println(".switch {position: relative; display: inline-block; width: 100px; height: 54px;}");
            client.println(".slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc;}");
            client.println(".slider:before {position: absolute; content: \"\"; height: 46px; width: 46px; left: 4px; bottom: 4px; background-color: white;}");
            client.println("input:checked + .slider {background-color: #2196F3;}");
            client.println("input:checked + .slider:before {transform: translateX(46px);}</style></head>");

            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // divider
            
            client.println("<hr>");

            client.println("<h2> Bridge status </h2>");
            client.println("<section><hr><h4> The lights are currently </h4><h4> The Motor is currently </h4><h4> The bridge is currently </h4></hr></section>");
            client.println("<section><hr><h2> Motor control </h2><label class=\"switch\"><input type=\"checkbox\"><span class=\"slider\"></hr></section>");
            client.println("");
            client.println("");
            client.println("");


            // Subheading

            client.println("<body><h2>Control system</h2>");
            client.println("<body><h3>Boat traffic light #1 </h3>");

            //display slider
            client.println("<label class=\"switch\"> <input type=\"checkbox\" id=\"myCheck\" onclick=\"myFunction()\"> <span class=\"slider\"></span></label>");
            client.println("<p id=\"text\" style=\"display:none\">Checkbox is CHECKED!</p>");

            client.println("<body><h3>Boat traffic light #2 </h3>");
           client.println("<label class=\"switch\"> <input type=\"checkbox\"> <span class=\"slider\"></span></label>");

            client.println("<body><h3>Car traffic light #2 </h3>");
            client.println("<label class=\"switch\"> <input type=\"checkbox\"> <span class=\"slider\"></span></label>");

            client.println("<body><h3>Car traffic light #2 </h3>");
            client.println("<label class=\"switch\"> <input type=\"checkbox\"> <span class=\"slider\"></span></label>");

            client.println("<script>function myFunction() { var checkBox = document.getElementById(\"myCheck\"); var text = document.getElementById(\"text\"); ");
            client.println("if (checkBox.checked == true){  text.style.display = \"block\";digitalWrite(REDPIN1,HIGH); alert(\"You are overriding sensor commands, proced with care\") ;} ");
            Serial.println("check");
            client.println("else { text.style.display = \"none\"; digitalWrite(REDPIN1,LOW); }}</script>");
            // // Display current state, and ON/OFF buttons for GPIO 26  
            // client.println("<p>GPIO 26 - State " + output26State + "</p>");
            // // If the output26State is off, it displays the ON button       
            // if (output26State=="off") {
            //   client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            // } else {
            //   client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            // } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            // client.println("<p>GPIO 27 - State " + output27State + "</p>");
            // // If the output27State is off, it displays the ON button       
            // if (output27State=="off") {
            //   client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            // } else {
            //   client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            // }
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
  // Send 10 µs pulse to TRIG
 
}

