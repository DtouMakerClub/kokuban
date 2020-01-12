#include "motorController.hpp"
#include <TimerOne.h>
#include "ellipsetable.hpp"


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
};

State state = NOT_INITIALIZED;

CommunicationData receiveData;
bool commandReceived = false;



char receiveBuf = 0;
char command = 0;
int targetIndex = 0;//楕円動作用


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
  /*
  if(Serial.available() > 0 ){
    receiveData.initialize();
    receiveBuf = Serial.read();
    if(receiveBuf == 'i' || receiveBuf =='I'){
      receiveData.setCommand('I');
    }
    else if(receiveData == 'M'){
      //フォーマット通りだった時だけ移動命令と解釈する
      dataDigitCounter = 0;
      while(serial.available() > 0 && dataDigitCounter < dataDigitSum){
        receiveBuf = Serial.read();
        if(dataDigitCounter<dataDigitX){
          
        }else if(dataDigitCounter< (dataDigitX + dataDigitY) {
          
        }
      }
      if(dataDigitCounter == (dataDigitMaxNum + dataDigitMaxNum)){
        receiveData.setCommand('M');
      }
    }
    else if(receiveData == 'F'){
      receiveData.setCommand('F');
    }
  }

  */

  if(commandReceived){
    //toggleLED();
    commandReceived = false;
  }
 
  if(command == 'I' || command == 'i'){
    Serial.print("start calibration\n");

    motorController.calibration();

    Serial.print("X range : ");
    Serial.print(motorController.getXRange(),DEC);
    Serial.print("\nY range : ");
    Serial.print(motorController.getYRange(),DEC);

    state = MOVE;
  }else if(state == MOVE && command == 'M'){

  }

  if(motorController.isCalibFinished()){
    // motorController.setXSpeedToTarget();

    
    //楕円形に動かす
    // motorController.setTargetPoint(ellipse[targetIndex][0],ellipse[targetIndex][1]);
    // targetIndex++;
    // targetIndex %= TARGET_MAX;
    // delay(100);
    //楕円形ここまで

    //motorController.setTargetPoint();
    delay(10);
  }

}

void handler1(){
  motorController.toggleXPulseAndUpdatePosition();
  motorController.toggleYPulseAndUpdatePosition();
}
void handler2(){
  motorController.toggleYPulseAndUpdatePosition();
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
//mainloopでコマンドを処理する前にコマンドを書き換えてしまう可能性があるのでmainloop内に入れた方がいいかも
void serialEvent() {
  String inputString = "";
  bool stringComplete = false;

  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      //toggleLED();
    }
  }
  
  if(stringComplete){
    receiveData.initialize();
    //toggleLED();//ここにはくる

    if (inputString.charAt(0) == 'I')
    {
      if (inputString.length() == 1)
      {
        receiveData.setCommand('I');
        commandReceived = true;
      }
    }
    else if (inputString[0] == 'M')
    {
      if (inputString.length() == 3)
      {
        receiveData.setCommand('M');
        receiveData.setX(inputString[1]);
        receiveData.setY(inputString[2]);
        commandReceived = true;
      }
    }
    else if (inputString[0] == 'F')
    {
      if (inputString.length() == 1)
      {
        receiveData.setCommand('F');
        commandReceived = true;
      }
    }

    stringComplete = false;
  }
  
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