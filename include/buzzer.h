#ifndef _BUZZER_H_
#define _BUZZER_H_

#include <gpio.h>
#include "output.h"

#define SLEEP_BUZZER 3	//seconds

int buzzer_configure();

void set_buzzer(u8_t value);

void alarm();

#endif