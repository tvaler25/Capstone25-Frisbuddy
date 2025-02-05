//Define servo motor connections:
#include<Servo.h> //include server library

// Define the pins for the ultrasonic sensor
const int trigPinRight = 9; // Trig pin of the ultrasonic sensor
const int echoPinRight = 10; // Echo pin of the ultrasonic sensor
const int trigPinLeft  = 7;
const int echoPinLeft  = 8;

// Create Servo Objects
Servo ser1; 
Servo ser2;

const int servo_left_pin = 5;
const int servo_right_pin = 11; 

int upValRight = 120; //Right Paddle Up position
int downValRight = 75; //right paddle down position
int upValLeft = 40;  //left paddle up position
int downValLeft = 170; //left paddle down position


// Variables for the duration of the pulse and the distance
long duration_right;
long distance_mm_right;

long duration_left;
long distance_mm_left; 


void setup() {

  Serial.begin(9600);

  // Define pin modes
  pinMode(trigPinRight, OUTPUT);
  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinRight, INPUT);
  pinMode(echoPinLeft, INPUT);

  ser1.attach(servo_left_pin);// servo is connected at pin 5

  ser1.write(downValLeft);// the servo will move according to position 
  
  ser2.attach(servo_right_pin);
  ser2.write(downValRight);

  delay(2000); //delay for the servo to get to the position

}

void loop() {
  // put your main code here, to run repeatedly:
// Clear the trigPin by setting it LOW for 2 microseconds
  digitalWrite(trigPinRight, LOW);
  delayMicroseconds(2);

  // Set the trigPin HIGH for 10 microseconds to trigger the sensor
  digitalWrite(trigPinRight, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinRight, LOW);

  // Read the duration of the echo pulse from the echoPin
  duration_right = pulseIn(echoPinRight, HIGH);

  // Calculate the distance in centimeters
  distance_mm_right = duration_right * 0.34 / 2.0;

  //open gripper
  if(distance_mm_right<=35)
  {


    ser1.write(upValLeft);// the servo will move according to position 
    delay(1000);//delay for the servo to get to the position
    ser2.write(upValRight);
    delay(3000);
    ser1.write(downValLeft);// the servo will move according to position
    delay(1000);
    ser2.write(downValRight);

  }

  
  // Print the distance to the serial monitor
  Serial.print("Distance: ");
  Serial.println(distance_mm_right);
  Serial.println(" mm");
  //Serial.print(duration);

  // Wait before taking the next measurement
  delay(500); // Adjust delay as needed for your application
}
