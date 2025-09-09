// C++ code
//

// sensor pins
const int trigPin = 9;
const int echoPin = 10;

const int trigPin2 = 5;
const int echoPin2 = 11;

// h-bridge pins
const int in1Pin = 13;
const int forwardPin = 12;
const int backwardPin = 13;

const int motPin = 11;

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
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance = duration * 0.034 / 2;
  
  if(distance <= threshold && distance != 0) {
    digitalWrite(motPin, HIGH);
    Serial.print("Distance: ");
    Serial.println(distance);
  } else if(distance > threshold && distance != 0) {
    digitalWrite(motPin, LOW);
    Serial.print("Distance: ");
    Serial.println(distance);
  } 
  /*
  //state = 0;
  if(distance <= threshold && state == 0) {
    digitalWrite(forwardPin, LOW);
	digitalWrite(backwardPin, HIGH);
    delay(5000);
    state = 1;
    
  } else if(state == 1 && distance > threshold){
    delay(1000);
    digitalWrite(forwardPin, HIGH);
	digitalWrite(backwardPin, LOW);
    delay(5000);
    state = 0;
  } else {
    digitalWrite(backwardPin, LOW);
    digitalWrite(forwardPin, LOW);
  }
  */
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
}



