#ifndef _LED_H_
#define _LED_H_

#include <gpio.h>
#include "output.h"


#define SLEEP_LED	250



// Função para configurar todos os LEDS
void leds_configure();

// Função para setar um determinado led
void set_led(char led, u8_t value);

// Função para setar todos os LEDS
void set_all_leds(u8_t value);

// Função que chama a rotina call do sistema
void call_leds();



#endif
