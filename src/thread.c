#include "thread.h"

#define WAITING 	0
#define CHARGING 	1
#define READY 		2

void led_thread() {
	//setup thread
	leds_configure();
	while(1) {
		if(get_state() == READY) {
			call_leds();
		} else {
			set_all_leds(1);
		}
		//printk("thread led activate\n");
		// call_leds();
		k_sleep(SLEEP_PULSE);
	}
}

void motor_thread() {
	//setup thread
	motors_configure();
	while(1) {
		if(get_state() == READY) {
			call_motors();
		} else {
			set_motor('0', 0);
			set_motor('1', 0);
		}
		//printk("thread motor activate\n");
		// call_motors();
		k_sleep(SLEEP_PULSE);
	}
}

void buzzer_thread() {
	//setup thread
	buzzer_configure();
	while(1) {
		if(get_state() == READY) {
			alarm();
		} else {
			set_buzzer(0);
		}
		//printk("thread buzzer activate\n");
		// alarm();
		k_sleep(SLEEP_PULSE);
	}
}

void battery_thread() {
	//setup thread
	battery_configure();
	while(1) {
		//printk("thread battery activate\n");
		// int percent = get_meter();
		//TODO: implementar envio de porcentagem para o chamador via bluetooth

		/* PGOOD signal controls the charging state. */
		if(get_pgood()) {
			if(get_state() != CHARGING) {
				set_charging();
				printk("charging da bateria\n");
			}
		} else if(get_state() == CHARGING) {
			set_waiting();
			printk("waiting da bateria\n");
		}

		/* CHR signal controls the leds as visual aid. */
		// printk("ari\n");
		// char led = '5';
		// if(get_chr()) {
		// 	printk("gato\n");
		// 	set_led(led, 0);
		// 	printk("carregando\n");
		// } else if(get_state() != READY) {
		// 	printk("gato2\n");
		// 	set_led(led, 1);
		// 	printk("descarregando\n");
		// }
		// printk("arigato\n");
		k_sleep(SLEEP_PULSE/3);
	}
}

K_THREAD_DEFINE(leds_id, STACKSIZE, led_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
K_THREAD_DEFINE(motor_id, STACKSIZE, motor_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
K_THREAD_DEFINE(buzzer_id, STACKSIZE, buzzer_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
K_THREAD_DEFINE(battery_id, STACKSIZE, battery_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);