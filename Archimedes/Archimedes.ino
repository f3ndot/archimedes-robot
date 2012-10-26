/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.
 
 This example code is in the public domain.
 */

#include <AFMotor.h>
// Motor stuff
#define SAME_SPEED -1
#define MAX_SPEED 255
#define GO_SPEED 45
#define LEFT_MOTOR_ADJUST 6

// Tweaks to account for sensor variance
#define LEFT_ADJUST 100
#define CENTRE_ADJUST 0
#define RIGHT_ADJUST 0

// set up analog ports
#define LEFT_PORT A2
#define CENTRE_PORT A0
#define RIGHT_PORT A5

#define SETUP_COUNT 20

// semantic read state
#define UNKNOWN -1
#define TAPE 1
#define FLOOR 0

// The calculated blacktape and whitefloor values
int blackTape = 0;
int whiteFloor = 0;

// The scan values from last read
int lastLeft = 0;
int lastCentre = 0;
int lastRight = 0;

// Relative threshold calculated from the difference between floor and tape value divided by two
int stateThreshold = 0;

// On tope or on floor states
int leftState = UNKNOWN;
int centreState = UNKNOWN;
int rightState = UNKNOWN;

// Instantiates two motors
AF_DCMotor rightMotor(1);
AF_DCMotor leftMotor(2);

// Special states for detecting corners
boolean turningHardRight = false;
boolean turningHardLeft = false;

// Global reverse mode
boolean reverseMode = false;
int reverseFlagCounter = 0;

///////////////////////////////////////////////////////////////////////////
////  IR SENSOR FUNCTIONS
///////////////////////////////////////////////////////////////////////////

boolean onBlackTape(int val) {
  if((blackTape - stateThreshold) <= val) {
    return true;
  } else {
    return false;
  }
}


int readLeft() {
  lastLeft = analogRead(LEFT_PORT) + LEFT_ADJUST;
  if(onBlackTape(lastLeft)) {
    leftState = TAPE;
  } else {
    leftState = FLOOR;    
  }
  return lastLeft;
}
int readCentre() {
  lastCentre = analogRead(CENTRE_PORT) + CENTRE_ADJUST;
  if(onBlackTape(lastCentre)) {
    centreState = TAPE;
  } else {
    centreState = FLOOR;    
  }
  return lastCentre;
}
int readRight() {
  lastRight = analogRead(RIGHT_PORT) + RIGHT_ADJUST;
  if(onBlackTape(lastRight)) {
    rightState = TAPE;
  } else {
    rightState = FLOOR;    
  }
  return lastRight;
}
void scanFloor() {
  readLeft();
  readCentre();
  readRight();
}
void printScan() {
  Serial.print("Left: ");
  Serial.print(leftState);
  Serial.print(",  Centre: ");
  Serial.print(centreState);
  Serial.print(",  Right: ");
  Serial.println(rightState);
}
void printRawScan() {
  Serial.print("Left: ");
  Serial.print(lastLeft);
  Serial.print(",  Centre: ");
  Serial.print(lastCentre);
  Serial.print(",  Right: ");
  Serial.println(lastRight);
}


///////////////////////////////////////////////////////////////////////////
////  DC MOTOR FUNCTIONS
///////////////////////////////////////////////////////////////////////////

void setRobotSpeed(int speed) {
  if(0 <= speed <= 255) {
    rightMotor.setSpeed(speed);     // set the speed to 200/255
    leftMotor.setSpeed((speed+LEFT_MOTOR_ADJUST));     // set the speed to 200/255     
  }
}
void goForward(int robotSpeed) {
  Serial.println("FORWARD");
  setRobotSpeed(robotSpeed);
  leftMotor.run(BACKWARD); 
  rightMotor.run(BACKWARD); 
}
void goBackward(int robotSpeed) {
  Serial.println("BACKWARD");
  setRobotSpeed(robotSpeed);
  leftMotor.run(FORWARD); 
  rightMotor.run(FORWARD); 
}
void turnLeft(int turnSpeed) {
  Serial.println("LEFT");
  setRobotSpeed(turnSpeed);
  leftMotor.run(FORWARD); 
  rightMotor.run(BACKWARD); 
}
void turnRight(int turnSpeed) {
  Serial.println("RIGHT");
  setRobotSpeed(turnSpeed);
  leftMotor.run(BACKWARD); 
  rightMotor.run(FORWARD); 
}
void goRelease() {
  Serial.println("RELEASE");
  leftMotor.run(RELEASE); 
  rightMotor.run(RELEASE); 
}


///////////////////////////////////////////////////////////////////////////
////  SETUP
///////////////////////////////////////////////////////////////////////////

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Setting Up...");  

  int left_total = 0;
  int centre_total = 0;
  int right_total = 0;
  for(int i = 0; i < SETUP_COUNT; i++) {
    Serial.print("Count: ");Serial.print(i);Serial.print("/");Serial.print(SETUP_COUNT);Serial.print("  | ");
    left_total += readLeft();
    centre_total += readCentre();
    right_total += readRight();
    printRawScan();
    delay(10);
  }
  
  left_total /= SETUP_COUNT;
  centre_total /= SETUP_COUNT;
  right_total /= SETUP_COUNT;
  
  blackTape = centre_total;
  whiteFloor = ((left_total + right_total) / 2);
  stateThreshold = ((blackTape - whiteFloor) / 2);
  
  for(int i = 0; i < 3; i++) {
    Serial.print("AVG LEFT: ");
    Serial.print(left_total);
    Serial.print(",  AVG CENTRE: ");
    Serial.print(centre_total);
    Serial.print(",  AVG RIGHT: ");
    Serial.println(right_total);
  }
  delay(50);
  for(int i = 0; i < 3; i++) {
    Serial.print("BLACK TAPE: ");
    Serial.print(blackTape);
    Serial.print(",  WHITE FLOOR: ");
    Serial.print(whiteFloor);
    Serial.print(",  LOWER BLACK THRESHOLD: ");
    Serial.println((blackTape - stateThreshold));
  }

  leftState = UNKNOWN;
  centreState = UNKNOWN;
  rightState = UNKNOWN;
  delay(1000);
  Serial.println("Turning on motors!!!");
  goForward(GO_SPEED);
}


///////////////////////////////////////////////////////////////////////////
////  MAIN LOOP
///////////////////////////////////////////////////////////////////////////

void loop() {
  scanFloor();
  printScan();
  //printRawScan();


  if(leftState == FLOOR && centreState == TAPE && rightState == FLOOR) {
    goForward(GO_SPEED);
    turningHardLeft = false;
    turningHardRight = false;
  }
  if(leftState == TAPE && centreState == FLOOR && rightState == FLOOR) {
    if(!turningHardLeft && !turningHardRight) {
      Serial.println("CORRECTING: ADJUSTING LEFT");
      Serial.println("CORRECTING: ADJUSTING LEFT");
      Serial.println("CORRECTING: ADJUSTING LEFT");
      Serial.println("---");
      turnLeft(GO_SPEED);
      delay(75);
      goForward(GO_SPEED);
    } else {
      Serial.println("@@@ HARD TURN OVERRIDE. IGNORE CORRECTION @@@");
      Serial.println("@@@ HARD TURN OVERRIDE. IGNORE CORRECTION @@@");
      Serial.println("@@@ HARD TURN OVERRIDE. IGNORE CORRECTION @@@");
    }
  }
  //
  if(leftState == FLOOR && centreState == FLOOR && rightState == TAPE) {
    if(!turningHardLeft && !turningHardRight) {
      Serial.println("CORRECTING: ADJUSTING RIGHT");
      Serial.println("CORRECTING: ADJUSTING RIGHT");
      Serial.println("CORRECTING: ADJUSTING RIGHT");
      Serial.println("---");
      turnRight(GO_SPEED);
      delay(75);
      goForward(GO_SPEED);    
    } else {
      Serial.println("@@@ HARD TURN OVERRIDE. IGNORE CORRECTION @@@");
      Serial.println("@@@ HARD TURN OVERRIDE. IGNORE CORRECTION @@@");
      Serial.println("@@@ HARD TURN OVERRIDE. IGNORE CORRECTION @@@");
    }

  }
  
  if(leftState == FLOOR && centreState == TAPE && rightState == TAPE) {
    Serial.println("*** HARD TURN RIGHT ***");
    Serial.println("*** HARD TURN RIGHT ***");
    Serial.println("*** HARD TURN RIGHT ***");
    Serial.println("---");  
//    goForward(GO_SPEED);
//    delay(600);
    turnRight(GO_SPEED);
    turningHardRight = true;
  }
  if(leftState == TAPE && centreState == TAPE && rightState == FLOOR) {
    Serial.println("*** HARD TURN LEFT ***");
    Serial.println("*** HARD TURN LEFT ***");
    Serial.println("*** HARD TURN LEFT ***");
    Serial.println("---");  
//    goForward(GO_SPEED);
//    delay(600);
    turnLeft(GO_SPEED);
    turningHardLeft = true;

  }
  if(leftState == FLOOR && centreState == FLOOR && rightState == FLOOR) {
    if(!turningHardLeft && !turningHardRight) {
      boolean stopBot = true;
      for(int i = 0; i < 5; i++) {
        scanFloor();
        if(leftState == TAPE || centreState == TAPE || rightState == TAPE) {
          stopBot = false;
          Serial.println("Resetting Flag Counter...");
          reverseFlagCounter = 0;
          break;
        }
        delay(100);
      }
      
      if(stopBot) {
        if(reverseFlagCounter <= 3) {
          Serial.println("*** REVERSE TRIPPED ***");
          Serial.println("*** REVERSE TRIPPED ***");
          Serial.println("*** REVERSE TRIPPED ***");
          turnLeft(GO_SPEED);
          turningHardLeft = true;
        } else {
          Serial.println("Increasing Flag Counter...");
          reverseFlagCounter++;
          goRelease();
          delay(500);          
          goBackward(GO_SPEED);
        }
      }
      
    }
  }  

  
  delay(10);        // delay in between reads for stability
}