#include <zephyr.h>
#include <zephyr/types.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/mesh.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/hci.h>
#include <settings/settings.h>
#include <misc/printk.h>
#include <misc/util.h>
#include <misc/byteorder.h>
#include <nrf.h>
#include <device.h>
#include <gpio.h>
#include <stdio.h>

/* Status */
#define ON 	0
#define OFF 1

/* Model Operation Codes */
#define BT_MESH_MODEL_OP_GEN_ONOFF_GET			BT_MESH_MODEL_OP_2(0x82, 0x01)
#define BT_MESH_MODEL_OP_GEN_ONOFF_SET			BT_MESH_MODEL_OP_2(0x82, 0x02)
#define BT_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK	BT_MESH_MODEL_OP_2(0x82, 0x03)
#define BT_MESH_MODEL_OP_GEN_ONOFF_STATUS		BT_MESH_MODEL_OP_2(0x82, 0x04)

/* Callbacks Headers */
static void gen_onoff_set(struct bt_mesh_model *model,
			  struct bt_mesh_msg_ctx *ctx,
			  struct net_buf_simple *buf);

static void gen_onoff_set_unack(struct bt_mesh_model *model,
			  struct bt_mesh_msg_ctx *ctx,
			  struct net_buf_simple *buf);

static void gen_onoff_get(struct bt_mesh_model *model,
			  struct bt_mesh_msg_ctx *ctx,
			  struct net_buf_simple *buf);

/* Server Configuration Declaration */
// duvida: ativar beacon?
static struct bt_mesh_cfg_srv cfg_srv = {
	.relay = BT_MESH_RELAY_ENABLED,
	.beacon = BT_MESH_BEACON_DISABLED,
	.frnd = BT_MESH_FRIEND_NOT_SUPPORTED,
	.gatt_proxy = BT_MESH_GATT_PROXY_ENABLED,
	.default_ttl = 7,
	.net_transmit = BT_MESH_TRANSMIT(1, 20),
	.relay_retransmit = BT_MESH_TRANSMIT(1, 20),
};

/* Health Server Declaration */
static struct bt_mesh_health_srv health_srv = {};

/* Client Configuration Declaration */
static struct bt_mesh_cfg_cli cfg_cli = {};

/* Publication Declarations */
BT_MESH_HEALTH_PUB_DEFINE(health_pub, 0);
BT_MESH_MODEL_PUB_DEFINE(gen_onoff_pub_srv, NULL, 2 + 2);

/* OnOff Model Server Op Dispatch Table */
static const struct bt_mesh_model_op gen_onoff_srv_op[] = {
	{BT_MESH_MODEL_OP_GEN_ONOFF_GET, 0, gen_onoff_get},
	{BT_MESH_MODEL_OP_GEN_ONOFF_SET, 2, gen_onoff_set},
	{BT_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK, 2, gen_onoff_set_unack},
	BT_MESH_MODEL_OP_END,
};

/* Element Context */
struct onoff_state {
	u8_t current;
	u8_t previous;
};

/* Declare and Initialize Element Context */
static struct onoff_state onoff_state = {
	.current = OFF,
	.previous = ON,
};

/* Element Model Declaration */
static struct bt_mesh_model root_models[] = {
	BT_MESH_MODEL_CFG_SRV(&cfg_srv),
	BT_MESH_MODEL_CFG_CLI(&cfg_cli),
	BT_MESH_MODEL_HEALTH_SRV(&health_srv, &health_pub),
	BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_ONOFF_SRV, gen_onoff_srv_op, &gen_onoff_pub_srv, &onoff_state),
};

/* Root Element Declaration */
static struct bt_mesh_elem elements[] = {
	BT_MESH_ELEM(0, root_models, BT_MESH_MODEL_NONE),
};

/* Node Declaration */
static const struct bt_mesh_comp comp = {
	.cid = BT_COMP_ID_LF,
	.elem = elements,
	.elem_count = ARRAY_SIZE(elements),
};

/*
 * Generic OnOff Model Server Message Handlers
 *
 * Mesh Model Specification 3.1.1
 *
 */
static void gen_onoff_get(struct bt_mesh_model *model,
			  struct bt_mesh_msg_ctx *ctx,
			  struct net_buf_simple *buf) {
	NET_BUF_SIMPLE_DEFINE(msg, 2 + 1 + 4);
	struct onoff_state *onoff_state = model->user_data;

	printk("addr 0x%04x onoff 0x%02x\n", bt_mesh_model_elem(model)->addr, onoff_state->current);
	bt_mesh_model_msg_init(&msg, BT_MESH_MODEL_OP_GEN_ONOFF_STATUS);
	net_buf_simple_add_u8(&msg, onoff_state->current);

	if (bt_mesh_model_send(model, ctx, &msg, NULL, NULL)) {
		printk("Unable to send On Off Status response\n");
	}
	// eh bom incluir um else com uma mensagem de log de sucesso
}

static void gen_onoff_set_unack(struct bt_mesh_model *model,
				struct bt_mesh_msg_ctx *ctx,
				struct net_buf_simple *buf) {
	struct net_buf_simple *msg = model->pub->msg;
	struct onoff_state *onoff_state = model->user_data;
	int err;

	onoff_state->current = net_buf_simple_pull_u8(buf);
	printk("addr 0x%02x state 0x%02x\n", bt_mesh_model_elem(model)->addr, onoff_state->current);

	/* Invocar tratamento de chamado do Pager aqui */

	/* Pin set low turns on LED's on the nrf52840-pca10056 board */
	// gpio_pin_write(onoff_state->led_device,
	// 	       onoff_state->led_gpio_pin,
	// 	       onoff_state->current ? 0 : 1);

	/*
	 * If a server has a publish address, it is required to
	 * publish status on a state change
	 *
	 * See Mesh Profile Specification 3.7.6.1.2
	 *
	 * Only publish if there is an assigned address
	 */

	// duvida: publish eh lido por quem?
	if (onoff_state->previous != onoff_state->current && model->pub->addr != BT_MESH_ADDR_UNASSIGNED) {
		printk("publish last 0x%02x cur 0x%02x\n", onoff_state->previous, onoff_state->current);
		onoff_state->previous = onoff_state->current;
		bt_mesh_model_msg_init(msg, BT_MESH_MODEL_OP_GEN_ONOFF_STATUS);
		net_buf_simple_add_u8(msg, onoff_state->current);
		err = bt_mesh_model_publish(model);
		if (err) {
			printk("bt_mesh_model_publish err %d\n", err);
		}
	}
}

static void gen_onoff_set(struct bt_mesh_model *model,
			  struct bt_mesh_msg_ctx *ctx,
			  struct net_buf_simple *buf) {
	printk("gen_onoff_set\n");

	gen_onoff_set_unack(model, ctx, buf);
	gen_onoff_get(model, ctx, buf);
}

/* Bluetooth Mesh Provisioning */
static u8_t dev_uuid[16] = { 0xdd, 0xdd };
static u16_t primary_addr;
static u16_t primary_net_idx;

static int output_number(bt_mesh_output_action_t action, u32_t number)
{
	printk("OOB Number %06u\n", number);
	return 0;
}

static int output_string(const char *str)
{
	printk("OOB String %s\n", str);
	return 0;
}

static void prov_complete(u16_t net_idx, u16_t addr)
{
	printk("provisioning complete for net_idx 0x%04x addr 0x%04x\n",
	       net_idx, addr);
	primary_addr = addr;
	primary_net_idx = net_idx;
}

static void prov_reset(void)
{
	bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);
}

static const struct bt_mesh_prov prov = {
	.uuid = dev_uuid,
#if 1
	.output_size = 6,
	.output_actions = (BT_MESH_DISPLAY_NUMBER | BT_MESH_DISPLAY_STRING),
	.output_number = output_number,
	.output_string = output_string,
#else
	.output_size = 0,
	.output_actions = 0,
	.output_number = 0,
#endif
	.complete = prov_complete,
	.reset = prov_reset,
};

/* Bluetooth Ready Callback */
static void bt_ready(int err) {
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");

	err = bt_mesh_init(&prov, &comp);
	if (err) {
		printk("Initializing mesh failed (err %d)\n", err);
		return;
	}

	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
	}

	///* Use identity address as device UUID */
	//// Disable OOB security for SILabs Android app
	//struct bt_le_oob oob;

	//if (bt_le_oob_get_local(BT_ID_DEFAULT, &oob)) {
	//	printk("Identity Address unavailable\n");
	//} else {
	//	memcpy(dev_uuid, oob.addr.a.val, 6);
	//}

	bt_mesh_prov_enable(BT_MESH_PROV_GATT | BT_MESH_PROV_ADV);

	printk("Mesh initialized\n");
}

// Teste
struct device *led_device;
void init_led(u8_t dev, const char *port, u32_t pin_num)
{
	led_device = device_get_binding(port);
	gpio_pin_configure(led_device,
			   pin_num, GPIO_DIR_OUT | GPIO_PUD_PULL_UP);
}

void main(void) {
	/* Initialize the Bluetooth Subsystem */
	int err;

	err = bt_enable(bt_ready);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
	}

	// Teste
	init_led(0, LED0_GPIO_CONTROLLER, LED0_GPIO_PIN);
	
	while(1) {
		if(onoff_state.current == 1) {
			gpio_pin_write(led_device, LED0_GPIO_PIN, 0);
			// k_sleep(1000);
		} else {
			gpio_pin_write(led_device, LED0_GPIO_PIN, 1);
			// k_sleep(1000);
		}
		k_sleep(500);
	}
}

// TODO: testar esse codigo junto do chamador (verificar se servidor ta recebendo as mensagens)