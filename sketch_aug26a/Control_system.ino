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
          
          if (header.indexOf("GET /led?state=1") >= 0) {
            digitalWrite(REDPIN1, HIGH);
            //REDPIN1State = "ON";
          } else if (header.indexOf("GET /led?state=0") >= 0) {
            digitalWrite(REDPIN1, LOW);
            //REDPIN1State = "OFF";
          }
            
           String REDPIN1State = digitalRead(REDPIN1) ? "ON" : "OFF"; 
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

            /*// turning light on and off 
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(output26, LOW);
            } */
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <title> Bridge Control Version 1.0 </title>");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            
            //slider design
            client.println(".switch {position: relative; display: inline-block; width: 100px; height: 54px;}");
            client.println(".slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc;}");
            client.println(".slider:before {position: absolute; content: \"\"; height: 46px; width: 46px; left: 4px; bottom: 4px; background-color: white;}");
            client.println("input:checked + .slider {background-color: #2196F3;}");
            client.println("input:checked + .slider:before {transform: translateX(46px);}</style></head>");

            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
                     
            // divider            
            client.println("<section><hr><h4> The light is currently " + REDPIN1State + "</h4>");
            client.println("<h4> The Motor is currently </h4><h4> The bridge is currently </h4></hr></section>");
            client.println("<section><hr><h2> Motor control </h2><label class=\"switch\"><input type=\"checkbox\"><span class=\"slider\"></hr></section>");
            client.println("");
            client.println("");
            client.println("");


            // Subheading
            client.println("<body><h2>Control system</h2>");
            client.println("<body><h3>Boat traffic light #1 </h3>");

            /*//display slider
            client.println("<label class=\"switch\"> <input type=\"checkbox\" id=\"myCheck\" onchange=\"myFunction()\"");
            client.println(digitalRead(REDPIN1)?"checked":"");
            client.println("><span class=\"slider\"></span></label>");
            client.println("<p id=\"text\" style=\"display:none\">Checkbox is CHECKED!</p>");*/

            client.print("<label class=\"switch\"><input type=\"checkbox\" id=\"myCheck\" onchange=\"myFunction()\" ");
            if (digitalRead(REDPIN1)) client.print("checked");
            client.println("><span class=\"slider\"></span></label>");

            client.println("<body><h3>Boat traffic light #2 </h3>");
            client.println("<label class=\"switch\"> <input type=\"checkbox\"> <span class=\"slider\"></span></label>");

            client.println("<body><h3>Car traffic light #2 </h3>");
            client.println("<label class=\"switch\"> <input type=\"checkbox\"> <span class=\"slider\"></span></label>");

            client.println("<body><h3>Car traffic light #2 </h3>");
            client.println("<label class=\"switch\"> <input type=\"checkbox\"> <span class=\"slider\"></span></label>");


            client.println("<script>function myFunction() { var xhr = new XMLHttpRequest(); ");
            client.println("var ledstate = document.getElementById(\"myCheck\").checked?1:0;") ;
            client.println("xhr.open(\"GET\",\"/led?state=\"+ledstate,true);");
            client.println("xhr.send();}");
            //client.println("if (checkBox.checked == true){  text.style.display = \"block\";digitalWrite(REDPIN1,HIGH); } ");
            client.println("</script>");

            
            // Display current state, and ON/OFF buttons for GPIO 26  
            /*client.println("<p>GPIO 26 - Green LED: <span class=\"underline\">" + output26State + "</span></p>");
            // If the output26State is off, it displays the ON button       
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } */
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
  
} 
