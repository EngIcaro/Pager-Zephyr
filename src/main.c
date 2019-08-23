#include <zephyr.h>
#include <device.h>
#include <gpio.h>
#include <misc/printk.h>
#include <misc/util.h>
#include <settings/settings.h>
#include <shell/shell.h>
#include <stdlib.h>

#include "bt_callbacks.h"
#include "bt_node.h"
#include "bt_mesh.h"
#include "input.h"
#include "output.h"
#include "led.h"
#include "motor.h"
#include "buzzer.h"
#include "battery.h"
#include "thread.h"
#include "ic_logging.h"
#include "state_machine.h"

/* Size of stack area used by each thread */
#define SLEEP_TIME 6200

static int cmd_call_spin(const struct shell *shell, size_t argc, char **argv) {	
	shell_print(shell, "Spin Leds ...\n");
	call_leds();
	shell_print(shell, "Done!\n");
	return 0;
}

static int cmd_call_leds(const struct shell *shell, size_t argc, char **argv) {	
	shell_print(shell, "Turn on all leds ...\n");
	set_all_leds(0);
	shell_print(shell, "Done!\n");
	return 0;
}

static int cmd_call_led(const struct shell *shell, size_t argc, char **argv) {
	char led = *(*(argv+1));
	if(led == '0') shell_print(shell, "Turn on led %c...\n", led);
	else shell_print(shell, "Turn off led %c...\n", led);
	set_led(led, 0);
	shell_print(shell, "Done!\n");
	return 0;
}

static int cmd_call_motor(const struct shell *shell, size_t argc, char **argv) {
	char motor = *(*(argv+1));
	if(motor == '0') shell_print(shell, "Turn on motor %c...\n", motor);
	else shell_print(shell, "Turn off motor %c...\n", motor);
	set_motor(motor, 0);
	shell_print(shell, "Done!\n");
	return 0;
}

static int cmd_call_motors(const struct shell *shell, size_t argc, char **argv) {
	// Acessar: Array de Strings -> String na posição argc+1
	shell_print(shell, "Vibrating ...\n");
	call_motors();
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
	SHELL_CMD_ARG(led, NULL, "Acione um Led.", cmd_call_led, 2, NULL),
    SHELL_CMD_ARG(leds, NULL, "Acione todos os Leds.", cmd_call_leds, 1, NULL),
    SHELL_CMD_ARG(motor, NULL, "Acione um Motor.", cmd_call_motor, 2, NULL),
    SHELL_CMD_ARG(motors, NULL, "Acione os Motores.", cmd_call_motors, 1, NULL),
    SHELL_CMD_ARG(buzzer, NULL, "Acione o buzzer.", cmd_call_buzzer, 1, NULL),
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
	//post();

	int err;

	printk("[SYSTEM] Initializing Pager...\n");

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(bt_ready);
	if (err) {
		printk("[BLUETOOTH] Bluetooth init failed with err %d.\n", err);
	}

	k_thread_suspend(leds_id);
	k_thread_suspend(motor_id);
	k_thread_suspend(buzzer_id);
	while (1) {
		// set_waiting();
		// state_machine();
		// k_sleep(K_SECONDS(4));
		// set_ready();
		// state_machine();
		// k_sleep(K_SECONDS(4));
		// set_charging();
		state_machine();
		k_sleep(K_SECONDS(1));
		/*k_sleep(SLEEP_TIME);

		k_sleep(SLEEP_TIME);
		*/
	}
}
