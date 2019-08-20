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

extern const k_tid_t leds_id;
extern const k_tid_t motor_id;
extern const k_tid_t buzzer_id;

void motor_thread();

void led_thread();

void buzzer_thread();


#endif
