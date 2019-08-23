#include "button.h"

struct sw sw;
struct device *sw_device;
struct gpio_callback button_cb;

u8_t button_press_cnt;
u8_t trans_id;
u32_t time, last_time;

/*
 * One button press within a BUTTON_PRESS_INTERVAL_SEC sends an ON message
 * More than one sends an OFF message
 */

/* Button Pressed Event Handler */
void button_pressed(struct device *dev, struct gpio_callback *cb, u32_t pin) {
	
	time = k_uptime_get_32();

	/* Debounce the switch within a BUTTON_DEBOUNCE_DELAY_MS */
	if (time < last_time + BUTTON_DEBOUNCE_DELAY_MS) {
		last_time = time;
		return;
	}

	/* If button was pressed only one time, start counting BUTTON_PRESS_INTERVAL_SEC */
	if (button_press_cnt == 0U) {
		k_timer_start(&sw.button_timer, K_SECONDS(BUTTON_PRESS_INTERVAL_SEC), 0);
	}

	printk("[BUTTON] Button pressed.\n");
	button_press_cnt++;

	/* Save last time button was pressed for debounce purposes */
	last_time = time;
}

/* Button Count Timer Worker */
void button_cnt_timer(struct k_timer *work) {

	struct sw *button_sw = CONTAINER_OF(work, struct sw, button_timer);
	
	button_sw->onoff_state = button_press_cnt == 1U ? 1 : 0;
	printk("[BUTTON] Button was pressed 0x%02x time(s), setting state 0x%02x.\n",
	       button_press_cnt, button_sw->onoff_state);
	button_press_cnt = 0U;
	k_work_submit(&sw.button_work);
}

/**************** TESTE ***************/
struct device *led_device;

void init_led(u8_t dev, const char *port, u32_t pin_num)
{
	led_device = device_get_binding(port);
	gpio_pin_configure(led_device,
			   pin_num, GPIO_DIR_OUT | GPIO_PUD_PULL_UP);
}
/**************** TESTE ***************/

/* Button Pressed Worker Task */
void button_pressed_worker(struct k_work *work) {
	struct bt_mesh_model *mod_cli;
	struct bt_mesh_model_pub *pub_cli;
	struct sw *sw = CONTAINER_OF(work, struct sw, button_work);
	int err;

	/* Get Generic OnOff Client Model from Root */
	mod_cli = &root_models[3];
	pub_cli = mod_cli->pub;
	
	/**************** TESTE ***************/
	init_led(0, LED0_GPIO_CONTROLLER, LED0_GPIO_PIN);
	/**************** TESTE ***************/

	/* In case publish address is not defined yet */
	if (pub_cli->addr == BT_MESH_ADDR_UNASSIGNED) {
		/**************** TESTE ***************/
		gpio_pin_write(led_device, LED0_GPIO_PIN, 0);
		k_sleep(500);
		gpio_pin_write(led_device, LED0_GPIO_PIN, 1);
		k_sleep(500);
		/**************** TESTE ***************/
		printk("[BLUETOOTH] Publish address unassigned. Please, set a Publish Address for the OnOff Client.\n");
		return;
	}

	/**************** TESTE ***************/
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
	gpio_pin_write(led_device, LED0_GPIO_PIN, 0);
	k_sleep(500);
	gpio_pin_write(led_device, LED0_GPIO_PIN, 1);
	k_sleep(500);
	/**************** TESTE ***************/

	/* Publishing message */
	printk("[BLUETOOTH] Publishing state 0x%04x to address 0x%04x.\n",
	       sw->onoff_state, pub_cli->addr);
	bt_mesh_model_msg_init(pub_cli->msg, BT_MESH_MODEL_OP_GEN_ONOFF_SET);
	net_buf_simple_add_u8(pub_cli->msg, sw->onoff_state);
	net_buf_simple_add_u8(pub_cli->msg, trans_id++);
	err = bt_mesh_model_publish(mod_cli);

	if (err) {
		printk("[BLUETOOTH] Publishing failed with err %d.\n", err);
		/**************** TESTE ***************/
		// gpio_pin_write(led_device, LED0_GPIO_PIN, 0);
		// k_sleep(500);
		// gpio_pin_write(led_device, LED0_GPIO_PIN, 1);
		// k_sleep(500);
		// gpio_pin_write(led_device, LED0_GPIO_PIN, 0);
		// k_sleep(500);
		// gpio_pin_write(led_device, LED0_GPIO_PIN, 1);
		// k_sleep(500);
		// gpio_pin_write(led_device, LED0_GPIO_PIN, 0);
		// k_sleep(500);
		// gpio_pin_write(led_device, LED0_GPIO_PIN, 1);
		// k_sleep(500);
		/**************** TESTE ***************/
	}
}

void button_init() {
    /* Initialize button debouncer */
	last_time = k_uptime_get_32();

	/* Initialize button worker task */
	k_work_init(&sw.button_work, button_pressed_worker);
	
	/* Initialize button count timer */
	k_timer_init(&sw.button_timer, button_cnt_timer, NULL);

	/* Initialize button */
    sw_device = device_get_binding(BUTTON_0_CONTROLLER);
	gpio_pin_configure(sw_device, BUTTON_0_PIN, FLAGS);
	gpio_init_callback(&button_cb, button_pressed, BIT(BUTTON_0_PIN));
	gpio_add_callback(sw_device, &button_cb);
	gpio_pin_enable_callback(sw_device, BUTTON_0_PIN);
}