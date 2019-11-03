#include "arduino.h"
#include "motorController.hpp"

void MotorController::pinSetup()
{
    pinMode(PIN_MOTOR_XY_ENABLE, OUTPUT);
    pinMode(PIN_MOTOR_X_STEP, OUTPUT);
    pinMode(PIN_MOTOR_X_DIR, OUTPUT);
    pinMode(PIN_MOTOR_Y_STEP, OUTPUT);
    pinMode(PIN_MOTOR_Y_DIR, OUTPUT);
    
    //motorON
    digitalWrite(PIN_MOTOR_XY_ENABLE, MOTOR_ENABLE);
    //pulseStateと実際の状況を同じにしておく
    digitalWrite(PIN_MOTOR_X_STEP, pulseStateX);
    digitalWrite(PIN_MOTOR_Y_STEP, pulseStateY);
}

bool MotorController::isX0Limit()
{
    return (digitalRead(PIN_SW_X0) == HIGH); //スイッチ押下時がHIGH、押されていないときはLOW　端にいるときtrue
}
bool MotorController::isX1Limit()
{
    return (digitalRead(PIN_SW_X1) == HIGH); //スイッチ押下時がHIGH、押されていないときはLOW　端にいるときtrue
}
bool MotorController::isY0Limit()
{
    return (digitalRead(PIN_SW_Y0) == HIGH); //スイッチ押下時がHIGH、押されていないときはLOW　端にいるときtrue
}
bool MotorController::isY1Limit()
{
    return (digitalRead(PIN_SW_Y1) == HIGH); //スイッチ押下時がHIGH、押されていないときはLOW　端にいるときtrue
}
bool MotorController::isLimit()
{
    return (isX0Limit() || isX1Limit() || isY0Limit() || isY1Limit());
}

void MotorController::moveXOneStepPlusForCalib()
{
    pulseStateX = !pulseStateX;
    digitalWrite(PIN_MOTOR_X_DIR, X_PLUS);
    digitalWrite(PIN_MOTOR_X_STEP, pulseStateX);
}

void MotorController::moveXOneStepMinusForCalib()
{
    pulseStateX = !pulseStateX;
    digitalWrite(PIN_MOTOR_X_DIR, X_MINUS);
    digitalWrite(PIN_MOTOR_X_STEP, pulseStateX);
}

void MotorController::moveYOneStepPlusForCalib()
{
    pulseStateY = !pulseStateY;
    digitalWrite(PIN_MOTOR_Y_DIR, Y_PLUS);
    digitalWrite(PIN_MOTOR_Y_STEP, pulseStateY);
}

void MotorController::moveYOneStepMinusForCalib()
{
    pulseStateY = !pulseStateY;
    rangeY++; //X方向のパルス数をカウント
    digitalWrite(PIN_MOTOR_Y_DIR, Y_MINUS);
    digitalWrite(PIN_MOTOR_Y_STEP, pulseStateY);
}

void MotorController::calibration()
{
    const int PULSE_NUM_TO_ESCAPE_FROM_LIMIT_SW = 500;
    rangeX = 0;
    rangeY = 0;

    //X1まで移動
    while (!isX1Limit())
    { 
        moveXOneStepPlusForCalib();
        delayMicroseconds(HALF_PERIOD_OF_PULSE_MICROSEC);
    }
    while (!isX0Limit())
    { //X0まで移動
        moveXOneStepMinusForCalib();
        rangeX++;
        delayMicroseconds(HALF_PERIOD_OF_PULSE_MICROSEC);
    }//X終了
    //Y1まで移動
    while (!isY1Limit())
    { 
        moveYOneStepPlusForCalib();
        delayMicroseconds(HALF_PERIOD_OF_PULSE_MICROSEC);
    }
    while (!isY0Limit())
    { //Y0まで移動
        moveYOneStepMinusForCalib();
        rangeY++;
        delayMicroseconds(HALF_PERIOD_OF_PULSE_MICROSEC);
    }//Y終了
}

long MotorController::getXRange()
{
    return rangeX;
}

long MotorController::getYRange()
{
    return rangeY;
}
