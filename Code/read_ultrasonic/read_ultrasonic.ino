// Define the pins for the ultrasonic sensor
const int trigPin = 9; // Trig pin of the ultrasonic sensor
const int echoPin = 10; // Echo pin of the ultrasonic sensor

//Define servo motor connections:
#include<Servo.h> //include server library
Servo ser; //create servo object to control a servo
int openVal = 110; //Open gripper value
int closeVal = 10; //Close gripper value

// Variables for the duration of the pulse and the distance
long duration;
long distance_mm;


void setup() {
  // put your setup code here, to run once:
  // Initialize serial communication
  Serial.begin(9600);

  // Define pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Sets the servo pin and opens gripper:
  ser.attach(5);// server is connected at pin 5
  ser.write(closeVal);// the servo will move according to position 
  delay(2000);//delay for the servo to get to the position

}

void loop() {
  // put your main code here, to run repeatedly:
// Clear the trigPin by setting it LOW for 2 microseconds
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Set the trigPin HIGH for 10 microseconds to trigger the sensor
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the duration of the echo pulse from the echoPin
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance in centimeters
  distance_mm = duration * 0.34 / 2.0;

  //open gripper
  if(distance_mm<=35)
  {
    ser.write(openVal);// the servo will move according to position 
    delay(3000);//delay for the servo to get to the position
    ser.write(closeVal);// the servo will move according to position
  }

  
  // Print the distance to the serial monitor
  Serial.print("Distance: ");
  Serial.println(distance_mm);
  Serial.println(" mm");
  //Serial.print(duration);

  // Wait before taking the next measurement
  delay(500); // Adjust delay as needed for your application
}
