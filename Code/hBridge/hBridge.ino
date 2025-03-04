int PWMPin = 3; int DirPin = 2;
int power = 10;
int val;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PWMPin, OUTPUT);
  pinMode(DirPin, OUTPUT);
  
  digitalWrite(DirPin, HIGH);
  digitalWrite(PWMPin, power);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()){
    val = Serial.read();

    if(val == '=') {
      power = power - 5;
      digitalWrite(PWMPin, power);
    }

    if(val == '-') {
      power = power + 5;
      digitalWrite(PWMPin, power);
    }

  }
}
