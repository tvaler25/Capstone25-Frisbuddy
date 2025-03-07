//Set digital pin numbers
int motorDirPin = 2; int motorPWMPin = 3;
int reorienterTriggerPin = 4; int reorienterEchoLeftPin = 5; int reorienterEchoRightPin = 6;
int pusherServoPin = 7;
int reserveGateServoPin = 8; int reserveLifterServoPin = 9;
int aimerIn1Pin = 10; int aimerIn2Pin = 11;
int IRReceiverSignal = 12;

//set analog pin numbers
int aimerLeftSignal = A0; int aimerRightSignal = A1;
int buzzerPin = A2;

//set constants for throughout program
int motorPower = 165;

void setup() {
  //Set pin modes
  pinMode(motorDirPin, OUTPUT); pinMode(motorPWMPin, OUTPUT);

  //Write default values to pins
  analogWrite(motorDirPin, HIGH); digitalWrite(motorPWMPin, motorPower);


  Serial.begin(9600);
}

void loop() {
  
  //wait for signal from either reorienter or remote
  do {}
  while(not seeting anything from reorienter and not seeing anything from remote control);


  delay(3000); //wait for disc to reorient

  //push disc into wheel


}
