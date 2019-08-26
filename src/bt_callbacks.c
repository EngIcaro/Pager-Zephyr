#include "bt_callbacks.h"

#define WAITING 	0
#define CHARGING 	1
#define READY 		2

/* Generic OnOff Model Client Message Handlers */
/* Mesh Model Specification 3.1.1 */
void gen_onoff_get(struct bt_mesh_model *model,
			       struct bt_mesh_msg_ctx *ctx,
			       struct net_buf_simple *buf) {
	NET_BUF_SIMPLE_DEFINE(msg, 2 + 1 + 4);
	struct onoff_state *onoff_state = model->user_data;
	printk("[BLUETOOTH] Node 0x%04x has state 0x%02x.\n",
		   bt_mesh_model_elem(model)->addr, onoff_state->current);
	bt_mesh_model_msg_init(&msg, BT_MESH_MODEL_OP_GEN_ONOFF_STATUS);
	net_buf_simple_add_u8(&msg, onoff_state->current);

	if (bt_mesh_model_send(model, ctx, &msg, NULL, NULL)) {
		printk("[BLUETOOTH] Unable to send OnOff Status response.\n");
	}
}

void gen_onoff_set_unack(struct bt_mesh_model *model,
				         struct bt_mesh_msg_ctx *ctx,
				         struct net_buf_simple *buf) {
	u8_t call;
	call = net_buf_simple_pull_u8(buf);

	if(call == ON && get_state() == WAITING) {
		set_ready();
	} else if(call == OFF && get_state() == READY) {
		set_waiting();
	} else {
		return;
	}

	struct net_buf_simple *msg = model->pub->msg;
	struct onoff_state *onoff_state = model->user_data;
	int err;
	
	onoff_state->current = call;
	printk("[BLUETOOTH] Node 0x%02x was set to state 0x%02x.\n",
		bt_mesh_model_elem(model)->addr, onoff_state->current);

	/*
	* If a server has a publish address, it is required to
	* publish status on a state change
	*
	* See Mesh Profile Specification 3.7.6.1.2
	*/

	/* Only publish if there is an assigned address */
	if (onoff_state->previous != onoff_state->current && model->pub->addr != BT_MESH_ADDR_UNASSIGNED) {
		printk("[BLUETOOTH] Node last state was 0x%02x, current state is 0x%02x.\n",
			onoff_state->previous, onoff_state->current);
		onoff_state->previous = onoff_state->current;
		bt_mesh_model_msg_init(msg, BT_MESH_MODEL_OP_GEN_ONOFF_STATUS);
		net_buf_simple_add_u8(msg, onoff_state->current);
		err = bt_mesh_model_publish(model);
		if (err) {
			printk("[BLUETOOTH] Publishing failed with err %d.\n", err);
		}
	}
}

void gen_onoff_set(struct bt_mesh_model *model,
			       struct bt_mesh_msg_ctx *ctx,
			       struct net_buf_simple *buf) {
	gen_onoff_set_unack(model, ctx, buf);
	gen_onoff_get(model, ctx, buf);
}