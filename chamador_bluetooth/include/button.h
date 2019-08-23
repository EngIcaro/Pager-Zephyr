#ifndef _BUTTON_H
#define _BUTTON_H

#include <gpio.h>
#include <misc/printk.h>
#include <settings/settings.h>

#include "bt_callbacks.h"
#include "bt_node.h"
#include "bt_mesh.h"

#define BUTTON_0_CONTROLLER 	SW0_GPIO_CONTROLLER
#define BUTTON_0_PIN 			SW0_GPIO_PIN

#define FLAGS 					(GPIO_DIR_IN | GPIO_INT | GPIO_INT_EDGE | GPIO_INT_ACTIVE_LOW | GPIO_PUD_PULL_UP)
#define BUTTON_PRESS_INTERVAL_SEC 	1
#define BUTTON_DEBOUNCE_DELAY_MS 	250

struct sw {
	u8_t onoff_state;
	struct k_work button_work;
	struct k_timer button_timer;
};

void button_pressed(struct device *dev, struct gpio_callback *cb, u32_t pin_pos);

void button_cnt_timer(struct k_timer *work);

void button_pressed_worker(struct k_work *work);

void button_init();

#endif