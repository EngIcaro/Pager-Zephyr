#include "thread.h"


K_THREAD_DEFINE(leds_id, STACKSIZE, led_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
K_THREAD_DEFINE(motor_id, STACKSIZE, motor_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
K_THREAD_DEFINE(buzzer_id, STACKSIZE, buzzer_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);

void led_thread(){
	//setup thread
	 leds_configure();

		while(1){
			call_leds();
			k_sleep(SLEEP_PULSE);
	}
}

void motor_thread(){
	//setup thread
	motors_configure();

	while(1){
		call_motors();
		k_sleep(SLEEP_PULSE);
	}

}

void buzzer_thread(){
	//setup thread
	buzzer_configure();
		while(1){
			alarm();
			k_sleep(SLEEP_PULSE);
		}
}
