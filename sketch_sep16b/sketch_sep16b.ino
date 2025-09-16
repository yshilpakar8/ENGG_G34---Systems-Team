// === Traffic light colors ===
#define COLOR_RED     1
#define COLOR_YELLOW  2
#define COLOR_GREEN   3

// === ESP32 Pin assignments ===
const int trigPin   = 17;   // Ultrasonic sensor TRIG
const int echoPin   = 16;   // Ultrasonic sensor ECHO

const int redPin    = 2;    // LED for Red traffic light
//const int yellowPin = 3;    // LED for Yellow traffic light
const int greenPin  = 4;    // LED for Green traffic light

// Distance threshold for boat detection (in cm)
const int boatThreshold = 50; 

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(redPin, OUTPUT);
  //pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  int distance = getDistance();

  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance > 0 && distance < boatThreshold) {
    // 🚢 Boat approaching → Red
    setTrafficLights(COLOR_RED);
    delay(5000);

    // 🚦 Yellow (warning)
    //setTrafficLights(COLOR_YELLOW);
    //delay(2000);

    // 🚦 Green → cars go
    setTrafficLights(COLOR_GREEN);

  } else {
    // 🚦 Normal condition = Green
    setTrafficLights(COLOR_GREEN);
  }

  delay(500);
}

// --- Measure distance from ultrasonic ---
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2; // convert to cm
  return distance;
}

// --- Control traffic lights ---
void setTrafficLights(int color) {
  digitalWrite(redPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, LOW);

  if (color == COLOR_RED) {
    digitalWrite(redPin, HIGH);
  } else if (color == COLOR_YELLOW) {
    digitalWrite(yellowPin, HIGH);
  } else if (color == COLOR_GREEN) {
    digitalWrite(greenPin, HIGH);
  }
}
