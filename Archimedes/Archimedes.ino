#include <AFMotor.h>
#define SAME_SPEED -1

AF_DCMotor rightMotor(1);
AF_DCMotor leftMotor(2);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Motor test!");
  
  rightMotor.setSpeed(100);     // set the speed to 200/255
  leftMotor.setSpeed(100);     // set the speed to 200/255
}

void setRobotSpeed(int speed) {
  if(0 <= speed <= 255) {
    rightMotor.setSpeed(speed);     // set the speed to 200/255
    leftMotor.setSpeed(speed);     // set the speed to 200/255     
  }
}
void goForward(int robotSpeed) {
 setRobotSpeed(robotSpeed);
 leftMotor.run(BACKWARD); 
 rightMotor.run(BACKWARD); 

}
void goBackward(int robotSpeed) {
 setRobotSpeed(robotSpeed);
 leftMotor.run(FORWARD); 
 rightMotor.run(FORWARD); 
}
void turnLeft(int turnSpeed) {
 setRobotSpeed(turnSpeed);
 leftMotor.run(BACKWARD); 
 rightMotor.run(FORWARD); 
}
void turnRight(int turnSpeed) {
 setRobotSpeed(turnSpeed);
 leftMotor.run(FORWARD); 
 rightMotor.run(BACKWARD); 
}
void goRelease() {
 leftMotor.run(RELEASE); 
 rightMotor.run(RELEASE); 
}


void loop() {
  goForward(SAME_SPEED);
  delay(9000);
  
  goBackward(0.75*255);
  delay(6000);

  goBackward(0.50*255);
  delay(8000);
  
  goBackward(0.25*255);
  delay(8000);
  
  goRelease();
  delay(2000);
  
/*  Serial.println("forward");
  
  goForward(SAME_SPEED);
  delay(2000);

  Serial.println("backward");
  goBackward(SAME_SPEED);
  delay(2000);
  
  Serial.println("turn left");
  turnLeft(SAME_SPEED);
  delay(2000);
  
  Serial.println("turn right");
  turnRight(SAME_SPEED);
  delay(2000);
  
  Serial.println("released motors");
  goRelease();
  delay(2000); */
}
