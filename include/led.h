#ifndef _LED_H_
#define _LED_H_

#include "output.h"

#define SLEEP_LED	250	//mseconds

int leds_configure();

void set_led(char led, u8_t value);

void set_all_leds(u8_t value);

void call_leds();

#endif
