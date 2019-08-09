#include <zephyr.h>
#include <device.h>
#include <gpio.h>
#include <misc/util.h>
#include <misc/printk.h>
#include <shell/shell.h>
#include <stdlib.h>

#include "input.h"
#include "output.h"
#include "led.h"
#include "motor.h"

// /* size of stack area used by each thread */
#define STACKSIZE 1024
#define PRIORITY 2

#define SLEEP_TIME 500

void led_function(){
		leds_configure();
		call_leds();
	}

void motor_function(){
	motors_configure();
	call_motors();
}
/*
K_THREAD_DEFINE(leds_id, STACKSIZE, led_function, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
K_THREAD_DEFINE(motor_id, STACKSIZE, motor_function, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
*/
static int cmd_call_spin(const struct shell *shell, size_t argc, char **argv) {
		shell_print(shell, "Spin Leds ...\n");

		led_function();
		call_leds();

		shell_print(shell, "Done!\n");
		return 0;
}

// SHELL
// Colocando os comandos criados como subcomandos do run
SHELL_STATIC_SUBCMD_SET_CREATE(sub_call,
    SHELL_CMD_ARG(spin, NULL, "Acione animaçao SPIN.", cmd_call_spin, 1, NULL),
    //SHELL_CMD_ARG(motor, NULL, "Acione o Motor de vibracao.", cmd_call_motor, 2, NULL),
    SHELL_SUBCMD_SET_END
);

// Setando o comando call no root
SHELL_CMD_REGISTER(call, &sub_call, "Comandos do pager", NULL);

void main(void)
{
	printk("One piece é foda!\n");
	while (1) {
		u32_t val = 0U;
		k_sleep(SLEEP_TIME);
	}
}
