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
#define SLEEP_TIME 6200

static int cmd_call_spin(const struct shell *shell, size_t argc, char **argv) {	
	shell_print(shell, "Spin Leds ...\n");
	call_leds();
	shell_print(shell, "Done!\n");
	return 0;
}

static int cmd_call_motor(const struct shell *shell, size_t argc, char **argv) {
	// Acessar: Array de Strings -> String na posição argc+1
	char motor = *(*(argv+1));
	set_motor();
	shell_print(shell, "Vibrating ...\n");
	shell_print(shell, "Done!\n");
	return 0;
}

static int cmd_call_buzzer(const struct shell *shell, size_t argc, char **argv) {	
	shell_print(shell, "Beaping ...\n");
	alarm();
	shell_print(shell, "Done!\n");
	return 0;
}

// SHELL
// Colocando os comandos criados como subcomandos do run
SHELL_STATIC_SUBCMD_SET_CREATE(sub_call,
    SHELL_CMD_ARG(spin, NULL, "Acione animaçao SPIN.", cmd_call_spin, 1, NULL),
    SHELL_CMD_ARG(motor, NULL, "Acione um Motor.", cmd_call_motor, 2, NULL),
    SHELL_CMD_ARG(buzzer, NULL, "Acione o buzzer.", cmd_call_motor, 2, NULL),
	SHELL_SUBCMD_SET_END
);

// Setando o comando call no root
SHELL_CMD_REGISTER(call, &sub_call, "Comandos do pager", NULL);

void post(){

	printk("\n... ... Running Post ... ...\n");

	printk("Leds Configure...\n");
	if(!leds_configure()){
		printk("Leds OK!\n");
	} else{ 
		printk("... ... Error on Leds configure ... ...\n");
		return;
	}
	set_all_leds(0);
	k_sleep(500);
	set_all_leds(1);
	
	printk("Motors Configure...\n");
	if(!motors_configure()){
		printk("Motors OK!\n");
	} else{ 
		printk("... ... Error on Motors configure ... ...\n");
		return;
	}
	call_motors();

	printk("Buzzer Configure...\n");
	if(!buzzer_configure()){
		printk("Buzzer OK!\n");
	} else{ 
		printk("... ... Error on Buzzer configure ... ...\n");
		return;
	}
	alarm();

	printk("... ... All right! ... ...\n");
}

// Para realizar o teste a Thread main vai dormir durante 10 segundos e quando acordar vai
// abortar as threads motor, leds e buzzer
void main(void)
{
	post();
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
