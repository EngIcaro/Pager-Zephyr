#include <zephyr.h>
#include <device.h>
#include <gpio.h>
#include <misc/printk.h>
#include <shell/shell.h>
#include <stdlib.h>

#include "input.h"
#include "output.h"
#include "led.h"
#include "motor.h"
#include "buzzer.h"


/* size of stack area used by each thread */
#define STACKSIZE 1024
#define PRIORITY 2

#define SLEEP_TIME	3000

// função da thread leds_id
void led_function(){
	leds_configure();
	call_leds();
}
// função da thread motor_id
void motor_function(){
	motors_configure();
	call_motors();
}
// AO UTILIZAR K_THREAD_DEFINE É DEFINIDO UMA ÁREA NA PILHA E SEU BLOCO  DE CONTROLE
// PARAMETROS: (THREAD ID, STACK SIZE, FUNÇÃO DE CHAMADA, P1, P2, P3, PRIORIDADE, OPT, DELAY)
K_THREAD_DEFINE(leds_id, STACKSIZE, led_function, NULL, NULL, NULL, PRIORITY, 0,
		K_NO_WAIT);
K_THREAD_DEFINE(motor_id, STACKSIZE, motor_function, NULL, NULL, NULL, PRIORITY, 0,
		K_NO_WAIT);


void main(void)
{
	printk("passei hanney");
	while (1) {
		printk("passei hanney");
		u32_t val = 0U;
	}
}
