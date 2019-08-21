#include "bt_callbacks.h"

/* Generic OnOff Model Client Message Handlers */
/* Mesh Model Specification 3.1.1 */
void gen_onoff_status(struct bt_mesh_model *model,
			          struct bt_mesh_msg_ctx *ctx,
			          struct net_buf_simple *buf) {
	u8_t state;

	state = net_buf_simple_pull_u8(buf);

	// printk("Node 0x%04x OnOff status from 0x%04x with state 0x%02x\n",
	//        bt_mesh_model_elem(model)->addr, ctx->addr, state);
	printk("[BLUETOOTH] Node 0x%04x from 0x%04x has state 0x%02x.\n",
	       bt_mesh_model_elem(model)->addr, ctx->addr, state);
}