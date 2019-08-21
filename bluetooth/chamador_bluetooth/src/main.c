#include <zephyr.h>
#include <misc/printk.h>
#include <misc/util.h>
#include <settings/settings.h>
#include <stdio.h>

#include "bt_callbacks.h"
#include "bt_node.h"
#include "bt_mesh.h"
#include "button.h"

void main(void) {
	int err;

	printk("[SYSTEM] Initializing Caller...\n");

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(bt_ready);
	if (err) {
		printk("[BLUETOOTH] Bluetooth init failed with err %d.\n", err);
	}
	
	button_init();

	/**************** TESTE ***************/
	// init_led(0, LED0_GPIO_CONTROLLER, LED0_GPIO_PIN);
	
	//while(1) {
	//	gpio_pin_write(led_device, LED0_GPIO_PIN, 1);
	//	k_sleep(1000);
	//	gpio_pin_write(led_device, LED0_GPIO_PIN, 0);
	//	k_sleep(1000);
	//}
	/**************** TESTE ***************/
}

// TODO: implementar get para saber status do servidor sem precisar dar set