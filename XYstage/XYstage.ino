#include "motorController.hpp"
#include <TimerOne.h>
#include "ellipsetable.hpp"


const int DATA_LENGHT_INIT = 2;
const int DATA_LENGHT_MOVE = 4;
const int DATA_LENGHT_FREEZE = 2;

const unsigned char SEND_RANGE = 255;

enum State{
  NOT_INITIALIZED,
  MOVE,
  STAY
};

class CommunicationData{
private:
  char _command;
  unsigned char _x;
  unsigned char _y;
public:
  CommunicationData():_command(0),_x(0),_y(0){}

  void initialize(){
    _command = 0;
    _x = 0;
    _y = 0;
  }
  void setCommand(char command){
    _command = command;
  }
  void setX(unsigned char x){
    _x = x;
  }
  void setY(unsigned char y){
    _y = y;
  }
  char getCommand(){
    return  _command;
  }
  unsigned char getX(){
    return _x;
  }
  unsigned char getY(){
    return _y;
  }
};

State state = NOT_INITIALIZED;

CommunicationData receiveData;
bool commandReceived = false;

String inputString = "";
bool stringComplete = false;


const unsigned long STEPPING_MOTOR_PERIOD_HALF_US = 300;//100;//周期はこれの２倍

MotorController motorController = MotorController();

void setup() {
  Serial.begin(9600);
  Serial.print("This is XYstage.ino\n");

  motorController.pinSetup();
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);

  //パルス出力用のtimer
  Timer1.initialize(motorController.getTimerPeriodForX());
  Timer1.attachInterrupt(handler1);//&MotorController::toggleXPulseAndUpdatePosition);

  //Timer2.initialize(motorController.getTimerPeriodForY());//こうしたかった
  //Timer2.attachInterrupt(handler2);
}

void loop() {
  //受信データのパース
  while (Serial.available()) {

    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      receiveData.initialize();

      if (inputString.charAt(0) == 'I')
      {
        if (inputString.length() == DATA_LENGHT_INIT)
        {
          receiveData.setCommand('I');
          commandReceived = true;
        }
      }
      else if (inputString[0] == 'M')
      {
        if (inputString.length() == DATA_LENGHT_MOVE)
        {
          receiveData.setCommand('M');
          receiveData.setX(inputString[1]);
          receiveData.setY(inputString[2]);
          commandReceived = true;
        }
      }
      else if (inputString[0] == 'F')
      {
        if (inputString.length() == DATA_LENGHT_FREEZE)
        {
          receiveData.setCommand('F');
          commandReceived = true;
        }
      }
      inputString = "";
      stringComplete = false;
    }
  }

  if(commandReceived){
    if(receiveData.getCommand() == 'I' ){
      Serial.print("start calibration\n");
      
      motorController.calibration();

      Serial.print("X range : ");
      Serial.print(motorController.getXRange(),DEC);
      Serial.print("\nY range : ");
      Serial.print(motorController.getYRange(),DEC);
      Serial.print('\n');

      state = MOVE;
    }
    else if(receiveData.getCommand() == 'M'){
      if(state == MOVE){
        //受信値をモータ―のレンジに変換(OK)
        //それを平滑化(まだ)
        //目標値更新(OK)
        motorController.setTargetPoint(
          convertToMotorStepRange(receiveData.getX(),motorController.getXRange()),
          convertToMotorStepRange(receiveData.getY(),motorController.getYRange())
        );
        // Serial.print(convertToMotorStepRange(receiveData.getX(),motorController.getXRange()), DEC);
        // Serial.print('\n');
        // Serial.print(convertToMotorStepRange(receiveData.getY(),motorController.getYRange()), DEC);
        // Serial.print('\n');
      }
    }
    else if(receiveData.getCommand() == 'F'){
      state = STAY;
    }
    
    commandReceived = false;
  }

  //指令位置へ動作するための速度計算等（の予定）
  if(motorController.isCalibFinished()){

    if(state == MOVE){
      //
    }
    // motorController.setXSpeedToTarget();

    //motorController.setTargetPoint();
    delay(10);
  }

  //現在位置の送信
  if(motorController.isCalibFinished()){
    sendToPC(
      convertToSendRange(motorController.getPositionXStep(),motorController.getXRange()),
      convertToSendRange(motorController.getPositionYStep(),motorController.getYRange())
      );
  }

}

void handler1(){
  motorController.toggleXPulseAndUpdatePosition();
  motorController.toggleYPulseAndUpdatePosition();
}
void handler2(){
  motorController.toggleYPulseAndUpdatePosition();
}

void sendToPC(const unsigned char x, const unsigned char y){
  Serial.write('K');
  Serial.write(x);
  Serial.write(y);
  Serial.write('\n');
}

unsigned char convertToSendRange(const long motorStepNum, const long motorStepRange){
  return motorStepNum * SEND_RANGE / motorStepRange;
}

long convertToMotorStepRange(const unsigned char recvPosition, const long motorStepNumRange){
  return recvPosition * motorStepNumRange /SEND_RANGE;
}

void toggleLED(){
  static bool state = LOW;

  if(state == LOW){
    state = HIGH;
  }else{
    state = LOW;
  }
  digitalWrite(LED_BUILTIN,state);
}