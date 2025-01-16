// Define the pins for the ultrasonic sensor
const int trigPin = 9; // Trig pin of the ultrasonic sensor
const int echoPin = 10; // Echo pin of the ultrasonic sensor

//Define servo motor connections:
#include<Servo.h> //include server library
<<<<<<< Updated upstream
<<<<<<< Updated upstream
Servo ser; //create servo object to control a servo
int openVal = 110; //Open gripper value
int closeVal = 10; //Close gripper value
=======
=======
>>>>>>> Stashed changes
Servo ser1; //create servo object to control a servo
Servo ser2;
int openVal = 120; //Open gripper value
int closeVal = 75; //Close gripper value
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes

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
<<<<<<< Updated upstream
<<<<<<< Updated upstream
  ser.attach(5);// server is connected at pin 5
  ser.write(closeVal);// the servo will move according to position 
=======
=======
>>>>>>> Stashed changes
  ser1.attach(5);// server is connected at pin 5
  

  ser1.write(closeVal);// the servo will move according to position 

  ser2.attach(11);
  ser2.write(closeVal);
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
<<<<<<< Updated upstream
    ser.write(openVal);// the servo will move according to position 
    delay(3000);//delay for the servo to get to the position
    ser.write(closeVal);// the servo will move according to position
=======
=======
>>>>>>> Stashed changes
    ser1.write(openVal);// the servo will move according to position 
    delay(1000);//delay for the servo to get to the position
    ser2.write(openVal);
    delay(3000);
    ser1.write(closeVal);// the servo will move according to position
    delay(1000);
    ser2.write(closeVal);
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
  }

  
  // Print the distance to the serial monitor
  Serial.print("Distance: ");
  Serial.println(distance_mm);
  Serial.println(" mm");
  //Serial.print(duration);

  // Wait before taking the next measurement
  delay(500); // Adjust delay as needed for your application
}
