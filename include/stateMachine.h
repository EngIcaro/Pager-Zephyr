#ifndef _SATATEMACHINE_H_
#define _SATATEMACHINE_H_

#include <device.h>
#include <gpio.h>
#include "thread.h"

void state_machine();

void set_ready();

void set_waiting();

void set_charging();

#endif