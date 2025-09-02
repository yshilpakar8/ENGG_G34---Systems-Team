// C++ code
//

// sensor pins
const int trigPin1 = 9;
const int echoPin1 = 10;

const int trigPin2 = 5;
const int echoPin2 = 11;

// h-bridge pins
const int in1Pin = 13;
const int forwardPin = 12;
const int backwardPin = 13;

const int out1Pin = 2;
const int out2Pin = 4;

const int motPin = 6;
int state; // 1 = up 0 = down

// defines variables
long duration1;
int distance1;

long duration2;
int distance2;

int threshold = 150;



void setup() {
  pinMode(motPin, OUTPUT);
  pinMode(forwardPin, OUTPUT);
  pinMode(backwardPin, OUTPUT);
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT);
   pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT);// Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  state = 0;
}

void loop() {
  
  // Measure first distance
  // Clears the trigPin
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration1 = pulseIn(echoPin1, HIGH);
  // Calculating the distance
  distance1 = duration1 * 0.034 / 2;

  delay(50);
  
  // Measure second distance
  // Clears the trigPin
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration2 = pulseIn(echoPin2, HIGH);
  // Calculating the distance
  distance2 = duration2 * 0.034 / 2;

  
  if(distance1 <= threshold && state == 0) {
    digitalWrite(forwardPin, LOW);
	digitalWrite(backwardPin, HIGH);
    delay(5000);
    state = 1;
    
  } else if(state == 1 && distance1 > threshold && distance2 == threshold){
    delay(1000);
    digitalWrite(forwardPin, HIGH);
	digitalWrite(backwardPin, LOW);
    delay(5000);
    state = 0;
  } else {
    digitalWrite(backwardPin, LOW);
    digitalWrite(forwardPin, LOW);
  }
  // Prints the distance on the Serial Monitor
  Serial.print("Distance1: ");
  Serial.println(distance1);
  Serial.print("Distance2: ");
  Serial.println(distance2);
}



