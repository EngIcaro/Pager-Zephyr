#include "led.h"

output_t led0, led1, led2, led3, led4, led5;

/* Leds setup */
int leds_configure() {

  if(output_open(&led0, LED0_GPIO_CONTROLLER)){ return 1; }
  if(output_open(&led1, LED1_GPIO_CONTROLLER)){ return 1; }
  if(output_open(&led2, LED2_GPIO_CONTROLLER)){ return 1; }
  if(output_open(&led3, LED3_GPIO_CONTROLLER)){ return 1; }
  if(output_open(&led4, LED4_GPIO_CONTROLLER)){ return 1; }
  if(output_open(&led5, LED5_GPIO_CONTROLLER)){ return 1; } 

  if(output_configure(&led0, LED0_GPIO_PIN, GPIO_DIR_OUT)){ return 1; }
  if(output_configure(&led1, LED1_GPIO_PIN, GPIO_DIR_OUT)){ return 1; }
  if(output_configure(&led2, LED2_GPIO_PIN, GPIO_DIR_OUT)){ return 1; }
  if(output_configure(&led3, LED3_GPIO_PIN, GPIO_DIR_OUT)){ return 1; }
  if(output_configure(&led4, LED4_GPIO_PIN, GPIO_DIR_OUT)){ return 1; }
  if(output_configure(&led5, LED5_GPIO_PIN, GPIO_DIR_OUT)){ return 1; }

  return 0;
}

/* Set a led with desired value (0 - ON, 1 - OFF) */
void set_led(char led, u8_t value) {
  switch (led) {
    case '0':
      output_set(&led0, value);
      break;
    case '1':
      output_set(&led1, value);
      break;
    case '2':
      output_set(&led2, value);
      break;
    case '3':
      output_set(&led3, value);
      break;
    case '4':
      output_set(&led4, value);
      break;
    case '5':
      output_set(&led5, value);
      break;
    default:
      printk("[LED] Value of set led is incorret.\n");
      break;
  }
}

/* Set all leds with desired value (0 - ON, 1 - OFF) */
void set_all_leds(u8_t value) {
  output_set(&led0, value);
  output_set(&led1, value);
  output_set(&led2, value);
  output_set(&led3, value);
  output_set(&led4, value);
  output_set(&led5, value);
}

/* Leds alarm routine */
void call_leds() {
  set_led('0', 0);
  k_sleep(K_MSEC(SLEEP_LED));
  set_led('1', 0);
  k_sleep(K_MSEC(SLEEP_LED));
  set_led('2', 0);
  k_sleep(K_MSEC(SLEEP_LED));
  set_led('3', 0);
  k_sleep(K_MSEC(SLEEP_LED));
  set_led('4', 0);
  k_sleep(K_MSEC(SLEEP_LED));
  set_led('5', 0);
  k_sleep(K_MSEC(SLEEP_LED));

  set_led('5', 1);
  k_sleep(K_MSEC(SLEEP_LED));
  set_led('4', 1);
  k_sleep(K_MSEC(SLEEP_LED));
  set_led('3', 1);
  k_sleep(K_MSEC(SLEEP_LED));
  set_led('2', 1);
  k_sleep(K_MSEC(SLEEP_LED));
  set_led('1', 1);
  k_sleep(K_MSEC(SLEEP_LED));
  set_led('0', 1);
  k_sleep(K_MSEC(SLEEP_LED));
}