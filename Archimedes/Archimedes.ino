#include <AFMotor.h>

#define STATE_PANIC 0
#define STATE_SAFE 1

#define MOTION_LEFT 0
#define MOTION_RIGHT 1
#define MOTION_FORWARD 2
#define MOTION_BACKWARD 3
#define MOTION_STOPPED 4

#define GO_SPEED 70

#define BAD_OBJECT 150

// Instantiates two motors
AF_DCMotor rightMotor(1);
AF_DCMotor leftMotor(2);

int currentMotion = MOTION_STOPPED;

int lastLeftState = STATE_SAFE;
int lastRightState = STATE_SAFE;

int leftSensorValue = 0;
int rightSensorValue = 0;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps

  Serial.println("**** BOOTING UP OR RESET ****");
  goForward(GO_SPEED);
}

void setRobotSpeed(int speed) {
  if(0 <= speed <= 255) {
    rightMotor.setSpeed(speed);     // set the speed to 200/255
    leftMotor.setSpeed(speed);     // set the speed to 200/255     
  }
}
void goForward(int robotSpeed) {
  if(currentMotion != MOTION_FORWARD) {
    Serial.println("CHANGING FORWARD");
    Serial.println("CHANGING FORWARD");
    Serial.println("CHANGING FORWARD");
    currentMotion = MOTION_FORWARD;
    setRobotSpeed(robotSpeed);
    leftMotor.run(BACKWARD); 
    rightMotor.run(BACKWARD); 
  }
}
void goBackward(int robotSpeed) {
  if(currentMotion != MOTION_BACKWARD) {
    Serial.println("CHANGING BACKWARD");
    Serial.println("CHANGING BACKWARD");
    Serial.println("CHANGING BACKWARD");
    currentMotion = MOTION_BACKWARD;
    setRobotSpeed(robotSpeed);
    leftMotor.run(FORWARD); 
    rightMotor.run(FORWARD); 
  }
}
void turnLeft(int turnSpeed) {
  if(currentMotion != MOTION_LEFT) {
    Serial.println("CHANGING LEFT");
    Serial.println("CHANGING LEFT");
    Serial.println("CHANGING LEFT");
    currentMotion = MOTION_LEFT;
    setRobotSpeed(turnSpeed);
    leftMotor.run(FORWARD); 
    rightMotor.run(BACKWARD); 
  }
}
void turnRight(int turnSpeed) {
  if(currentMotion != MOTION_RIGHT) {
    Serial.println("CHANGING RIGHT");
    Serial.println("CHANGING RIGHT");
    Serial.println("CHANGING RIGHT");
    currentMotion = MOTION_RIGHT;
    setRobotSpeed(turnSpeed);
    leftMotor.run(BACKWARD); 
    rightMotor.run(FORWARD); 
  }
}
void goRelease() {
  Serial.println("RELEASE");
  Serial.println("RELEASE");
  Serial.println("RELEASE");
  currentMotion = MOTION_STOPPED;
  leftMotor.run(RELEASE); 
  rightMotor.run(RELEASE); 
}

boolean leftPanicCheck() {
  leftSensorValue = analogRead(A0);
  if(leftSensorValue >= BAD_OBJECT) {
    lastLeftState = STATE_PANIC;
    return true; 
  } else {
    lastLeftState = STATE_SAFE;
    return false;
  }
}
boolean rightPanicCheck() {
  rightSensorValue = analogRead(A3);
  if(rightSensorValue >= BAD_OBJECT) {
    lastRightState = STATE_PANIC;
    return true; 
  } else {
    lastRightState = STATE_SAFE;
    return false;
  }
}

void loop() {

  leftPanicCheck();
  Serial.print("Left: ");
  Serial.print(leftSensorValue);
  
  rightPanicCheck();
  Serial.print(",  Right: ");
  Serial.println(rightSensorValue);

  if(lastRightState == STATE_PANIC && lastLeftState == STATE_PANIC) {
    Serial.println(" *** BOTH PANIC *** ");
    goBackward(GO_SPEED);
    delay(500);
  } else if(lastRightState == STATE_PANIC) {
    Serial.println(" *** RIGHT PANIC *** ");
    goBackward(GO_SPEED);
    delay(500);
    turnRight(GO_SPEED);
    delay(300);
  } else if(lastLeftState == STATE_PANIC) {
    Serial.println(" *** LEFT PANIC *** ");
    goBackward(GO_SPEED);
    delay(500);
    turnLeft(GO_SPEED);
    delay(300);
  } else {
    goForward(GO_SPEED);
  }
  

  delay(10);
}