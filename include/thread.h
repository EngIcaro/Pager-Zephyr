#ifndef _THREAD_H_
#define _THREAD_H_

#include <device.h>
#include <gpio.h>
#include "led.h"
#include "motor.h"
#include "buzzer.h"
#include "battery.h"
#include "state_machine.h"

#define ALERT_PULSE 	250		// mseconds
#define BATTERY_PULSE 	1		// seconds
#define STACKSIZE 		1024
#define PRIORITY 		1

// extern const k_tid_t leds_id;
// extern const k_tid_t motor_id;
// extern const k_tid_t buzzer_id;
extern const k_tid_t alert_id;
extern const k_tid_t battery_id;

// void motor_thread();

// void led_thread();

// void buzzer_thread();

void alert_thread();

void battery_thread();

#endif