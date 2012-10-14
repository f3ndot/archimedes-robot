#include <Archimedes.h>
#define SAME_SPEED -1
#define MAX_SPEED 255

#define BOOTUP_AVG_NUM 10
#define BOOTUP_JUNK_NUM 10

#define STATE_PANIC 0
#define STATE_SAFE 1

#define GO_SPEED 75

#define ALERT_TOLERENCE 50

// Instantiates two motors
ArchimedesMotor1 rightMotor;
ArchimedesMotor2 leftMotor;

int bootAvg = 0;
int deltaState = STATE_SAFE;
int sensorValue = 0;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Calculating table reference value");
  
  int distanceRead = 0;

  for(int i = 0; i <= (BOOTUP_AVG_NUM + BOOTUP_JUNK_NUM); i++) {
    distanceRead = analogRead(A0);
    if(i > BOOTUP_JUNK_NUM) {
      Serial.print ("Read: "); 
      Serial.print (distanceRead); 
      Serial.print (", Total: "); 
      bootAvg += distanceRead;
      Serial.println (bootAvg);
    } else {
      Serial.print ("Ignored: "); 
      Serial.print (distanceRead); 
      Serial.print (", Total: "); 
      Serial.println (bootAvg);
    }
    delay(100);
  }

  bootAvg /= BOOTUP_AVG_NUM;

  Serial.print("Average: ");
  Serial.println(bootAvg);


  Serial.println("Turning on motors");
  goForward(GO_SPEED);
}

void setRobotSpeed(int speed) {
  if(0 <= speed <= 255) {
    rightMotor.setSpeed(speed);     // set the speed to 200/255
    leftMotor.setSpeed(speed);     // set the speed to 200/255     
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

boolean isPanicking() {
  sensorValue = analogRead(A0);
  if((bootAvg - sensorValue) >= ALERT_TOLERENCE) {
    Serial.print("*** TABLE DROP DETECTED! ALERT: ");
    Serial.print(sensorValue);
    Serial.println(" ***");
    return true;
  } else {
    return false;
  }
}

// checks previous motor command ever 0.01 seconds for panic
void safeDelay(int time) {
  int i = 0;
  while(i < time) {
    if(isPanicking()) {
      Serial.println("******* DELAY EXIT! PANIC DETECTED ******");
      return;
    }
    i += 10;
    delay(10);
  }
}

void loop() {
  if(isPanicking()) {
    if(deltaState == STATE_SAFE) {
      // Just entered panic state: attempt recovery
      deltaState = STATE_PANIC;
      goBackward(200);
      delay(200);
      turnLeft(100);
      delay(100);
      goRelease();
    } else {
      // Still panicking / last recovery attempt did not work
      turnLeft(100);
      delay(100);
      goRelease();      
    }
  } else {
    if(deltaState == STATE_PANIC) {
      deltaState = STATE_SAFE;
      goForward(GO_SPEED);
    }
  }

  delay(10);
}
