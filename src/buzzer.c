#include "buzzer.h"


output_t buzzer;

int buzzer_configure(){
  if(output_open(&buzzer, DT_GPIO_KEYS_BUZZER_0_GPIO_CONTROLLER)){ return 1; }
  if(output_configure(&buzzer, DT_GPIO_KEYS_BUZZER_0_GPIO_PIN, GPIO_DIR_OUT)){ return 1; }
  return 0;
}

void set_buzzer(u8_t value){
  output_set(&buzzer, value);
}

void alarm(){
  set_buzzer(1);
  k_sleep(SLEEP_BUZZER);
}
