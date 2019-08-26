#ifndef _BT_MESH_H
#define _BT_MESH_H

#include <bluetooth/bluetooth.h>
#include <bluetooth/mesh.h>
#include <misc/printk.h>
#include <settings/settings.h>

#include "bt_callbacks.h"
#include "bt_node.h"

int output_number(bt_mesh_output_action_t action, u32_t number);

int output_string(const char *str);

void prov_complete(u16_t net_idx, u16_t addr);

void prov_reset(void);

void bt_ready(int err);

#endif