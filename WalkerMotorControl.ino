//>5V Motor Start/Stop and Speed Control

int ControlPin = 3;   //give your arduino pin a name
bool rocking = LOW;
int switchState = LOW;
int DutyCyclePercent = 82;
long timerStart = 0;
bool speedChange = LOW;
bool bedtimeDelay = LOW;
int timeCheck = 0;

int targetKilometers = 10;
int estimatedMinutesPerKM = 12;
int bufferTimeMin = 15;
long timerDuration = 12000000;
//red: 9m@58 = 6.4km -> 0.71m/s, 7m@57 = 4.7km -> 0.67m/s
//black: 0.7m@58 = 0.7km -> 1m/s, 2.1m@59 = 2.24km -> 1.07m/s, 4039589@60 = 4.1k -> 1.01m/s, 4.9m@60 = 5.17km -> 1.055m/s
// 690sec@66 = 700m -> 1.014m/s, 3.9m@66 = 4.73km -> 1.21m/s, 1.7m@66 = 1.23km, 12m@65 = 8.7km
// 10.3k from 8m ms, 6.8 from 5.5m - looking like 1.2 is reliable on black phone

void setup() {
  
  pinMode(ControlPin, OUTPUT); // initialize the digital pin as an output.
  pinMode(2, INPUT);

  Serial.begin(115200);          //setup the serial monitor for viewing the active PWM control value
}

void readouts() {
    Serial.print("Timer Checkpoint: ");
    Serial.println(timerStart);

    // Serial.println(targetKilometers);
    // Serial.println(estimatedMinutesPerKM);

    if (rocking) {
      Serial.print("Rocking started. Duty cycle: ");
      Serial.print(DutyCyclePercent);
      Serial.println("%");

      Serial.print("Timer Delta Setting: ");
      Serial.print(timerDuration/1000);
      Serial.println(" seconds.\n");
    }
    if (!rocking) {
      Serial.println("Rocking stopped.\n");
    }

}

void DelayStart() {
    Serial.println("Two hour delay started.");
    delay(10000000);
    Serial.println("Two hour delay ended.");
}

void loop() { 

  switchState = digitalRead(2);

  if (switchState == HIGH) {
    if (bedtimeDelay){
      DelayStart();
    }
    else{
      Serial.println("Start delay off.");
    }
    timerStart = millis();
    timeCheck = 0;
    Serial.println("Run timer of " + String(timerDuration/1000) + " seconds started.");
    digitalWrite(ControlPin, HIGH);
    delay(340);
    rocking = !rocking;
    speedChange = HIGH;
    //readouts();
  }
  
  if (millis()-timerStart > timerDuration && speedChange == LOW){
    Serial.println("Run timer ended.");
    rocking = LOW;
    speedChange = HIGH;
  }

  if (rocking == HIGH && speedChange == HIGH) {
    analogWrite(ControlPin, int(DutyCyclePercent*255/100));
    speedChange = LOW;
  }
  
  if (rocking == LOW && speedChange == HIGH) {
    digitalWrite(ControlPin, LOW);
    speedChange = LOW;
  }

  if (round((millis()-timerStart)/1000) % 2 == 0)
  {
    timeCheck++;
    Serial.println("Timer (minutes): " + String(timeCheck));
  }
  

}


  // seconds = millis()/1000;

  // if (seconds % 2 == 0){
  //   digitalWrite(13, HIGH);
  // }
  // else{
  //   digitalWrite(13, LOW);
  // }