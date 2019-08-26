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
#include "state_machine.h"

/* Shell commands */
static int cmd_call_spin(const struct shell *shell, size_t argc, char **argv) {	
	shell_print(shell, "[SHELL] Spining leds...\n");
	call_leds();
	shell_print(shell, "[SHELL] Done!\n");
	return 0;
}

static int cmd_call_leds(const struct shell *shell, size_t argc, char **argv) {	
	char led = *(*(argv+1));
	shell_print(shell, "[SHELL] Turning on all leds...\n");
	printk(" u8t: %d\n",(u8_t)led - 48);
	set_all_leds((u8_t)led - 48);
	shell_print(shell, "[SHELL] Done!\n");
	return 0;
}

static int cmd_call_led(const struct shell *shell, size_t argc, char **argv) {
	char led = *(*(argv+1));
	char status = *(*(argv+2));
	printk("status do call led %d\n", (int)status);
	if(status == '0') shell_print(shell, "[SHELL] Turning on led %c...\n", led);
	else shell_print(shell, "[SHELL] Turning off led %c...\n", led);
	set_led(led, (u8_t)status - 48);
	shell_print(shell, "[SHELL] Done!\n");
	return 0;
}

static int cmd_call_motor(const struct shell *shell, size_t argc, char **argv) {
	char motor = *(*(argv+1));
	char status = *(*(argv+2));
	if(status == '0') shell_print(shell, "[SHELL] Turning on motor %c...\n", motor);
	else shell_print(shell, "[SHELL] Turning off motor %c...\n", motor);
	set_motor(motor, (u8_t)status - 48);
	shell_print(shell, "[SHELL] Done!\n");
	return 0;
}

static int cmd_call_motors(const struct shell *shell, size_t argc, char **argv) {
	// Acessar: Array de Strings -> String na posição argc+1
	shell_print(shell, "[SHELL] Vibrating...\n");
	call_motors();
	shell_print(shell, "[SHELL] Done!\n");
	return 0;
}

static int cmd_call_buzzer(const struct shell *shell, size_t argc, char **argv) {	
	// char status = *(*(argv+1));
	shell_print(shell, "[SHELL] Beeping...\n");
	alarm();
	shell_print(shell, "[SHELL] Done!\n");
	return 0;
}

/* Shell declaration */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_call,
    SHELL_CMD_ARG(spin, NULL, "Acione animaçao SPIN.", cmd_call_spin, 1, NULL),
	SHELL_CMD_ARG(led, NULL, "Acione um Led.", cmd_call_led, 3, NULL),
    SHELL_CMD_ARG(leds, NULL, "Acione todos os Leds.", cmd_call_leds, 2, NULL),
    SHELL_CMD_ARG(motor, NULL, "Acione um Motor.", cmd_call_motor, 3, NULL),
    SHELL_CMD_ARG(motors, NULL, "Acione os Motores.", cmd_call_motors, 1, NULL),
    SHELL_CMD_ARG(buzzer, NULL, "Acione o buzzer.", cmd_call_buzzer, 1, NULL),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(call, &sub_call, "Comandos do pager", NULL);

/* Power-On Self-Test (POST) */
void post() {

	printk("[POST] Running POST...\n");
	k_sleep(K_MSEC(1500));

	if(!leds_configure()){
		printk("[POST] Configuring leds... OK!\n");
	} else{ 
		printk("[POST] Configuring leds... ERROR!\n");
		return;
	}
	set_all_leds(0);
	k_sleep(K_MSEC(1000));
	set_all_leds(1);
	
	if(!motors_configure()){
		printk("[POST] Configuring motors... OK!\n");
	} else{ 
		printk("[POST] Configuring motors... ERROR!\n");
		return;
	}

	set_motor('0', 1);
	set_motor('1', 1);
	k_sleep(K_MSEC(1000));
	set_motor('0', 0);
	set_motor('1', 0);

	if(!buzzer_configure()){
		printk("[POST] Configuring buzzer... OK!\n");
	} else{ 
		printk("[POST] Configuring buzzer... ERROR!\n");
		return;
	}

	set_buzzer(1);
	k_sleep(K_MSEC(1000));
	set_buzzer(0);

	printk("[POST] Everything seems right. PAU NA MAQUINA!\n");
	k_sleep(K_MSEC(1500));
}

/* System Main Loop */
void main(void) {
	k_sleep(K_SECONDS(1));

	/* Execute POST */
	post();

	printk("[SYSTEM] Initializing Pager...\n");
	
	/* Initialize the Bluetooth Subsystem */
	int err;
	err = bt_enable(bt_ready);
	if (err) {
		printk("[BLUETOOTH] Bluetooth init failed with err %d.\n", err);
	}

	while (1) {
		state_machine();
		k_sleep(K_SECONDS(1));
	}
}
