#include <zephyr.h>
#include <device.h>
#include <gpio.h>
#include <misc/printk.h>
#include <shell/shell.h>
#include <stdlib.h>

#include "input.h"
#include "output.h"
#include "led.h"
#include "motor.h"


/* size of stack area used by each thread */
#define STACKSIZE 1024
#define PRIORITY 2

#define SLEEP_TIME	3000


void led_function(){
	leds_configure();
	call_leds();
}

void motor_function(){
	motors_configure();
	call_motors();
}

K_THREAD_DEFINE(leds_id, STACKSIZE, led_function, NULL, NULL, NULL, PRIORITY, 0,
		K_NO_WAIT);
K_THREAD_DEFINE(motor_id, STACKSIZE, motor_function, NULL, NULL, NULL, PRIORITY, 0,
		K_NO_WAIT);


void main(void)
{
	while (1) {
		u32_t val = 0U;
	}
}
