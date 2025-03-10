#include <Servo.h> // Include servo library

// Define the pins for the ultrasonic sensor
const int trigPin = 4; // Shared Trig pin
const int echoPinRight = 5; // Echo pin for right sensor
const int echoPinLeft  = 6; // Echo pin for left sensor

//main motor run
int PWMPin = 3; int DirPin = 2;
int power = 0;
int val;


// Create Servo Objects
Servo left_servo; 
Servo right_servo;


const int servo_left_pin = 7;
const int servo_right_pin = 8; 

char test_val = '\0';  // Initialize with a default value

// Servo positions
const int ClearValRight = 0; 
const int downValRight = 73;
const int PushValLeft = 30;  
const int downValLeft = 60;
const int ClearValLeft = 180;
const int PushValRight = 160;

// Sensor variables
long duration_right, duration_left;
long distance_mm_right, distance_mm_left;

// Filter settings
const int numSamples = 5; // Number of readings to average
long filteredDistanceRight = 0;
long filteredDistanceLeft = 0;

void setup() {
  Serial.begin(9600);

//run main motor
  pinMode(PWMPin, OUTPUT);
  pinMode(DirPin, OUTPUT);
  
  digitalWrite(DirPin, HIGH);
  analogWrite(PWMPin, power);
  Serial.print(power);
//********


  pinMode(trigPin, OUTPUT);
  pinMode(echoPinRight, INPUT);
  pinMode(echoPinLeft, INPUT);

  left_servo.attach(servo_left_pin);
  left_servo.write(downValLeft);
  
  right_servo.attach(servo_right_pin);
  right_servo.write(downValRight);

  delay(2000); // Allow servos to reach the position
}

void loop() {
  if (Serial.available()) {
    test_val = Serial.read();
  }

  // Get filtered distance readings
  filteredDistanceRight = getFilteredDistance(echoPinRight);
  filteredDistanceLeft = getFilteredDistance(echoPinLeft);

  Serial.print("Right: ");
  Serial.print(filteredDistanceRight);
  Serial.print(" mm | Left: ");
  Serial.println(filteredDistanceLeft);

  // Left sensor activated
  if (test_val == 'l' || filteredDistanceLeft < 50) {
    delay(500);
    right_servo.write(ClearValRight);
    delay(500);
    left_servo.write(PushValLeft);
    delay(1000);
    left_servo.write(downValLeft);
    right_servo.write(downValRight);

  }
  // Right sensor activated
  else if (test_val == 'r' || filteredDistanceRight < 40) {
    delay(500);
    left_servo.write(ClearValLeft);
    delay(500);
    left_servo.write(downValLeft);


    

  }

  

  delay(500);
}

// Function to get a filtered distance using multiple samples
long getFilteredDistance(int echoPin) {
  long sum = 0;
  long validSamples = 0;
  long previousReading = 0;

  for (int i = 0; i < numSamples; i++) {
    long distance = getDistance(echoPin);
    
    // Ignore outlier readings (change threshold as needed)
    if (i == 0 || abs(distance - previousReading) < 50) {
      sum += distance;
      validSamples++;
    }
    
    previousReading = distance;
    delay(10); // Small delay between samples
  }

  return validSamples > 0 ? sum / validSamples : previousReading; // Return average distance
}

// Function to get a single distance reading
long getDistance(int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.34 / 2.0; // Convert to mm
}
