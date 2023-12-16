
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int MotorSignalPin = 6;
int ButtonPinRight = 7;
int ButtonPinLeft = 8;
bool rocking = LOW;
int switchStateL = LOW;
int switchStateR = LOW;
bool prevSwitchStateL = LOW;
bool prevSwitchStateR = LOW;
int DutyCyclePercent = 65;
long timerStart = 0;
bool speedChange = LOW;
float bedtimeDelayHrs = 0;
long timerElapsed = 0;

float targetKilometers = 69;
float estimatedMpS = 1.19;
long timerDuration = targetKilometers * 1000000 / estimatedMpS;

void setup() {
  
  // initialize pins as output.
  pinMode(MotorSignalPin, OUTPUT);
  pinMode(ButtonPinRight, INPUT);
  pinMode(ButtonPinLeft, INPUT);

  lcd.begin(16, 2);
  lcd.print("TARGET:" + String(int(targetKilometers)) + "k @" + String(estimatedMpS));

  Serial.begin(38400);
}

void readouts() {
    Serial.print("Timer Checkpoint: ");
    Serial.println(timerStart);


    if (rocking) {
      Serial.println("Rocking started.");
      Serial.println("Duty cycle: " + String(DutyCyclePercent) + "%");
      Serial.println("Timer will run for " + String(timerDuration/60000) + " minutes. \n");
      lcd.setCursor(0, 1);
      lcd.print("                ");
    }
    if (!rocking) {
      Serial.println("Rocking stopped after " + String(timerElapsed/1000) + " seconds. \n");
      lcd.setCursor(0, 1);
      lcd.print(String(((millis()-timerStart) / 1000)*estimatedMpS/1000) + "km");
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
    digitalWrite(MotorSignalPin, 140);
    delay(200);
    rocking = !rocking;
    speedChange = HIGH;
    readouts();
}

void doLeftButton() {
        if (rocking == LOW && speedChange == LOW) {
          Engage();
        }
        else {
          rocking = LOW;
          speedChange = HIGH;
        }
}

void doRightButton() {
      String lcdString = "0.5";

      if (targetKilometers < 5)
      {
        targetKilometers = targetKilometers + 0.5;
        lcdString = String(targetKilometers, 1);
      }
      else if (targetKilometers == 5)
      {
        targetKilometers = 7;
        lcdString = "7";
      }
      else if (targetKilometers == 7)
      {
        targetKilometers = 10;
        lcdString = "10";
      }
      else if (targetKilometers == 10)
      {
        targetKilometers = 12;
        lcdString = "12";
      }
      else{
        targetKilometers = 0.5;
      }
      
      
      lcd.setCursor(0, 0);
      lcd.print("TARG:" + lcdString + "k @" + String(estimatedMpS) + "   ");
      timerDuration = targetKilometers * 1000000 / estimatedMpS;

}

void loop() { 
  if (rocking) {
    lcd.setCursor(0, 1);
    lcd.print(String(((millis()-timerStart) / 1000)*estimatedMpS/1000) + "km");
  }

  switchStateL = digitalRead(ButtonPinLeft);
  switchStateR = digitalRead(ButtonPinRight);

  // Monitor Left Button
  if (switchStateL != prevSwitchStateL) {
    Serial.println("switch state LEFT changed");
    if (switchStateL == HIGH) {
      doLeftButton();
      }
  }
  prevSwitchStateL = switchStateL;
  
  // Monitor Right Button
  if (switchStateR != prevSwitchStateR) {
    Serial.println("switch state RIGHT changed");
    if (switchStateR == HIGH) {
      doRightButton();
      }
  }
  prevSwitchStateR = switchStateR;
 

  if (millis()-timerStart > timerDuration && rocking == HIGH){
    Serial.println("Run timer ended.");
    rocking = LOW;
    speedChange = HIGH;
  }

  if (rocking == HIGH && speedChange == HIGH) {
    analogWrite(MotorSignalPin, int(DutyCyclePercent*255/100));
    speedChange = LOW;
  }
  
  if (rocking == LOW && speedChange == HIGH) {
    digitalWrite(MotorSignalPin, LOW);
    speedChange = LOW;
    readouts();
  }

  timerElapsed = millis()-timerStart;
  if (timerElapsed % 100000 == 0 && rocking){
    Serial.println(String(timerElapsed/100000) + " hundred seconds. \nRoughly " + String(timerElapsed/1000*estimatedMpS/1000) + " kilometers. \n");
    delay(50);
  }


}

