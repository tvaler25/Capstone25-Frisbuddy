//Define servo motor connections:
#include<Servo.h> //include server library

// Define the pins for the ultrasonic sensor
const int trigPin = 4; // Trig pin of the ultrasonic sensor
const int echoPinRight = 5; // Echo pin of the ultrasonic sensor right
const int echoPinLeft  = 6;  // Echo pin of the left ultraonic 

// Create Servo Objects
Servo left_servo; 
Servo right_servo;

const int servo_left_pin = 7;
const int servo_right_pin = 8; 

char test_val = '\0';  // Initialize with a default value

int ClearValRight = 0; //Right Paddle Up position
int downValRight = 73; //right paddle down position
int PushValLeft = 30;  //left paddle up position
int downValLeft = 60; //left paddle down position
int ClearValLeft = 180;
int PushValRight = 160;


// Variables for the duration of the pulse and the distance
long duration_right;
long distance_mm_right;

long duration_left;
long distance_mm_left; 


void setup() {

  Serial.begin(9600);

  // Define pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPinRight, INPUT);
  pinMode(echoPinLeft, INPUT);

  left_servo.attach(servo_left_pin);// servo is connected at pin 5

  left_servo.write(downValLeft);// the servo will move according to position 
  
  right_servo.attach(servo_right_pin);
  right_servo.write(downValRight);

  delay(2000); //delay for the servo to get to the position

}

void loop() {

  
  if (Serial.available()){
  test_val = Serial.read();
  }
  // put your main code here, to run repeatedly:
// Clear the trigPin by setting it LOW for 2 microseconds
digitalWrite(trigPin, LOW);
delayMicroseconds(2);


// Set the trigPin HIGH for 10 microseconds to trigger the sensor
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Read the duration of the echo pulse from the echoPin
duration_right = pulseIn(echoPinRight, HIGH);
duration_left = pulseIn(echoPinLeft, HIGH);

// Calculate the distance in centimeters
distance_mm_right = duration_right * 0.34 / 2.0;
distance_mm_left  = duration_left  * 0.34 / 2.0;


  //Left Sesnor Activated
  
  if(test_val == 'l' || distance_mm_left <30)
  {
    right_servo.write(103);
    left_servo.write(90);
    delay(3000);
    right_servo.write(ClearValRight);
    delay(3000);
    left_servo.write(downValLeft);// the servo will move according to position
    delay(1000);
    right_servo.write(downValRight);
    
    //right_servo.write(ClearValRight);
    //delay(1000);//delay for the servo to get to the position
    //left_servo.write(PushValLeft);// the servo will move according to position 
    //delay(3000);
    //left_servo.write(downValLeft);// the servo will move according to position
    //delay(1000);
    //right_servo.write(downValRight);

  }

  else if(test_val == 'r' || distance_mm_right<33)
  {
    delay(3000);
    left_servo.write(ClearValLeft);// the servo will move according to position 
    delay(1000);//delay for the servo to get to the position
    right_servo.write(PushValRight);
    delay(3000);
    right_servo.write(downValRight);
    delay(1000);
    left_servo.write(downValLeft);// the servo will move according to position
 
    

  }

  
  // Print the distance to the serial monitor
  //Serial.print("Distance: ");
  //Serial.println(distance_mm_right);
  //Serial.println(" mm");
  //Serial.print(duration);

  // Wait before taking the next measurement
  delay(500); // Adjust delay as needed for your application
}
