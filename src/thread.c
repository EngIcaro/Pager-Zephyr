#include "thread.h"

void led_thread(){
	//setup thread
	leds_configure();

		while(1){
			call_leds();
			//printk("\nThread Led activate.\n");
			k_sleep(SLEEP_PULSE);
	}
}

void motor_thread(){
	//setup thread
	motors_configure();

	while(1){
		call_motors();
		//printk("\nThread Motor activate.\n");
		k_sleep(SLEEP_PULSE);
	}

}

void buzzer_thread(){
	//setup thread
	buzzer_configure();
		while(1){
			//printk("\nThread Buzzer activate.\n");
			alarm();
			k_sleep(SLEEP_PULSE);
		}
}
K_THREAD_DEFINE(leds_id, STACKSIZE, led_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
K_THREAD_DEFINE(motor_id, STACKSIZE, motor_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
K_THREAD_DEFINE(buzzer_id, STACKSIZE, buzzer_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
