//still need to do:
//implement IR remote into code
//add aimer code
//make sure reorienter code is up to date (delays too long?)
//make sure all analog pins are working properly (is int A0/1/2/3 illegal?)


#include <Servo.h>

//---------------------------------------PIN NUMBERS & SERVO OBJECTS----------------------------------

//Set digital pin numbers
int motorDirPin = 2; int motorPWMPin = 3;
int reorienterTriggerPin = 4; int reorienterEchoRightPin = 5; int reorienterEchoLeftPin = 6;
int reorienterServoRightPin = 7; int reorienterServoLeftPin = 8;
int pusherServoPin = 9;
int reserveGateServoPin = 10; int reserveLifterServoPin = 11;
int aimerIn1Pin = 12; int aimerIn2Pin = 13;

//set analog pin numbers
int aimerLeftSignalPin = A0; int aimerRightSignalPin = A1;
int buzzerPin = A2;
int IRReceiverPin = A3;

//set servo names
Servo servoLeft; Servo servoRight;
Servo servoPusher;
Servo servoGate; Servo servoLifter;

//------------------------------------------------CONSTANTS---------------------------------------

//Motor/H-Bridge constants:
int motorPower = 165;

//Reorienter constants:
const int ClearValRight = 0; //servo positions
const int downValRight = 73;
const int PushValLeft = 30;  
const int downValLeft = 60;
const int ClearValLeft = 180;
const int PushValRight = 160;

long duration_right, duration_left; //initialize sensor variables
long distance_mm_right, distance_mm_left;

long filteredDistanceRight = 0; //filter settings
long filteredDistanceLeft = 0;
const int numSamples = 5; // Number of readings to average

int rightDistanceBound = 33; int leftDistanceBound = 33;

// Pusher constants: 
int pusherBack = 30; int pusherForward = 150;

//Reserve stack constants:
int lifterDown = 10; int lifterUp = 70;
int gateDown = 70; int gateUp = 10;

//Aimer constants:


//IR Receiver/buzzer constants:

//--------------------------------------------SETUP-------------------------------------------------

void setup() {

  Serial.begin(9600);

  //Set pin modes or attach servos
  pinMode(motorDirPin, OUTPUT); pinMode(motorPWMPin, OUTPUT); //motor
  pinMode(reorienterTriggerPin, OUTPUT); pinMode(reorienterEchoLeftPin, INPUT); pinMode(reorienterEchoRightPin, INPUT); //reserve stack 
  servoRight.attach(reorienterServoRightPin); servoLeft.attach(reorienterServoLeftPin);
  servoPusher.attach(pusherServoPin); //pusher
  servoGate.attach(reserveGateServoPin); servoLifter.attach(reserveLifterServoPin); //reserve stack
  pinMode(aimerIn1Pin, OUTPUT); pinMode(aimerIn2Pin, OUTPUT); //aimer

  pinMode(aimerLeftSignalPin, INPUT); pinMode(aimerRightSignalPin, INPUT);
  pinMode(buzzerPin, OUTPUT); //buzzer
  pinMode(IRReceiverPin, INPUT); //IR signal

  //Write default values to pins
  analogWrite(motorDirPin, HIGH); digitalWrite(motorPWMPin, motorPower);

  servoGate.write(gateDown); servoLifter.write(lifterDown); //reserve stack
  
}

//--------------------------------------------MAIN LOOP--------------------------------------------------------

void loop() {
  
  //wait for signal from either reorienter or remote
  do {

    // Get filtered distance readings
    filteredDistanceRight = getFilteredDistance(reorienterEchoRightPin);
    filteredDistanceLeft = getFilteredDistance(reorienterEchoLeftPin);

    //Serial.print("Right: ");
    //Serial.print(filteredDistanceRight);
    //Serial.print(" mm | Left: ");
    //Serial.println(filteredDistanceLeft);


    delay(100); //check every 100 ms

  }
  while(filteredDistanceRight < rightDistanceBound && filteredDistanceLeft < leftDistanceBound);

  //reorient right
  if(filteredDistanceRight >= rightDistanceBound) {
    servoRight.write(103);
    servoLeft.write(90);
    delay(3000);
    servoRight.write(ClearValRight);
    delay(3000);
    servoLeft.write(downValLeft);
    delay(1000);
    servoRight.write(downValRight);
  }

  //reorient left
  else if(filteredDistanceLeft >= leftDistanceBound) {
    delay(3000);
    servoLeft.write(ClearValLeft);
    delay(1000);
    servoRight.write(PushValRight);
    delay(3000);
    servoRight.write(downValRight);
    delay(1000);
    servoLeft.write(downValLeft);
  }

  //pull from reserve stack
  else if(remote control saw something) {
    servoLifter.write(lifterUp); //lift discs up
    delay(1000);
    servoGate.write(gateUp); //open gate
    delay(1500);
    servoGate.write(gateDown); //close gate
    delay(1000);
    servoLifter.write(lifterDown);//lower linkage
  }

  //push disc into wheel, give warning with buzzer
  tone(buzzerPin, 750); 
  for (int pos = pusherBack; pos <= pusherForward; pos += 1) { // Increase angle in small steps
    servoPusher.write(pos);
    delay(20);  // Small delay for smoother motion
  } 
  delay(1000);
  for (int pos = pusherForward; pos >= pusherBack; pos -= 1) { 
    servoPusher.write(pos);
    delay(20);
  }
  noTone(buzzerPin); // Decrease angle in small steps

  //aim
  delay(2000); //wait for disc to pass the aimer
  int aimerAngle = (random(3) - 1) * 30; //random between -30, 0, 30

}

//----------------------------------------ADDITIONAL FUNCTIONS------------------------------------------

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
  digitalWrite(reorienterTriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(reorienterTriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(reorienterTriggerPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.34 / 2.0; // Convert to mm
}
