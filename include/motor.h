#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "output.h"

#define SLEEP_MOTOR 3	//seconds

int motors_configure();

void set_motor(char motor, u8_t value);

void call_motors();

#endif