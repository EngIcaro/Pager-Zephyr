#include "motor.h"

output_t motor0, motor1;

int motors_configure(){
  
  if(output_open(&motor0, DT_GPIO_KEYS_MOTOR_0_GPIO_CONTROLLER)){ return 1; }
  if(output_open(&motor1, DT_GPIO_KEYS_MOTOR_1_GPIO_CONTROLLER)){ return 1; }
  if(output_configure(&motor0, DT_GPIO_KEYS_MOTOR_0_GPIO_PIN, GPIO_DIR_OUT)){ return 1; }
  if(output_configure(&motor1, DT_GPIO_KEYS_MOTOR_1_GPIO_PIN, GPIO_DIR_OUT)){ return 1; }

  return 0;

}

void set_motor(char motor, u8_t value){
  switch (motor) {
    case '0':
      output_set(&motor0, value);
      break;
    case '1':
      output_set(&motor1, value);
      break;
    default:
      printk("Value of set motor is incorret");
      break;
    }
}

void call_motors(){
  set_motor('0', 1);
  set_motor('1', 1);
  k_sleep(SLEEP_MOTOR);
  set_motor('0', 0);
  set_motor('1', 0);
}
