#include "bt_mesh.h"

/* Bluetooth Mesh Provisioning */
static u8_t dev_uuid[16] = { 0xdd, 0xdd };
u16_t primary_addr;
u16_t primary_net_idx;

int output_number(bt_mesh_output_action_t action, u32_t number) {
	printk("[BLUETOOTH] OOB Number %06u.\n", number);
	return 0;
}

int output_string(const char *str) {
	printk("[BLUETOOTH] OOB String %s.\n", str);
	return 0;
}

void prov_complete(u16_t net_idx, u16_t addr) {
	printk("[BLUETOOTH] Provisioning complete for net_idx 0x%04x, address 0x%04x.\n", net_idx, addr);
	primary_addr    = addr;
	primary_net_idx = net_idx;
}

void prov_reset(void) {
	bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);
	printk("[BLUETOOTH] Provisioning reset.\n");
}

const struct bt_mesh_prov prov = {
	.uuid           = dev_uuid,
	.output_size    = 6,
	.output_actions = (BT_MESH_DISPLAY_NUMBER | BT_MESH_DISPLAY_STRING),
	.output_number  = output_number,
	.output_string  = output_string,
	.complete       = prov_complete,
	.reset          = prov_reset,
};

/* Bluetooth Ready Callback */
void bt_ready(int err) {
	if (err) {
		printk("[BLUETOOTH] Bluetooth init failed with err %d.\n", err);
		return;
	}

	printk("[BLUETOOTH] Bluetooth initialized.\n");

	err = bt_mesh_init(&prov, &comp);
	if (err) {
		printk("[BLUETOOTH] Initializing mesh failed with err %d.\n", err);
		return;
	}

	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
	}

	bt_mesh_prov_enable(BT_MESH_PROV_GATT | BT_MESH_PROV_ADV);

	printk("[BLUETOOTH] Mesh initialized.\n");
}