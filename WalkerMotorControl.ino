//>5V Motor Start/Stop and Speed Control

int ControlPin = 3;   //give your arduino pin a name
bool rocking = LOW;
int switchState = LOW;
int DutyCyclePercent = 58;
int timerCount = 0;
long timerEnd = 1500000;
bool once = LOW;

void setup() {
  
  pinMode(ControlPin, OUTPUT); // initialize the digital pin as an output.
  pinMode(2, INPUT);

  Serial.begin(9600);          //setup the serial monitor for viewing the active PWM control value
}

void loop() { 
  timerCount++;
  if (timerCount < timerEnd){
      Serial.println(timerCount);
  }

  switchState = digitalRead(2);

  if (switchState == HIGH) {
    timerCount = 0;
    Serial.print("Duty cycle percent: ");
    Serial.println(DutyCyclePercent);
    digitalWrite(ControlPin, HIGH);
    delay(340);
    rocking = !rocking;
  }
  
  if (timerCount > timerEnd){
    rocking = LOW;
    if (once == false){
      once = true;
      Serial.print(millis());
    }
  }

  if (rocking == HIGH) {
    analogWrite(ControlPin, int(DutyCyclePercent*255/100));
  }
  
  if (rocking == LOW) {
    digitalWrite(ControlPin, LOW);
  }

}
