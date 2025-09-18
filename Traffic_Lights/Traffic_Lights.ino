const int trigPin = 14;   // TRIG on GPIO14
const int echoPin = 27;   // ECHO on GPIO27
//for boats
const int REDPIN= 25;
const int GREENPIN= 33; 

//for cars
const int REDPIN2 = 26;
const int GREENPIN2 = 13;                

void setup() {

  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);

  pinMode(REDPIN2, OUTPUT);
  pinMode(GREENPIN2, OUTPUT);

  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
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

  // Convert to distance in cm
  float distance = duration * 0.0343 / 2;

  //Print result
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

if (duration > 0 && distance <= 15) {
   digitalWrite(REDPIN, HIGH);
   digitalWrite(GREENPIN2, HIGH);
   digitalWrite(GREENPIN, LOW);
   digitalWrite(REDPIN2, LOW);
   //digitalWrite(GREENPIN2, HIGH);
}else{
   //digitalWrite(REDPIN, LOW);
   digitalWrite(GREENPIN, HIGH);
   digitalWrite(REDPIN2, HIGH);
   digitalWrite(REDPIN, LOW);
   digitalWrite(GREENPIN2, LOW);
}

  delay(200);
}
