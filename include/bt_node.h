#ifndef _BT_NODE_H
#define _BT_NODE_H

#include <bluetooth/bluetooth.h>
#include <bluetooth/mesh.h>
#include <settings/settings.h>
#include "bt_callbacks.h"

/* Model Operation Codes */
#define BT_MESH_MODEL_OP_GEN_ONOFF_GET			BT_MESH_MODEL_OP_2(0x82, 0x01)
#define BT_MESH_MODEL_OP_GEN_ONOFF_SET			BT_MESH_MODEL_OP_2(0x82, 0x02)
#define BT_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK	BT_MESH_MODEL_OP_2(0x82, 0x03)
#define BT_MESH_MODEL_OP_GEN_ONOFF_STATUS		BT_MESH_MODEL_OP_2(0x82, 0x04)

/* Element Status */
#define ON 	1
#define OFF 0

/* Element Context */
struct onoff_state {
	u8_t current;
	u8_t previous;
};

extern struct onoff_state onoff_state;
extern const struct bt_mesh_comp comp;

#endif