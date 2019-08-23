#include "state_machine.h"

static enum {WAITING, CHARGING, READY} state = WAITING;

struct data_item_type{
    u32_t data1;       // BT ON/OFF
    u32_t data2;       // PAGER DEVOLVIDO
    u32_t data3;	   // CARRREGANDO
};

K_MSGQ_DEFINE(my_msgq, sizeof(struct data_item_type), 10, 4);

void state_machine()
{
	printk(" *** State Machine \n");
	printk("estado atual: %d\n", state);
    struct data_item_type package = { 0 };	
    k_msgq_get(&my_msgq, &package, K_FOREVER);

    switch (state) {
        case WAITING:
            printk(" *** State --- WAITING \n");
            // pedido esta pronto. BT ON
            if (package.data1) {
				state = READY;
                // k_thread_resume(leds_id);
		        // k_thread_resume(motor_id);
		        // k_thread_resume(buzzer_id);
				printk("waiting -> ready\n");
            }
			// Pager foi colocado para carregar
            else if(package.data3){
				state = CHARGING;
				printk("waiting -> charging\n");
            }
			// Continue esperando
			else{
				state = WAITING;
				printk("waiting -> waiting\n");
			}
            break;
        case READY:
            printk(" *** State --- READY\n");
            // Se o pager for devolvido
            if (package.data2){
				state = WAITING;
                // k_thread_suspend(leds_id);
		        // k_thread_suspend(motor_id);
	        	// k_thread_suspend(buzzer_id);
				printk("ready -> waiting\n");
            }
			// Se o pager for colocado para carregar
			else if(package.data3){
				state = CHARGING;
                // k_thread_suspend(leds_id);
		        // k_thread_suspend(motor_id);
	        	// k_thread_suspend(buzzer_id);
				printk("ready -> charging, bt off\n");
				onoff_state.current = OFF;
            }
			// Se ele ainda não for devolvido
            else{
				state = READY;
				printk("ready -> ready\n");
            }
            break;
        case CHARGING:
            printk(" *** State --- CHARGING \n");
            // se ele estiver não estiver carregando
            if (package.data2) {
                state = WAITING;
                // k_thread_suspend(leds_id);
		        // k_thread_suspend(motor_id);
	        	// k_thread_suspend(buzzer_id);
				printk("charging -> waiting\n");
            }
            // se ele continuar carregando.
            else{
                // k_thread_suspend(leds_id);
		        // k_thread_suspend(motor_id);
	        	// k_thread_suspend(buzzer_id);
				printk("charging -> charging\n");
            }
            break;
        default:
			printk("default\n");
            break;
    }
}

void set_ready(){
    struct data_item_type package;
	package.data1 = 1;
	package.data2 = 0;
	package.data3 = 0;
	/* send data to consumers */
	while (k_msgq_put(&my_msgq, &package, K_NO_WAIT) != 0) {
		/* message queue is full: purge old data & try again */
		k_msgq_purge(&my_msgq);
	}
}

void set_waiting(){
    struct data_item_type package;
	package.data1 = 0;
	package.data2 = 1;
	package.data3 = 0;
	/* send data to consumers */
	while (k_msgq_put(&my_msgq, &package, K_NO_WAIT) != 0) {
		/* message queue is full: purge old data & try again */
		k_msgq_purge(&my_msgq);
	}
}

void set_charging(){
    struct data_item_type package;
	package.data1 = 0;
	package.data2 = 0;
	package.data3 = 1;
	/* send data to consumers */
	while (k_msgq_put(&my_msgq, &package, K_NO_WAIT) != 0) {
		/* message queue is full: purge old data & try again */
		k_msgq_purge(&my_msgq);
	}
}

int get_state() {
	return state;
}