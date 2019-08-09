#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <gpio.h>
#include "output.h"

#define SLEEP_MOTOR 3000

// Função para configurar os motores
void motors_configure();
// Função para setar determinado motor
void set_motor(char motor, u8_t value);
// Função que chama a rotina de call do sistema
void call_motors();


#endif
