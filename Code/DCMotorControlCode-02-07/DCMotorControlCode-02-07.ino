#include <ezButton.h>

#define DIRECTION_CCW -1
#define DIRECTION_CW 1

#define ENA_PIN 7  // The Arduino pin connected to the EN1 pin L298N
#define IN1_PIN 6  // The Arduino pin connected to the IN1 pin L298N
#define IN2_PIN 5  // The Arduino pin connected to the IN2 pin L298N

ezButton limitSwitch_1(A0);  // create ezButton object that attach to pin A0
ezButton limitSwitch_2(A1);  // create ezButton object that attach to pin A1

int direction = DIRECTION_CW;
int prev_direction = DIRECTION_CW;

void setup() {
  Serial.begin(9600);

  limitSwitch_1.setDebounceTime(50);  // set debounce time to 50 milliseconds
  limitSwitch_2.setDebounceTime(50);  // set debounce time to 50 milliseconds

  // initialize digital pins as outputs.
  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);

  digitalWrite(ENA_PIN, HIGH);  // max speed
  digitalWrite(IN1_PIN, HIGH);  // control motor A spins clockwise
  digitalWrite(IN2_PIN, LOW);   // control motor A spins clockwise
}

void loop() {
  limitSwitch_1.loop();  // MUST call the loop() function first
  limitSwitch_2.loop();  // MUST call the loop() function first

  if (limitSwitch_1.isPressed()) {
    direction *= -1;  // change direction
    Serial.println(F("The limit switch 1: TOUCHED"));
  }

  if (limitSwitch_2.isPressed()) {
    direction *= -1;  // change direction
    Serial.println(F("The limit switch 2: TOUCHED"));
  }

  if (prev_direction != direction) {
    Serial.print(F("The direction -> "));
    if (direction == DIRECTION_CW) {
      Serial.println(F("CLOCKWISE"));
      digitalWrite(IN1_PIN, HIGH);  // control motor A spins clockwise
      digitalWrite(IN2_PIN, LOW);   // control motor A spins clockwise
   
    } else {
      Serial.println(F("ANTI-CLOCKWISE"));
      digitalWrite(IN1_PIN, LOW);   // control motor A spins anti-clockwise
      digitalWrite(IN2_PIN, HIGH);  // control motor A spins anti-clockwise
  
    }

    prev_direction = direction;
  }
}