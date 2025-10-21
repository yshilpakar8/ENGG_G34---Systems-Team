#include <ESP32Servo.h> 

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
const int greenPinCAR1 = 13;                
const int redPinCAR2 = 22;
const int greenPinCAR2 = 21;

// Boom Gate Variables
Servo boomgate1;
Servo boomgate2;

boolean gateClosed = false;

// h-bridge pins
int IN1 = 18;
int IN2 = 19;

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

void liftBridge() {
  // Close road boom gate BEFORE lifting bridge
  Serial.println("=======Closing Gate========");
  for (int i = 90; i >= 0; i--) {
    boomgate1.write(i);
    boomgate2.write(i);
    delay(20);
  }
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
}

void lowerBridge() {
  // Serial message indicating bridge is being lowered
  Serial.println("======Lowering Brdige======");
  // Open road boom gate AFTER lowering bridge
  Serial.println("=======Opening Gate========");
  for (int i = 0; i <= 90; i++) {
    boomgate1.write(i);
    boomgate2.write(i);
    delay(20);
  }
  // Rotates the Motor A counter-clockwise
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  // Keep motor running until the bridge is fully lowered
  delay(liftTime);                  
  // Stop Motor A
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  delay(500);
  // Change state variable to reflect the bridge is lowered
  state = 0;
}

void setBoatSignalLights(int color) {
  if (color == COLOR_RED) {
    digitalWrite(redPinBOAT1, HIGH);
    digitalWrite(redPinBOAT2, HIGH);
    digitalWrite(greenPinBOAT1, LOW);
    digitalWrite(greenPinBOAT2, LOW);
  } else if (color == COLOR_GREEN) {
    digitalWrite(greenPinBOAT1, HIGH);
    digitalWrite(greenPinBOAT2, HIGH); // fix: drive green boat LEDs when GREEN
    digitalWrite(redPinBOAT1, LOW);
    digitalWrite(redPinBOAT2, LOW);
  }
}

void setCarSignalLights(int color) {
  if (color == COLOR_RED) {
    digitalWrite(redPinCAR1, HIGH);
    digitalWrite(redPinCAR2, HIGH);
    digitalWrite(greenPinCAR1, LOW);
    digitalWrite(greenPinCAR2, LOW);
  } else if (color == COLOR_GREEN) {
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

}

void loop() {  
  distance1 = measureDistance(trigPin1, echoPin1, duration1);
  distance2 = measureDistance(trigPin2, echoPin2, duration2);
  
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
  // Prints the distance on the Serial Monitor
  Serial.print("Distance1: ");
  Serial.println(distance1);
  Serial.print("Distance2: ");
  Serial.println(distance2);
  Serial.print("State: ");
  Serial.println(state);
  delay(500);
}














