#ifndef _BUZZER_H_
#define _BUZZER_H_

#include <gpio.h>
#include "output.h"

#define SLEEP_BUZZER 3000

// Função que configura o buzzer
void buzzer_configure();
// Função que seta o buzzer com valor desejado
void set_buzzer(u8_t value);
// Função que chama a rotina de alarme do buzzer
void alarm();


#endif
