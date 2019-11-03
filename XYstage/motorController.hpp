#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <TimerOne.h>

class MotorController
{
private:
    //pin number
    const int PIN_MOTOR_XY_ENABLE = 8;
    const int PIN_MOTOR_X_STEP = 2;
    const int PIN_MOTOR_X_DIR = 5;
    const int PIN_MOTOR_Y_STEP = 3;
    const int PIN_MOTOR_Y_DIR = 6;
    const int PIN_SW_X0 = 9;
    const int PIN_SW_X1 = 7;
    const int PIN_SW_Y0 = 4;
    const int PIN_SW_Y1 = 12;

    //motor
    const int MOTOR_ENABLE = LOW;
    const int MOTOR_DISABLE = !MOTOR_ENABLE;

    //motor rotation direction
    const bool X_PLUS = HIGH;
    const bool X_MINUS = !X_PLUS;
    const bool Y_PLUS = HIGH;
    const bool Y_MINUS = !Y_PLUS;

    //motor pulse
    bool pulseStateX = 0;
    bool pulseStateY = 0;
    long positionX = 0;
    long positionY = 0;
    long targetX = 0;
    long targetY = 0;

    //キャリブレーション用
    //bool calibXFinishedFlag = false;
    //bool calibYFinishedFlag = false;
    long rangeX = 0;
    long rangeY = 0;
    const int HALF_PERIOD_OF_PULSE_MICROSEC = 600 / 2;
    void moveXOneStepPlusForCalib();
    void moveXOneStepMinusForCalib();
    void moveYOneStepPlusForCalib();
    void moveYOneStepMinusForCalib();


    bool isX0Limit();
    bool isX1Limit();
    bool isY0Limit();
    bool isY1Limit();
    bool isLimit();

    //const unsigned long STEPPING_MOTOR_PERIOD_HALF_US = 300; //100;//周期はこれの２倍
public:
    void pinSetup();
    void calibration();
    long getXRange();
    long getYRange();
};

#endif