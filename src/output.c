#include "output.h"
#include <gpio.h>

int output_open(output_t *out, char *dev_label) {
    out->dev_label = dev_label;
    out->device = device_get_binding(out->dev_label);
    if(out->device == NULL){
      printk("Error: Device not found or cannot be used");
      return 1;
    }
    printk("Returning from output_open\n");
    return 0;
}

int output_configure(output_t *out, u32_t pin, int flags) {
    out->pin = pin;
    int status = gpio_pin_configure(out->device, out->pin, flags);
    if(status < 0){
      printk("Error: configure pin error");
    }
    printk("Returning from output_configure\n");
    return 0;
}

int output_set(output_t *out, u8_t value) {
    out->state = value;
    gpio_pin_write(out->device, out->pin, out->state);
    printk("Returning from output_set\n");
    return 0;
}
