#include "RobotMap.h"
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Encoder.h>
#include "Chassis.h"
#include "Arm.h"
#include "Linesensor.h"
#include "Ultrasonic.h"
#include "Navigation.h"
#include "FlameSense.h"

unsigned long timeForPushSubsysFreq = 0;
unsigned long timeForPushGyroFreq   = 0;
double sensorHeight= 8.5;
int robotHeading = 0;
signed short last, curr, speedMode;
bool closeFlame = false;
bool flameSensed = false;
Chassis chassis;
Ultrasonic ultrasonic;
Navigation nav;
FlameSense flame;
Encoder encLeft(encLeft1, encLeft2);
Encoder encRight(encRight1, encRight2);

LiquidCrystal lcd(40,41,42,43,44,45);

// Arm arm;

void setup() {

  Serial.begin(9600);

  Serial.println("Starting");
  timeForPushSubsysFreq = millis() + 100;
  chassis.attach(mtrLF, mtrLR);
  ultrasonic.init();

  curr = ultrasonic.get();
  last = curr;

  flame.init();

  lcd.begin(16,1);

  lcd.clear();

  // if (nav.init()) {
  //     lcd.print("GYRO SUCCESS");
  // } else {
  //     lcd.print("GYRO FAILED");
  // }

  pinMode(fan, OUTPUT);

  delay(1000);

}

void auton () {

    // curr = ultrasonic.get();
    // switch(curr) {
    //     case drive :
    //         lcd.clear();
    //         lcd.print("drive     ");
    //         speedMode = 30;
    //     break;
    //     case closeWall:
    //         lcd.clear();
    //         lcd.print("close    ");
    //         if(last!=closeWall) {
    //             robotHeading += 3;
    //             speedMode = 30;
    //         }
    //     break;
    //     case wall :
    //         lcd.clear();
    //         lcd.print("wall     ");
    //         if(last != wall) {
    //             robotHeading -= 100;
    //         }
    //         speedMode = 0;
    //     break;
    //     case edge :
    //         lcd.clear();
    //         lcd.print("edge     ");
    //         if(last != edge) robotHeading += 200;
    //         speedMode = 20;
    //     break;
    //     case halfDrive :
    //         lcd.clear();
    //         lcd.print("half     ");
    //         speedMode = 20;
    //     break;
    //     default : chassis.stop();
    //     break;
    // }
    // last = curr;
    if(flame.get(false)) {
        digitalWrite(fan, HIGH);
        speedMode = 10;
        robotHeading = flame.getTurn();

    }

    // digitalWrite(fan, HIGH);

    chassis.drive(speedMode, (robotHeading + nav.getDir()));

    // chassis.drive(0,0);
}

void updateSubsys () {
    chassis.update();
    ultrasonic.update();
    nav.updateEnc(encLeft.read(), encRight.read());
    // arm.update();
}



///////////////////////////////////////////////
// Stuff that shouldn't be touched too hard. //
// Just saying.                              //
///////////////////////////////////////////////

void loop() {
    auton();  //calls the auton method
    // if () {
    //
    // }
    if (millis() > timeForPushSubsysFreq) {
        timeForPushSubsysFreq = millis() + 100;
        updateSubsys();
    }
    if (millis() > timeForPushGyroFreq) {
        if (!flame.update()) {
            timeForPushGyroFreq = millis() + 5;
        }
    }

    // nav.updateGyro(); //nav.updateGyro()
}
