#ifndef _Archimedes_h_
#define _Archimedes_h_

#include <inttypes.h>
#include <avr/io.h>

// Human-readable versions commands sent to Archimedes lib
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

// Human-readable versions of the pins used for shield
#define MOTOR_LATCH_PIN 12
#define MOTOR_CLK_PIN 4
#define MOTOR_ENABLE_PIN 7
#define MOTOR_DATA_PIN 8

class ArchimedesMotor1
{
 public:
  ArchimedesMotor1();
  void run(uint8_t);
  void setSpeed(uint8_t);
private:
  uint8_t pwmFreq;
};

class ArchimedesMotor2
{
 public:
  ArchimedesMotor2();
  void run(uint8_t);
  void setSpeed(uint8_t);
private:
  uint8_t pwmFreq;
};

#endif
