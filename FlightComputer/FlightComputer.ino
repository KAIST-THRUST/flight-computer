/**
 * @file FlightComputer.ino
 * @brief Main sketch file for execution of the flight computer.
 */

#include "StateMachine.h"

static StateMachine rocket_state_machine;

void setup() { rocket_state_machine.begin(); }

void loop() {
  /*-----------------------------------------------------------------*/
  /* Servo part. */
  // updateServoFromSerial(serv);
  /*-----------------------------------------------------------------*/
  switch (rocket_current_state) {
  case RocketState::ST_BOOT:
    /* Boot state. */
    rocket_state_machine.boot();
    break;

  case RocketState::ST_STAND_BY:
    /* Stand by state. */
    rocket_state_machine.standBy();
    break;

  case RocketState::ST_BURN:
    /* Burn state. */
    rocket_state_machine.burn();
    break;

  case RocketState::ST_COAST:
    /* Coast state. */
    rocket_state_machine.coast();
    break;

  case RocketState::ST_DESCENT:
    /* Descent state. */
    break;

  case RocketState::ST_LANDED:
    /* Landed state. */
    break;
  }
}
