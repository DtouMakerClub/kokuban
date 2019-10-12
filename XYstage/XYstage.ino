const int PIN_MOTOR_XY_ENABLE = 8;
const int PIN_MOTOR_X_STEP = 2;
const int PIN_MOTOR_X_DIR = 5;
const int PIN_MOTOR_Y_STEP = 3;
const int PIN_MOTOR_Y_DIR = 6;

//motor
const int MOTOR_ENABLE = LOW;
const int MOTOR_DISABLE = !MOTOR_ENABLE;
//motor rotation direction
const bool X_RIGHT = HIGH;
const bool X_LEFT = !X_RIGHT;
const bool Y_RIGHT = HIGH;
const bool Y_LEFT = !Y_RIGHT;


const unsigned long STEPPING_MOTOR_PERIOD_HALF_MS = 10/2;

int amplitudeXPulseNum = 500;
//bool step = LOW;

void setup() {
  //init
  pinMode(PIN_MOTOR_XY_ENABLE,OUTPUT);
  pinMode(PIN_MOTOR_X_STEP,OUTPUT);
  pinMode(PIN_MOTOR_X_DIR,OUTPUT);
  pinMode(PIN_MOTOR_Y_STEP,OUTPUT);
  pinMode(PIN_MOTOR_Y_DIR,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);

  //motorON
  digitalWrite(PIN_MOTOR_XY_ENABLE,MOTOR_ENABLE);
}

void loop() {
  digitalWrite(PIN_MOTOR_X_DIR,X_RIGHT);
  digitalWrite(PIN_MOTOR_Y_DIR,Y_RIGHT);
  digitalWrite(LED_BUILTIN,HIGH);
  for(int i = 0; i < amplitudeXPulseNum; i++){
    digitalWrite(PIN_MOTOR_X_STEP,HIGH);
    digitalWrite(PIN_MOTOR_Y_STEP,HIGH);
    delay(STEPPING_MOTOR_PERIOD_HALF_MS);
    digitalWrite(PIN_MOTOR_X_STEP,LOW);
    digitalWrite(PIN_MOTOR_Y_STEP,LOW);
    delay(STEPPING_MOTOR_PERIOD_HALF_MS);
  }

  digitalWrite(PIN_MOTOR_X_DIR,X_LEFT);
  digitalWrite(PIN_MOTOR_Y_DIR,Y_LEFT);
  digitalWrite(LED_BUILTIN,LOW);
  for(int i = 0; i < amplitudeXPulseNum; i++){
    digitalWrite(PIN_MOTOR_X_STEP,HIGH);
    digitalWrite(PIN_MOTOR_Y_STEP,HIGH);
    delay(STEPPING_MOTOR_PERIOD_HALF_MS);
    digitalWrite(PIN_MOTOR_X_STEP,LOW);
    digitalWrite(PIN_MOTOR_Y_STEP,LOW);
    delay(STEPPING_MOTOR_PERIOD_HALF_MS);
  }
}

