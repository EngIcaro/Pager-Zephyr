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
#define BT_MESH_MODEL_OP_GEN_ONOFF_GET		BT_MESH_MODEL_OP_2(0x82, 0x01)
#define BT_MESH_MODEL_OP_GEN_ONOFF_SET		BT_MESH_MODEL_OP_2(0x82, 0x02)
#define BT_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK	BT_MESH_MODEL_OP_2(0x82, 0x03)
#define BT_MESH_MODEL_OP_GEN_ONOFF_STATUS	BT_MESH_MODEL_OP_2(0x82, 0x04)

#define BUTTON_DEBOUNCE_DELAY_MS 250

/* Callbacks Headers */
static void gen_onoff_status(struct bt_mesh_model *model,
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
BT_MESH_MODEL_PUB_DEFINE(gen_onoff_pub_cli, NULL, 2 + 2);

/* OnOff Model Client Op Dispatch Table */
static const struct bt_mesh_model_op gen_onoff_cli_op[] = {
	{ BT_MESH_MODEL_OP_GEN_ONOFF_STATUS, 1, gen_onoff_status },
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

/* Element Model Declarations */
static struct bt_mesh_model root_models[] = {
	BT_MESH_MODEL_CFG_SRV(&cfg_srv),
	BT_MESH_MODEL_CFG_CLI(&cfg_cli),
	BT_MESH_MODEL_HEALTH_SRV(&health_srv, &health_pub),
	BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_ONOFF_CLI, gen_onoff_cli_op,
		      &gen_onoff_pub_cli, &onoff_state),
};

/* Button to Client Model Assignments */
struct bt_mesh_model *mod_cli_sw[] = {
		&root_models[3],
};

// /* Pager to Server Model Assigmnents */
// struct bt_mesh_model *mod_srv_sw[] = {
// 		&root_models[3],
// };

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

struct device *sw_device;

struct sw {
	u8_t sw_num;
	u8_t onoff_state;
	struct k_work button_work;
	struct k_timer button_timer;
};

static u8_t button_press_cnt;
static struct sw sw;

static struct gpio_callback button_cb;

static u8_t trans_id;
static u32_t time, last_time;


/*
 * Generic OnOff Model Client Message Handlers
 *
 * Mesh Model Specification 3.1.1
 *
 */
static void gen_onoff_status(struct bt_mesh_model *model,
			     struct bt_mesh_msg_ctx *ctx,
			     struct net_buf_simple *buf)
{
	u8_t	state;

	state = net_buf_simple_pull_u8(buf);

	printk("Node 0x%04x OnOff status from 0x%04x with state 0x%02x\n",
	       bt_mesh_model_elem(model)->addr, ctx->addr, state);
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

static uint8_t pin_to_sw(uint32_t pin_pos)
{
	switch (pin_pos) {
	case BIT(SW0_GPIO_PIN): return 0;
	}

	printk("No match for GPIO pin 0x%08x\n", pin_pos);
	return 0;
}

/* Button pressed Event Handler */
static void button_pressed(struct device *dev, struct gpio_callback *cb,
			   u32_t pin_pos)
{
	/*
	 * One button press within a 1 second interval sends an on message
	 * More than one button press sends an off message
	 */

	time = k_uptime_get_32();

	/* debounce the switch */
	if (time < last_time + BUTTON_DEBOUNCE_DELAY_MS) {
		last_time = time;
		return;
	}

	if (button_press_cnt == 0U) {
		k_timer_start(&sw.button_timer, K_SECONDS(1), 0);
	}

	printk("button_press_cnt 0x%02x\n", button_press_cnt);
	button_press_cnt++;

	/* The variable pin_pos is the pin position in the GPIO register,
	 * not the pin number. It's assumed that only one bit is set.
	 */

	sw.sw_num = pin_to_sw(pin_pos);
	last_time = time;
}

/* Button Count Timer Worker */
static void button_cnt_timer(struct k_timer *work)
{
	struct sw *button_sw = CONTAINER_OF(work, struct sw, button_timer);

	button_sw->onoff_state = button_press_cnt == 1U ? 1 : 0;
	printk("button_press_cnt 0x%02x onoff_state 0x%02x\n",
	       button_press_cnt, button_sw->onoff_state);
	button_press_cnt = 0U;
	k_work_submit(&sw.button_work);
}

// Teste
struct device *led_device;

void init_led(u8_t dev, const char *port, u32_t pin_num)
{
	led_device = device_get_binding(port);
	gpio_pin_configure(led_device,
			   pin_num, GPIO_DIR_OUT | GPIO_PUD_PULL_UP);
}

/* Button Pressed Worker Task */
static void button_pressed_worker(struct k_work *work)
{
	struct bt_mesh_model *mod_cli;
	// struct bt_mesh_model *mod_srv;
	struct bt_mesh_model_pub *pub_cli;
	// struct bt_mesh_model_pub *pub_srv;
	struct sw *sw = CONTAINER_OF(work, struct sw, button_work);
	int err;
	u8_t sw_idx = sw->sw_num;

	mod_cli = mod_cli_sw[sw_idx];
	pub_cli = mod_cli->pub;

	// mod_srv = mod_srv_sw[sw_idx];
	// pub_srv = mod_srv->pub;

	/* If unprovisioned, just call the set function.
	 * The intent is to have switch-like behavior
	 * prior to provisioning. Once provisioned,
	 * the button and its corresponding led are no longer
	 * associated and act independently. So, if a button is to
	 * control its associated led after provisioning, the button
	 * must be configured to either publish to the led's unicast
	 * address or a group to which the led is subscribed.
	 */

	// if (primary_addr == BT_MESH_ADDR_UNASSIGNED) {
	// 	NET_BUF_SIMPLE_DEFINE(msg, 1);
	// 	struct bt_mesh_msg_ctx ctx = {
	// 		.addr = sw_idx + primary_addr,
	// 	};

	// 	/* This is a dummy message sufficient
	// 	 * for the led server
	// 	 */

	// 	net_buf_simple_add_u8(&msg, sw->onoff_state);
	// 	gen_onoff_set_unack(mod_srv, &ctx, &msg);
	// 	return;
	// }

	if (pub_cli->addr == BT_MESH_ADDR_UNASSIGNED) {
		// Teste
		gpio_pin_write(led_device, LED0_GPIO_PIN, 0);
		k_sleep(500);
		gpio_pin_write(led_device, LED0_GPIO_PIN, 1);
		k_sleep(500);
		return;
	}

	// Teste
	gpio_pin_write(led_device, LED0_GPIO_PIN, 0);
	k_sleep(500);
	gpio_pin_write(led_device, LED0_GPIO_PIN, 1);
	k_sleep(500);
	gpio_pin_write(led_device, LED0_GPIO_PIN, 0);
	k_sleep(500);
	gpio_pin_write(led_device, LED0_GPIO_PIN, 1);
	k_sleep(500);

	printk("publish to 0x%04x onoff 0x%04x sw_idx 0x%04x\n",
	       pub_cli->addr, sw->onoff_state, sw_idx);
	bt_mesh_model_msg_init(pub_cli->msg,
			       BT_MESH_MODEL_OP_GEN_ONOFF_SET);
	net_buf_simple_add_u8(pub_cli->msg, sw->onoff_state);
	net_buf_simple_add_u8(pub_cli->msg, trans_id++);
	err = bt_mesh_model_publish(mod_cli);
	if (err) {
		printk("bt_mesh_model_publish err %d\n", err);
		// Teste
		gpio_pin_write(led_device, LED0_GPIO_PIN, 0);
		k_sleep(500);
		gpio_pin_write(led_device, LED0_GPIO_PIN, 1);
		k_sleep(500);
		gpio_pin_write(led_device, LED0_GPIO_PIN, 0);
		k_sleep(500);
		gpio_pin_write(led_device, LED0_GPIO_PIN, 1);
		k_sleep(500);
		gpio_pin_write(led_device, LED0_GPIO_PIN, 0);
		k_sleep(500);
		gpio_pin_write(led_device, LED0_GPIO_PIN, 1);
		k_sleep(500);
	}
}

/* Bluetooth Ready Callback */
static void bt_ready(int err)
{
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

	// /* Use identity address as device UUID */
	// struct bt_le_oob oob;
	// if (bt_le_oob_get_local(BT_ID_DEFAULT, &oob)) {
	// 	printk("Identity Address unavailable\n");
	// } else {
	// 	memcpy(dev_uuid, oob.addr.a.val, 6);
	// }

	bt_mesh_prov_enable(BT_MESH_PROV_GATT | BT_MESH_PROV_ADV);

	printk("Mesh initialized\n");
}



void main(void)
{
	int err;

	printk("Initializing...\n");

	/* Initialize the button debouncer */
	last_time = k_uptime_get_32();

	/* Initialize button worker task*/
	k_work_init(&sw.button_work, button_pressed_worker);

	/* Initialize button count timer */
	k_timer_init(&sw.button_timer, button_cnt_timer, NULL);

	sw_device = device_get_binding(SW0_GPIO_CONTROLLER);
	gpio_pin_configure(sw_device, SW0_GPIO_PIN,
			  (GPIO_DIR_IN | GPIO_INT | GPIO_INT_EDGE |
			   GPIO_INT_ACTIVE_LOW | GPIO_PUD_PULL_UP));
	gpio_init_callback(&button_cb, button_pressed, BIT(SW0_GPIO_PIN));
	gpio_add_callback(sw_device, &button_cb);
	gpio_pin_enable_callback(sw_device, SW0_GPIO_PIN);

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(bt_ready);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
	}
	
	// Teste
	init_led(0, LED0_GPIO_CONTROLLER, LED0_GPIO_PIN);
	
	//while(1) {
	//	gpio_pin_write(led_device, LED0_GPIO_PIN, 1);
	//	k_sleep(1000);
	//	gpio_pin_write(led_device, LED0_GPIO_PIN, 0);
	//	k_sleep(1000);
	//}
}

// pode implementar get para saber status do servidor sem precisar dar set
// TODO: testar esse codigo (apertar botao e ver se ta enviando algo; lembrar de provisionar)