int PWMPin = 3; int DirPin = 2;
int power = 10;
int val;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PWMPin, OUTPUT);
  pinMode(DirPin, OUTPUT);
  
  digitalWrite(DirPin, HIGH);
  analogWrite(PWMPin, power);
  Serial.print(power);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()){
    val = Serial.read();

    if(val == '-') {
      power = power - 5;
      if(power<0) {
        power=0; 
      }
      Serial.println(power);
      analogWrite(PWMPin, power);
    }

    if(val == '+') {
      power = power + 5;
      if(power>255) {
        power=255; 
      }
      Serial.println(power);
      analogWrite(PWMPin, power);
    }


  }
}
