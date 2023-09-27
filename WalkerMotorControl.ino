//>5V Motor Start/Stop and Speed Control

int ControlPin = 3;   //give your arduino pin a name
bool rocking = LOW;
int switchState = LOW;
int DutyCyclePercent = 82;
long timerStart = 0;
bool speedChange = LOW;
float bedtimeDelayHrs = 0;
long timerElapsed = 0;
bool prevSwitchState = LOW;

float targetKilometers = 4.9;
float estimatedMpS = 1.1;
long timerDuration = targetKilometers * 1000000 / estimatedMpS;
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


    if (rocking) {
      Serial.println("Rocking started.");
      Serial.println("Duty cycle: " + String(DutyCyclePercent) + "%");
      // Serial.println("Run timer: " + String(timerDuration/1000) + " seconds.");
      Serial.println("Timer will run for " + String(timerDuration/60000) + " minutes. \n");
    }
    if (!rocking) {
      Serial.println("Rocking stopped after " + String(timerElapsed/1000) + " seconds. \n");
    }

}

void DelayStart() {
    long del = bedtimeDelayHrs*60*60*1000;
    Serial.println(String(bedtimeDelayHrs) + " hour delay started.");
    delay(del);
    Serial.println(String(bedtimeDelayHrs) + " hour delay ended.");
}

void Engage() {
   if (bedtimeDelayHrs > 0){
      DelayStart();
    }
    else{
      Serial.println("Start delay off.");
    }
    timerStart = millis();
    digitalWrite(ControlPin, HIGH);
    delay(340);
    rocking = !rocking;
    speedChange = HIGH;
    readouts();
}

void loop() { 

  switchState = digitalRead(2);

  if (switchState != prevSwitchState) {
    if (switchState == HIGH) {
        if (rocking == LOW && speedChange == LOW) {
          Engage();
        }
        else {
          rocking = LOW;
          speedChange = HIGH;
        }
      }
  }
  prevSwitchState = switchState;
  
  if (millis()-timerStart > timerDuration && rocking == HIGH){
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
    readouts();
  }

  timerElapsed = millis()-timerStart;
  if (timerElapsed % 100000 == 0 && rocking){
    Serial.println(String(timerElapsed/100000) + " hundred seconds. \nRoughly " + String(timerElapsed/1000*estimatedMpS/1000) + " kilometers. \n");
    delay(50);
  }


}

// constant : watch for button presses



  // seconds = millis()/1000;

  // if (seconds % 2 == 0){
  //   digitalWrite(13, HIGH);
  // }
  // else{
  //   digitalWrite(13, LOW);
  // }