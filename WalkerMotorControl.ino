//>5V Motor Start/Stop and Speed Control

int ControlPin = 3;   //give your arduino pin a name
bool rocking = LOW;
int switchState = LOW;
int DutyCyclePercent = 56;

void setup() {
  
  pinMode(ControlPin, OUTPUT); // initialize the digital pin as an output.
  pinMode(2, INPUT);

  Serial.begin(9600);          //setup the serial monitor for viewing the active PWM control value
}

void loop() { 
  
  switchState = digitalRead(2);

  if (switchState == HIGH) {
    Serial.print("Duty cycle percent: ");
    Serial.println(DutyCyclePercent);
    digitalWrite(ControlPin, HIGH);
    delay(340);
    analogWrite(ControlPin, int(DutyCyclePercent*255/100));
    rocking = !rocking;
  }
  
  if (rocking == LOW) {
    digitalWrite(ControlPin, LOW);
  }

// this seems good
  // Serial.println(int(DutyCyclePercent*255/100));

}
