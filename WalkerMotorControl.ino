//>5V Motor Start/Stop and Speed Control

int ControlPin = 3;   //give your arduino pin a name
bool rocking = LOW;
int switchState = LOW;
int DutyCyclePercent = 58;
int timerStart = 0;

int targetKilometers = 10;
int estimatedMinutesPerKM = 12;
int bufferTimeMin = 15;
long timerEnd = 9000000;

void setup() {
  
  pinMode(ControlPin, OUTPUT); // initialize the digital pin as an output.
  pinMode(2, INPUT);

  Serial.begin(9600);          //setup the serial monitor for viewing the active PWM control value
}

void loop() { 

  switchState = digitalRead(2);

  if (switchState == HIGH) {
    timerStart = millis();
    Serial.println(timerStart);
    Serial.println(targetKilometers);
    Serial.println(estimatedMinutesPerKM);
    Serial.println(timerEnd);
    Serial.print("Duty cycle percent: ");
    Serial.println(DutyCyclePercent);
    digitalWrite(ControlPin, HIGH);
    delay(340);
    rocking = !rocking;
  }
  
  if (millis()-timerStart > timerEnd){
    rocking = LOW;
  }

  if (rocking == HIGH) {
    analogWrite(ControlPin, int(DutyCyclePercent*255/100));
  }
  
  if (rocking == LOW) {
    digitalWrite(ControlPin, LOW);
  }

}
