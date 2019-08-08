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

/***** SETUP BUTTONS ******/
/* change this to use another GPIO port */
#define PORT0	SW0_GPIO_CONTROLLER
/*#define PORT1	SW1_GPIO_CONTROLLER
#define PORT2	SW2_GPIO_CONTROLLER
#define PORT3	SW3_GPIO_CONTROLLER

/* change this to use another GPIO pin */
/*
#define PIN0     SW0_GPIO_PIN
#define PIN1     SW1_GPIO_PIN
#define PIN2     SW2_GPIO_PIN
#define PIN3     SW3_GPIO_PIN
*/
/* change to use another GPIO pin interrupt config */
#define EDGE    GPIO_INT_EDGE
#define PULL_UP GPIO_PUD_PULL_UP
#define DEBOUCE GPIO_INT_DEBOUNCE
#define SLEEP_TIME	3000



void main(void)
{
	motors_configure();
	while (1) {
		u32_t val = 0U;
		set_motor('0', 1);
		k_sleep(SLEEP_TIME);
		//set_motor('0', 1);
		//k_sleep(SLEEP_TIME);

	}
}
