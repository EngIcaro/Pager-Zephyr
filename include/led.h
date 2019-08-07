#ifndef _LED_H_
#define _LED_H_

#include <gpio.h>
#include "output.h"

// Função para configurar todos os LEDS
void leds_configure();

// Função para setar um determinado led
void set_led(char led, u8_t value);


// Função para setar todos os LEDS
void set_all_leds(u8_t value);

// Função de piscar todos os LEDS


// Função caracol de todos os LEDS


#endif
