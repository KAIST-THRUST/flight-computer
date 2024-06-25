/**
 * @file NonBlockingServo.h
 * @brief A servo motor class with non-blocking features.
 */

#ifndef NONBLOCKINGSERVO_H
#define NONBLOCKINGSERVO_H

#include <Servo.h>

class NonBlockingServo {
public:
  NonBlockingServo();
  void write(int degree);  // Rotate servo immediately.
  void begin();            // Begin servo motor.
  void attach();           // Attach servo motor with default pin.
  void attach(int pin);    // Attach to a specific pin.
  void rotate(int degree); // Rotate servo.
  bool isrotating();       // Check if servo is rotating currently.

private:
  Servo servo;
  int target_degree;
  int current_degree;
  unsigned long prev_move_time;
};

#endif
