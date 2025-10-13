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
}

void loop() {

  // Send 10 µs pulse to TRIG
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
}

