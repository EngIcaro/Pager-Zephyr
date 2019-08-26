#include "thread.h"

#define WAITING 	0
#define CHARGING 	1
#define READY 		2

/* This thread is responsible for starting the alert cycle for state READY */
void alert_thread() {
	/* Setup */
	leds_configure();
	motors_configure();
	buzzer_configure();

	/* Alert Cycle (3 seconds) */
	while(1) {
		if(get_state() == READY) {
			set_led('0', 0);
			set_motor('0', 1);
			set_motor('1', 1);
			set_buzzer(1);
			k_sleep(K_MSEC(ALERT_PULSE));

			set_led('1', 0);
			k_sleep(K_MSEC(ALERT_PULSE));
			set_led('2', 0);
			k_sleep(K_MSEC(ALERT_PULSE));
			set_led('3', 0);
			k_sleep(K_MSEC(ALERT_PULSE));
			set_led('4', 0);
			k_sleep(K_MSEC(ALERT_PULSE));
			set_led('5', 0);
			k_sleep(K_MSEC(ALERT_PULSE));

			set_led('5', 1);
			set_motor('0', 0);
			set_motor('1', 0);
			set_buzzer(0);
			k_sleep(K_MSEC(ALERT_PULSE));

			set_led('4', 1);
			k_sleep(K_MSEC(ALERT_PULSE));
			set_led('3', 1);
			k_sleep(K_MSEC(ALERT_PULSE));
			set_led('2', 1);
			k_sleep(K_MSEC(ALERT_PULSE));
			set_led('1', 1);
			k_sleep(K_MSEC(ALERT_PULSE));
			set_led('0', 1);
			k_sleep(K_MSEC(ALERT_PULSE));
		}

		k_sleep(K_SECONDS(1));
	}
}

/* This thread is responsible for getting battery info and control the CHARGING state */
void battery_thread() {
	/* Setup */
	battery_configure();

	/* Battery Cycle (1 second) */
	while(1) {
		/* METER signal reads the current voltage of battery */
		// int percent = get_meter();
		/* PGOOD signal controls the charging state. */
		if(get_pgood()) {
			if(get_state() != CHARGING) {
				set_charging();
			}
		} else if(get_state() == CHARGING) {
			set_waiting();
		}

		/* CHR signal controls a led as visual aid. */
		// char led = '5';
		// if(get_chr()) {
		// 	set_led(led, 0);
		// } else if(get_state() != READY) {
		// 	set_led(led, 1);
		// }

		k_sleep(K_SECONDS(BATTERY_PULSE));
	}
}

// K_THREAD_DEFINE(leds_id, STACKSIZE, led_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
// K_THREAD_DEFINE(motor_id, STACKSIZE, motor_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
// K_THREAD_DEFINE(buzzer_id, STACKSIZE, buzzer_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
K_THREAD_DEFINE(alert_id, STACKSIZE, alert_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
K_THREAD_DEFINE(battery_id, STACKSIZE, battery_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);