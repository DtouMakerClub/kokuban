#include "motorController.hpp"
#include <TimerOne.h>

char command = 0;

const unsigned long STEPPING_MOTOR_PERIOD_HALF_US = 300;//100;//周期はこれの２倍

MotorController motorController = MotorController();

void setup() {
  Serial.begin(9600);
  Serial.print("This is XYstage.ino\n");

  motorController.pinSetup();
  pinMode(LED_BUILTIN,OUTPUT);

  digitalWrite(LED_BUILTIN,LOW);
  //digitalWrite(LED_BUILTIN,HIGH);
  //パルス出力用のtimer
  Timer1.initialize(motorController.getTimerPeriodForX());
  Timer1.attachInterrupt(handler1);//&MotorController::toggleXPulseAndUpdatePosition);

  //Timer2.initialize(motorController.getTimerPeriodForY());//こうしたかった
  //Timer2.attachInterrupt(handler2);
}

void loop() {
  command = Serial.read();

  if(command == 'c'){
    Serial.print("start calibration\n");

    motorController.calibration();

    Serial.print("X range : ");
    Serial.print(motorController.getXRange(),DEC);
    Serial.print("\nY range : ");
    Serial.print(motorController.getYRange(),DEC);
  }

  if(motorController.isCalibFinished()){
    motorController.setXSpeedToTarget();
  }
}

void handler1(){
  motorController.toggleXPulseAndUpdatePosition();
  motorController.toggleYPulseAndUpdatePosition();
}
void handler2(){
  motorController.toggleYPulseAndUpdatePosition();
}


