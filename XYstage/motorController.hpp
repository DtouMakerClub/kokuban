#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

//#include <TimerOne.h>

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
    long positionXStep = 0;
    long positionYStep = 0;
    long targetXStep = 45824/2;//適当に中央ぐらい
    long targetYStep = 63140/2;

    //const long controllSycleTimeUS = 100000;//制御周期、設定値ではない便宜上の値。ほんとの値は知らない。制御の単位時間
    const long SPEED_X_HZ_MAX = 5000;
    const long SPEED_Y_HZ_MAX = 5000;
    const long SPEED_X_HZ_MIN = 100;
    const long SPEED_Y_HZ_MIN = 100;
    long speedXHz = SPEED_X_HZ_MAX;//パルスの周波数
    long speedYHz = SPEED_Y_HZ_MAX;//15000;

    //long periodXUS = 1000000 / speedXHz;//単位時間/周波数　-> 周期 これをタイマの周期とする
    //long periodYUS = 1000000 / speedYHz;

    long convert2US(long Hz);

    //キャリブレーション用
    bool calibFinished = false;
    long rangeX = 0;
    long rangeY = 0;
    const int HALF_PERIOD_OF_PULSE_MICROSEC = 150 / 2;
    void moveXOneStepPlusForCalib();
    void moveXOneStepMinusForCalib();
    void moveYOneStepPlusForCalib();
    void moveYOneStepMinusForCalib();

    //リミットスイッチ
    bool isX0Limit();
    bool isX1Limit();
    bool isY0Limit();
    bool isY1Limit();
    bool isLimit();
    
    //速度調整
    long getXStepDistance();
    long getYStepDistance();


    //const unsigned long STEPPING_MOTOR_PERIOD_HALF_US = 300; //100;//周期はこれの２倍
public:
    void pinSetup();
    void calibration();
    bool hasCalibFinished();
    int setXSpeedToTarget();
    long getTimerPeriodForX();
    long getTimerPeriodForY();
    long getXRange();
    long getYRange();
    long getPositionXStep();
    long getPositionYStep();
    long calcSpeed();

    //目標地点追従用
    void setTargetPoint(long x, long y);
    void toggleXPulseAndUpdatePosition();
    void toggleYPulseAndUpdatePosition();
};

#endif