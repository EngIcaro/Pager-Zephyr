#include "led.h"

#define LED_CONTROLLER_ZERO	    LED0_GPIO_CONTROLLER
#define LED_CONTROLLER_ONE		  LED1_GPIO_CONTROLLER
#define LED_CONTROLLER_TWO	    LED2_GPIO_CONTROLLER
#define LED_CONTROLLER_THREE	  LED3_GPIO_CONTROLLER
#define LED_CONTROLLER_FOUR	    LED4_GPIO_CONTROLLER
#define LED_CONTROLLER_FIVE	    LED5_GPIO_CONTROLLER
#define LED_PIN_ZERO						LED0_GPIO_PIN
#define LED_PIN_ONE						  LED1_GPIO_PIN
#define LED_PIN_TWO					    LED2_GPIO_PIN
#define LED_PIN_THREE					  LED3_GPIO_PIN
#define LED_PIN_FOUR					  LED4_GPIO_PIN
#define LED_PIN_FIVE					  LED5_GPIO_PIN

output_t led0, led1, led2, led3, led4, led5;

void leds_configure(){

  output_open(&led0, LED0_GPIO_CONTROLLER);
  output_open(&led1, LED1_GPIO_CONTROLLER);
  output_open(&led2, LED2_GPIO_CONTROLLER);
  output_open(&led3, LED3_GPIO_CONTROLLER);
  output_open(&led4, LED4_GPIO_CONTROLLER);
  output_open(&led5, LED5_GPIO_CONTROLLER);

  output_configure(&led0, LED0_GPIO_PIN, GPIO_DIR_OUT);
  output_configure(&led1, LED1_GPIO_PIN, GPIO_DIR_OUT);
  output_configure(&led2, LED2_GPIO_PIN, GPIO_DIR_OUT);
  output_configure(&led3, LED3_GPIO_PIN, GPIO_DIR_OUT);
  output_configure(&led4, LED4_GPIO_PIN, GPIO_DIR_OUT);
  output_configure(&led5, LED5_GPIO_PIN, GPIO_DIR_OUT);
}

void set_led(char led, u8_t value){
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
      printk("Value of set led is incorret");
      break;
  }
}

void set_all_leds(u8_t value){
  output_set(&led0, value);
  output_set(&led1, value);
  output_set(&led2, value);
  output_set(&led3, value);
  output_set(&led4, value);
  output_set(&led5, value);
}
