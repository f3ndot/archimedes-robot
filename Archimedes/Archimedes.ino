#include <Archimedes.h>
#define SAME_SPEED -1
#define MAX_SPEED 255

// Instantiates two motors
ArchimedesMotor1 rightMotor;
ArchimedesMotor2 leftMotor;

// Give timers an initial speed
void setup() {  
  rightMotor.setSpeed(100);     // set the speed to 100/255
  leftMotor.setSpeed(100);     // set the speed to 100/255
}

// High-Level robot set speed func.
void setRobotSpeed(int speed) {
  if(0 <= speed <= 255) {
    rightMotor.setSpeed(speed);
    leftMotor.setSpeed(speed);
  }
}

// High-Level robot forward func.
void goForward(int robotSpeed) {
 setRobotSpeed(robotSpeed);
 leftMotor.run(BACKWARD); 
 rightMotor.run(BACKWARD); 
}

// High-Level robot backward func.
void goBackward(int robotSpeed) {
  setRobotSpeed(robotSpeed);
  leftMotor.run(FORWARD); 
  rightMotor.run(FORWARD); 
}

// High-Level robot turn left func.
void turnLeft(int turnSpeed) {
  setRobotSpeed(turnSpeed);
  leftMotor.run(BACKWARD); 
  rightMotor.run(FORWARD); 
}

// High-Level robot turn right func.
void turnRight(int turnSpeed) {
  setRobotSpeed(turnSpeed);
  leftMotor.run(FORWARD); 
  rightMotor.run(BACKWARD); 
}

// High-Level robot stop func.
// Not "active" breaking; uses friction to stop
void goRelease() {
  leftMotor.run(RELEASE); 
  rightMotor.run(RELEASE); 
}

// Lab 3 demo commands
void loop() {
  goForward(MAX_SPEED);
  delay(9000);
  
  goBackward(0.75*MAX_SPEED);
  delay(6000);

  goBackward(0.50*MAX_SPEED);
  delay(8000);
  
  goBackward(0.25*MAX_SPEED);
  delay(8000);
  
  goRelease();
  delay(2000);
}
