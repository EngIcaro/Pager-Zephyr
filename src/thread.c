#include "thread.h"

void led_thread() {
	//setup thread
	leds_configure();
	while(1) {
		//printk("thread led activate\n");
		call_leds();
		k_sleep(SLEEP_PULSE);
	}
}

void motor_thread() {
	//setup thread
	motors_configure();
	while(1) {
		//printk("thread motor activate\n");
		call_motors();
		k_sleep(SLEEP_PULSE);	
	}
}

void buzzer_thread() {
	//setup thread
	buzzer_configure();
	while(1) {
		//printk("thread buzzer activate\n");
		alarm();
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
		
		if(get_pgood()) {
			set_charging();
		}
		if(get_chr()) {
			//TODO: deixar algum LED aceso indicando carregamento
		}
		k_sleep(SLEEP_PULSE);
	}
}

K_THREAD_DEFINE(leds_id, STACKSIZE, led_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
K_THREAD_DEFINE(motor_id, STACKSIZE, motor_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
K_THREAD_DEFINE(buzzer_id, STACKSIZE, buzzer_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
K_THREAD_DEFINE(battery_id, STACKSIZE, battery_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);