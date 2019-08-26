#ifndef _BT_CALLBACKS_H
#define _BT_CALLBACKS_H

#include <bluetooth/bluetooth.h>
#include <bluetooth/mesh.h>
#include <misc/printk.h>
#include <settings/settings.h>
#include "bt_node.h"
#include "state_machine.h"

void gen_onoff_set(struct bt_mesh_model *model,
			       struct bt_mesh_msg_ctx *ctx,
			       struct net_buf_simple *buf);

void gen_onoff_set_unack(struct bt_mesh_model *model,
			             struct bt_mesh_msg_ctx *ctx,
			             struct net_buf_simple *buf);

void gen_onoff_get(struct bt_mesh_model *model,
			       struct bt_mesh_msg_ctx *ctx,
			       struct net_buf_simple *buf);

#endif