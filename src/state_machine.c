#include "state_machine.h"

/* Possible States */
static enum {WAITING, CHARGING, READY} state = WAITING;

/* Message struct */
struct data_item_type {
    u32_t data1;       // READY message
    u32_t data2;       // WAITING message
    u32_t data3;	   // CHARGING message
};

K_MSGQ_DEFINE(my_msgq, sizeof(struct data_item_type), 10, 4);

/* State Machine */
void state_machine() {
	/* Reading message queue for state update */
    struct data_item_type package = { 0 };	
    k_msgq_get(&my_msgq, &package, K_FOREVER);

	/* State Update */
    switch (state) {
        case WAITING:
            printk("[STATE MACHINE] Actual state: WAITING.\n");
            if (package.data1) {
				state = READY;
				printk("[STATE CHANGE] WAITING -> READY\n");
            } else if(package.data3) {
				state = CHARGING;
				printk("[STATE CHANGE] WAITING -> CHARGING\n");
            } else {
				state = WAITING;
				printk("waiting -> waiting\n");
			}
            break;
        case READY:
            printk("[STATE MACHINE] Actual state: READY.\n");
            if (package.data2) {
				state = WAITING;
				printk("[STATE CHANGE] READY -> WAITING\n");
            } else if(package.data3) {
				state = CHARGING;
				printk("[STATE CHANGE] READY -> CHARGING\n");
				onoff_state.current = OFF;	//update mesh node state
            } else {
				state = READY;
				printk("ready -> ready\n");
            }
            break;
        case CHARGING:
            printk("[STATE MACHINE] Actual state: CHARGING.\n");
            if (package.data2) {
                state = WAITING;
				printk("[STATE CHANGE] CHARGING -> WAITING\n");
            } else {
				printk("charging -> charging\n");
            }
            break;
        default:
			printk("default\n");
            break;
    }
}

/* Send READY message to consumers */
void set_ready(){
    struct data_item_type package;
	package.data1 = 1;
	package.data2 = 0;
	package.data3 = 0;

	while (k_msgq_put(&my_msgq, &package, K_NO_WAIT) != 0) {
		/* Message queue is full: purge old data & try again */
		k_msgq_purge(&my_msgq);
	}
}

/* Send WAITING message to consumers */
void set_waiting(){
    struct data_item_type package;
	package.data1 = 0;
	package.data2 = 1;
	package.data3 = 0;

	while (k_msgq_put(&my_msgq, &package, K_NO_WAIT) != 0) {
		/* Message queue is full: purge old data & try again */
		k_msgq_purge(&my_msgq);
	}
}

/* Send CHARGING message to consumers */
void set_charging(){
    struct data_item_type package;
	package.data1 = 0;
	package.data2 = 0;
	package.data3 = 1;
	
	while (k_msgq_put(&my_msgq, &package, K_NO_WAIT) != 0) {
		/* Message queue is full: purge old data & try again */
		k_msgq_purge(&my_msgq);
	}
}

/* Return actual state */
int get_state() {
	return state;
}