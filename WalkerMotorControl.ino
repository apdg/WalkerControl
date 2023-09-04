//>5V Motor Start/Stop and Speed Control

int ControlPin = 3;   //give your arduino pin a name
bool rocking = LOW;
int switchState = LOW;
int DutyCyclePercent = 60;
long timerStart = 0;

int targetKilometers = 10;
int estimatedMinutesPerKM = 12;
int bufferTimeMin = 15;
long timerDuration = 4600000;
//red: 9m@58 = 6.4km -> 0.71m/s, 7m@57 = 4.7km -> 0.67m/s
//black: 0.7m@58 = 0.7km -> 1m/s, 2.1m@59 = 2.24km -> 1.07m/s, 4.6m@60 = 

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
    Serial.println(timerDuration);
    Serial.print("Duty cycle percent: ");
    Serial.println(DutyCyclePercent);
    digitalWrite(ControlPin, HIGH);
    delay(340);
    rocking = !rocking;
  }
  
  if (millis()-timerStart > timerDuration){
    rocking = LOW;
  }

  if (rocking == HIGH) {
    analogWrite(ControlPin, int(DutyCyclePercent*255/100));
  }
  
  if (rocking == LOW) {
    digitalWrite(ControlPin, LOW);
  }

}
