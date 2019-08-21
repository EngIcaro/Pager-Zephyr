#include <zephyr.h>
#include <misc/printk.h>
#include <misc/util.h>
#include <settings/settings.h>
#include <stdio.h>

#include <gpio.h>

#include "bt_callbacks.h"
#include "bt_node.h"
#include "bt_mesh.h"

/**************** TESTE ***************/
struct device *led_device;
void init_led(u8_t dev, const char *port, u32_t pin_num)
{
	led_device = device_get_binding(port);
	gpio_pin_configure(led_device,
			   pin_num, GPIO_DIR_OUT | GPIO_PUD_PULL_UP);
}
/**************** TESTE ***************/

void main(void) {
	int err;

	printk("[SYSTEM] Initializing Pager...\n");

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(bt_ready);
	if (err) {
		printk("[BLUETOOTH] Bluetooth init failed with err %d.\n", err);
	}

	/**************** TESTE ***************/
	init_led(0, LED0_GPIO_CONTROLLER, LED0_GPIO_PIN);
	
	while(1) {
		if(onoff_state.current == 1) {
			gpio_pin_write(led_device, LED0_GPIO_PIN, 0);
			// k_sleep(1000);
		} else {
			gpio_pin_write(led_device, LED0_GPIO_PIN, 1);
			// k_sleep(1000);
		}
		k_sleep(500);
	}
	/**************** TESTE ***************/
}

// TODO: testar esse codigo junto do chamador (verificar se servidor ta recebendo as mensagens)