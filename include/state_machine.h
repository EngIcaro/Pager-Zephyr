#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#include <device.h>
#include <gpio.h>
#include "thread.h"
#include "bt_node.h"

void state_machine();

void set_ready();

void set_waiting();

void set_charging();

int get_state();

#endif