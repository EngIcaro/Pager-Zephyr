#include "battery.h"

/* https://lowpowerlab.com/forum/moteino/3-7v-lipo-what-is-considered-low-battery/ */

input_t meter, pgood, chr;
output_t ce, en1, en2, sysoff;

float resistor_R17 = 4700000;   // 4.7MOhm
float resistor_R18 = 10000000;  // 10MOhm
float max_voltage = 3.7;        // full charge
float min_voltage = 3.3;        // discharged

int get_meter() {
    //TODO: pesquisar api ADC para fazer leitura
    float measure = 0;
    float battery_voltage = 0;
    int percent = 0;

    /* Calculate battery voltage percent */
    battery_voltage = (measure * (resistor_R17 + resistor_R18)) / resistor_R18;

    percent = (int) (battery_voltage - min_voltage) / (max_voltage - min_voltage);

    return percent;
}

int get_pgood() {
    u32_t pgood_status = 0U;
    input_read(&pgood, &pgood_status);

    return pgood_status;
}

int get_chr() {
    u32_t chr_status = 0U;
    input_read(&chr, &chr_status);

    return chr_status;
}

void set_ce(u8_t value) {
    output_set(&ce, value);
}

void set_en(u8_t value_1, u8_t value_2) {
    output_set(&en1, value_1);
    output_set(&en2, value_2);
}

void set_sysoff(u8_t value) {
    output_set(&sysoff, value);
}

int battery_configure() {
    //TODO: pesquisar como declara pino analogico
    ///device_get(adc_port)
    //adc_channel_config(struct)
    //adc_read()
    // if(input_open(&meter,   METER_GPIO_PORT)){ return 1; }
    if(input_open(&pgood,   PGOOD_GPIO_PORT)){ return 1; }
    if(input_open(&chr,     CHR_GPIO_PORT)){ return 1; }

    if(output_open(&ce,     CE_GPIO_PORT)){ return 1; }
    if(output_open(&en1,    EN1_GPIO_PORT)){ return 1; }
    if(output_open(&en2,    EN2_GPIO_PORT)){ return 1; }
    if(output_open(&sysoff, SYSOFF_GPIO_PORT)){ return 1; }

    // if(input_configure(&meter,  METER_GPIO_PIN, GPIO_DIR_IN)){ return 1; }
    if(input_configure(&pgood,  PGOOD_GPIO_PIN, GPIO_DIR_IN)){ return 1; }
    if(input_configure(&chr,    CHR_GPIO_PIN,   GPIO_DIR_IN)){ return 1; }

    if(output_configure(&ce,     CE_GPIO_PIN,     GPIO_DIR_OUT)){ return 1; }
    if(output_configure(&en1,    EN1_GPIO_PIN,    GPIO_DIR_OUT)){ return 1; }
    if(output_configure(&en2,    EN2_GPIO_PIN,    GPIO_DIR_OUT)){ return 1; }
    if(output_configure(&sysoff, SYSOFF_GPIO_PIN, GPIO_DIR_OUT)){ return 1; }

    set_ce(0);      // (0 = enable charger / 1 = disable charger)
    set_en(1, 0);   // (0,0 = 100mA / 0,1 = 500mA / 1,0 = ILIM defined, 1.3A / 1,1 = standby)
    set_sysoff(0);  // (0 = normal operation / 1 = disable battery)

    return 0;
}