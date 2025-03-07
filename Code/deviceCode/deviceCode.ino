//Set digital pin numbers
int motorDirPin = 2; int motorPWMPin = 3;
int reorienterTriggerPin = 4; int reorienterEchoLeftPin = 5; int reorienterEchoRightPin = 6;
int reorienterServoRightPin = 7; int reorienterServoLeftPin = 8;
int pusherServoPin = 9;
int reserveGateServoPin = 10; int reserveLifterServoPin = 11;
int aimerIn1Pin = 12; int aimerIn2Pin = 13;


//set analog pin numbers
int aimerLeftSignalPin = A0; int aimerRightSignalPin = A1;
int buzzerPin = A2;
int IRReceiverPin = A3;

//set constants for throughout program
int motorPower = 165;

void setup() {
  //Set pin modes
  pinMode(motorDirPin, OUTPUT); pinMode(motorPWMPin, OUTPUT);
  pinMode(reorienterTriggerPin, OUTPUT); pinMode(reorienterEchoLeftPin, INPUT); pinMode(reorienterEchoRightPin, INPUT); 
  pinMode(reorienterServoRightPin, OUTPUT); pinMode(reorienterServoRightPin, OUTPUT);
  pinMode(pusherServoPin, OUTPUT);
  pinMode(reserveGateServoPin, OUTPUT); pinMode(reserveLifterServoPin, OUTPUT);
  pinMode(aimerIn1Pin, OUTPUT); pinMode(aimerIn2Pin, OUTPUT);

  pinMode(aimerLeftSignalPin, INPUT); pinMode(aimerRightSignalPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(IRReceiverPin, INPUT);

  //Write default values to pins
  analogWrite(motorDirPin, HIGH); digitalWrite(motorPWMPin, motorPower);


  Serial.begin(9600);
}

void loop() {
  
  //wait for signal from either reorienter or remote
  do {}
  while(not seeting anything from reorienter and not seeing anything from remote control);

  if(reorienter was seen) {
    reorient
  }

  else if(remote control saw something) {
    eject disc from reserve stack
  }

  delay(3000); //wait for disc to reorient

  
  tone(buzzer, 750); //turn on buzzer

  //push disc into wheel
  

  delay(2000); //wait for disc to pass the aimer
  noTone(buzzer); //turn off buzzer


  //re-aim


}
