#include "Arduino.h"
#include "Archimedes.h"

static uint8_t latch_state;

// Sets a voltage state to the digital pins (12, 4, 7, and 8)
// This is what activates the PWM on the pins
void resetPins(void) {
  uint8_t i;

  digitalWrite(MOTOR_LATCH_PIN, LOW);

  digitalWrite(MOTOR_DATA_PIN, LOW);

  // Init all combination of latch / h-bridge states
  for (i=0; i<8; i++) {
    digitalWrite(MOTOR_CLK_PIN, LOW);

    if (latch_state & _BV(7-i)) {
      digitalWrite(MOTOR_DATA_PIN, HIGH);
    } else {
      digitalWrite(MOTOR_DATA_PIN, LOW);
    }
    digitalWrite(MOTOR_CLK_PIN, HIGH);
  }
  digitalWrite(MOTOR_LATCH_PIN, HIGH);
}

// Setting Arduino digital pins (12, 4, 7, and 8) to OUTPUT mode
void setPins(void) {
  pinMode(MOTOR_LATCH_PIN, OUTPUT);
  pinMode(MOTOR_ENABLE_PIN, OUTPUT);
  pinMode(MOTOR_DATA_PIN, OUTPUT);
  pinMode(MOTOR_CLK_PIN, OUTPUT);

  latch_state = 0;

  // Sets an initial voltage state to the pins
  resetPins();  // "reset"

  // Enable motor pins (part of the shield's internal control signal)
  digitalWrite(MOTOR_ENABLE_PIN, LOW);
}


/******************************************
               MOTORS
******************************************/

ArchimedesMotor1::ArchimedesMotor1() {
  // Set default frequency of 1KHz
	pwmFreq = _BV(CS01);
	setPins();
	
  // Give it its own latch state
	latch_state &= ~_BV(2) & ~_BV(3);
    resetPins();
    
    // Set the timer / clock information
    TCCR2A |= _BV(COM2A1) | _BV(WGM20) | _BV(WGM21);
    TCCR2B = pwmFreq & 0x7;
    OCR2A = 0;

    // Speed/Activation control being set to OUTPUT
    pinMode(11, OUTPUT);
}

void ArchimedesMotor1::run(uint8_t cmd) {
  // Using bit-wise ops to control latch for this motor
  uint8_t a, b;
  a = 2;
  b = 3;
  
  // Control the latch / h-bridge by spending the voltage signals 
  switch (cmd) {
  case FORWARD:
    latch_state |= _BV(a);
    latch_state &= ~_BV(b); 
    resetPins();
    break;
  case BACKWARD:
    latch_state &= ~_BV(a);
    latch_state |= _BV(b); 
    resetPins();
    break;
  case RELEASE:
    latch_state &= ~_BV(a);
    latch_state &= ~_BV(b); 
    resetPins();
    break;
  }
}

// Set the speed of the timer to control the speed of the robot
void ArchimedesMotor1::setSpeed(uint8_t speed) {
  OCR2A = speed;
}

ArchimedesMotor2::ArchimedesMotor2() {
  // Same default freq. as ArchimedesMotor1
	pwmFreq = _BV(CS01);
	setPins();
	
    // Give its own space on the latch, unique to ArchimedesMotor1
    latch_state &= ~_BV(1) & ~_BV(4);
    resetPins();
    
    // Again, settings timers. Uses a diff timer from ArchimedesMotor1
    TCCR2A |= _BV(COM2B1) | _BV(WGM20) | _BV(WGM21);
    TCCR2B = pwmFreq & 0x7;
    OCR2B = 0;
    // Speed/Activation control being set to OUTPUT
    pinMode(3, OUTPUT);
}

void ArchimedesMotor2::run(uint8_t cmd) {
  // Using bit-wise ops to control latch for this motor
  uint8_t a, b;
  a = 1;
  b = 4;
  
  // Control the latch / h-bridge by spending the voltage signals 
  switch (cmd) {
  case FORWARD:
    latch_state |= _BV(a);
    latch_state &= ~_BV(b); 
    resetPins();
    break;
  case BACKWARD:
    latch_state &= ~_BV(a);
    latch_state |= _BV(b); 
    resetPins();
    break;
  case RELEASE:
    latch_state &= ~_BV(a);
    latch_state &= ~_BV(b); 
    resetPins();
    break;
  }
}

// Set the speed of the timer to control the speed of the robot
void ArchimedesMotor2::setSpeed(uint8_t speed) {
  OCR2B = speed;
}
