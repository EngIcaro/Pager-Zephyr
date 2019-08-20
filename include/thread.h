#ifndef _THREAD_H_
#define _THREAD_H_

#include <device.h>
#include <gpio.h>
#include "led.h"
#include "motor.h"
#include "buzzer.h"


#define SLEEP_PULSE 3000
#define STACKSIZE 1024
#define PRIORITY 1  

void motor_thread();

void led_thread();

void buzzer_thread();


#endif
