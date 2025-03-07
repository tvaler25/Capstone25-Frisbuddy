const int buzzer = 9; //buzzer to arduino pin 9

void setup(){
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
  tone(buzzer, 750); // Send 1KHz sound signal...
  delay(3000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
}

void loop(){

}