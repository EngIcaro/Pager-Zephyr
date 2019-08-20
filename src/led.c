#include "led.h"
#include "ic_logging.h"

output_t led0, led1, led2, led3, led4, led5;

void leds_configure(){

  //printk("Led: 0 ... opening ...");
  //printk("\033[1;34mLed: 0 ... opening ...\033[0m");
  k_sleep(500);
  output_open(&led0, LED0_GPIO_CONTROLLER);
  //printk("\033[1;32mOk!\033[0m\n");
  k_sleep(500);

  //printk("Led: 1 ... opening ...");
  //printk("\033[1;34mLed: 1 ... opening ...\033[0m");
  k_sleep(500);
  output_open(&led1, LED1_GPIO_CONTROLLER);
  //printk("\033[1;32mOk!\033[0m\n");
  k_sleep(500);

  //printk("\033[1;34mLed: 2 ... opening ...\033[0m");
  k_sleep(500);
  output_open(&led2, LED2_GPIO_CONTROLLER);
  //printk("\033[1;32mOk!\033[0m\n");
  k_sleep(500);

 // printk("\033[1;34mLed: 3 ... opening ...\033[0m");
  k_sleep(500); 
  output_open(&led3, LED3_GPIO_CONTROLLER);
  //printk("\033[1;32mOk!\033[0m\n");
  k_sleep(500);

  //printk("\033[1;34mLed: 4 ... opening ...\033[0m");
  k_sleep(500); 
  output_open(&led4, LED4_GPIO_CONTROLLER);
  //printk("\033[1;32mOk!\033[0m\n");
  k_sleep(500);

 // printk("\033[1;34mLed: 5 ... opening ...\033[0m");
  k_sleep(500);
  output_open(&led5, LED5_GPIO_CONTROLLER);
 // printk("\033[1;32mOk!\033[0m\n");
  k_sleep(500);
  
 // printk("\033[1;34mLed: 0 ... configurating ...\033[0m");
  k_sleep(500);
  output_configure(&led0, LED0_GPIO_PIN, GPIO_DIR_OUT);
 // printk("\033[1;32mOk!\033[0m\n");
  k_sleep(500);

//  printk("\033[1;34mLed: 1 ... configurating ...\033[0m");
  k_sleep(500);
  output_configure(&led1, LED1_GPIO_PIN, GPIO_DIR_OUT);
 // printk("\033[1;32mOk!\033[0m\n");
  k_sleep(500);

 // printk("\033[1;34mLed: 2 ... configurating ...\033[0m");
  k_sleep(500);
  output_configure(&led2, LED2_GPIO_PIN, GPIO_DIR_OUT);
  //printk("\033[1;32mOk!\033[0m\n");
  k_sleep(500);

 // printk("\033[1;34mLed: 3 ... configurating ...\033[0m");
  k_sleep(500);
  output_configure(&led3, LED3_GPIO_PIN, GPIO_DIR_OUT);
 // printk("\033[1;32mOk!\033[0m\n");
  k_sleep(500);

 // printk("\033[1;34mLed: 4 ... configurating ...\033[0m");
  k_sleep(500);
  output_configure(&led4, LED4_GPIO_PIN, GPIO_DIR_OUT);
 // printk("\033[1;32mOk!\033[0m\n");
  k_sleep(500);

  //printk("\033[1;34mLed: 5 ... configurating ...\033[0m");
  k_sleep(500);
  output_configure(&led5, LED5_GPIO_PIN, GPIO_DIR_OUT);
 // printk("\033[1;32mOk!\033[0m\n");
  k_sleep(500);

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

void call_leds(){
  set_led('0', 0);
  k_sleep(SLEEP_LED);
  set_led('1', 0);
  k_sleep(SLEEP_LED);
  set_led('2', 0);
  k_sleep(SLEEP_LED);
  set_led('3', 0);
  k_sleep(SLEEP_LED);
  set_led('4', 0);
  k_sleep(SLEEP_LED);
  set_led('5', 0);
  k_sleep(SLEEP_LED);
  // back
  set_led('5', 1);
  k_sleep(SLEEP_LED);
  set_led('4', 1);
  k_sleep(SLEEP_LED);
  set_led('3', 1);
  k_sleep(SLEEP_LED);
  set_led('2', 1);
  k_sleep(SLEEP_LED);
  set_led('1', 1);
  k_sleep(SLEEP_LED);
  set_led('0', 1);
}
