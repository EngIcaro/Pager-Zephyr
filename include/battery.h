#ifndef _BATTERY_H_
#define _BATTERY_H_

#include <adc.h>
#include <gpio.h>
#include <math.h>
#include "input.h"
#include "output.h"

/*  Inputs:
 *       METER - battery voltage
 *       PGOOD - power good status indication
 *       CHR   - charging status indication
 *  Outputs:
 *       CE      - charger enable active-low
 *       EN      - input current limit config
 *       SYSOFF  - system enable
*/

//TODO: procurar adc_port
// #define METER_GPIO_PORT   "GPIO_0"
#define PGOOD_GPIO_PORT   "GPIO_0"
#define CHR_GPIO_PORT     "GPIO_0"
#define CE_GPIO_PORT      "GPIO_0"
#define EN1_GPIO_PORT     "GPIO_0"
#define EN2_GPIO_PORT     "GPIO_0"
#define SYSOFF_GPIO_PORT  "GPIO_0"

// #define METER_GPIO_PIN   31
#define PGOOD_GPIO_PIN   27
#define CHR_GPIO_PIN     26
#define CE_GPIO_PIN      30
#define EN1_GPIO_PIN     28
#define EN2_GPIO_PIN     29
#define SYSOFF_GPIO_PIN  25

/* Get battery percentage */
int get_meter();

/* Get PGOOD status */
int get_pgood();

/* Get CHR status */
int get_chr();

/* Set Charger Enable mode */
void set_ce();

/* Set Input Current Limit mode */
void set_en();

/* Set SYSOFF mode */
void set_sysoff();

/* Configure Battery Pins */
int battery_configure();

#endif