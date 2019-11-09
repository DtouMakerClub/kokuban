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

    //パルス出力用のtimer
    // Timer1.initialize(periodXUS);
    // Timer1.attachInterrupt(toggleXPulseAndUpdatePosition);//&MotorController::toggleXPulseAndUpdatePosition);
    // Timer1.stop();
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

void MotorController::toggleXPulseAndUpdatePosition()
{
    int direction;

    if (calibFinished)
    {
        if (targetXStep == positionXStep)
        {
            direction = -1; //なにもしない
        }
        else if (targetXStep > positionXStep)
        {
            if(!isX1Limit()){
                direction = X_PLUS;
            }
            else{
                direction = -1;
            }
        }
        else if (targetXStep < positionXStep)
        {
            if(!isX0Limit()){
                direction = X_MINUS;
            }
            else{
                direction = -1;
            }
        }

        if (direction == X_PLUS && positionXStep < rangeX)
        {
            positionXStep++;
            pulseStateX = !pulseStateX;
        }
        else if (direction == X_MINUS && positionXStep > 0)
        {
            positionXStep--;
            pulseStateX = !pulseStateX;
        }

        if (direction == X_PLUS || direction == X_MINUS)
        {
            digitalWrite(PIN_MOTOR_X_DIR, direction);
            digitalWrite(PIN_MOTOR_X_STEP, pulseStateX);
        }
    }
}

void MotorController::toggleYPulseAndUpdatePosition()
{
    int direction;

    if (calibFinished)
    {
        if (targetYStep == positionYStep)
        {
            direction = -1; //なにもしない
        }
        else if (targetYStep > positionYStep)
        {
            if(!isY1Limit()){
                direction = Y_PLUS;
            }
            else{
                direction = -1;
            }
        }
        else if (targetYStep < positionYStep)
        {
            if(!isY0Limit()){
                direction = Y_MINUS;
            }
            else{
                direction = -1;
            }
        }

        if (direction == Y_PLUS && positionYStep < rangeY)
        {
            positionYStep++;
            pulseStateY = !pulseStateY;
        }
        else if (direction == Y_MINUS && positionYStep > 0)
        {
            positionYStep--;
            pulseStateY = !pulseStateY;
        }

        if (direction == Y_PLUS || direction == Y_MINUS)
        {
            digitalWrite(PIN_MOTOR_Y_DIR, direction);
            digitalWrite(PIN_MOTOR_Y_STEP, pulseStateY);
        }
    }
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
    } //X終了
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
    } //Y終了
    calibFinished = true;
}

bool MotorController::isCalibFinished()
{
    return calibFinished;
}

int MotorController::setXSpeedToTarget()
{
    if (calibFinished)
    {
        if (targetXStep == positionXStep)
        {
            //何もしないでいい
        }
        else
        { //ほんとはこの関数をXYの速度調整用に変える。
            //Timer1.initialize(periodXUS);
        }
    }
    return periodXUS;
}

long MotorController::getTimerPeriodForX()
{
    return periodXUS;
}

long MotorController::getTimerPeriodForY()
{
    return periodYUS;
}

long MotorController::getXRange()
{
    return rangeX;
}

long MotorController::getYRange()
{
    return rangeY;
}

void MotorController::setTargetPoint(long x, long y){
    if( 0 <= x && x <= rangeX && 0 <= y && y <= rangeY ){
        targetXStep = x;
        targetYStep = y;        
    }
}