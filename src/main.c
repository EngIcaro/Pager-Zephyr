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
#include "thread.h"
#include "ic_logging.h"

// /* size of stack area used by each thread */
#define SLEEP_TIME 10000

/*
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
*/

void post(){
	
	printk("Running Post ...\n");
	leds_configure();
	motors_configure();
	buzzer_configure();
	
	printk("All right!\n");
}

// Para realizar o teste a Thread main vai dormir durante 10 segundos e quando acordar vai
// abortar as threads motor, leds e buzzer
void main(void)
{
	post();
	k_sleep(SLEEP_TIME);
	while (1) {
		k_sleep(SLEEP_TIME);
		k_thread_suspend(leds_id);
		k_thread_suspend(motor_id);
		k_thread_suspend(buzzer_id);
		k_sleep(SLEEP_TIME);
		k_thread_resume(leds_id);
		k_thread_resume(motor_id);
		k_thread_resume(buzzer_id);
	}
}
