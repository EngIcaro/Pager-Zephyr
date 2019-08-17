#include "buzzer.h"


output_t buzzer;

void buzzer_configure(){
  output_open(&buzzer, DT_GPIO_KEYS_BUZZER_0_GPIO_CONTROLLER);
  output_configure(&buzzer, DT_GPIO_KEYS_BUZZER_0_GPIO_PIN, GPIO_DIR_OUT);
}

void set_buzzer(u8_t value){
  output_set(&buzzer, value);
}

void alarm(){
  set_buzzer(1);
  k_sleep(SLEEP_BUZZER);
}
