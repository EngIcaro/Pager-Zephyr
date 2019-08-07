#include "input.h"
#include <gpio.h>

//static struct gpio_callback callback;

int input_open(input_t *in, char *dev_label) {
    in->dev_label = dev_label;
    in->device = device_get_binding(in->dev_label);
    //in->gpio_callback = callback;
    printk("Returning from input_open\n");
    return 0;
}

int input_configure(input_t *in, u32_t pin, int flags, gpio_callback_handler_t cb) {
    in->pin = pin;
    gpio_pin_configure(in->device, in->pin, flags);

    gpio_init_callback(&in->gpio_callback, cb, BIT(in->pin));
    gpio_add_callback(in->device, &in->gpio_callback);
    gpio_pin_enable_callback(in->device, in->pin);
    printk("Returning from input_configure\n");
    return 0;
}

int input_read(input_t *in, u32_t *state) {
    gpio_pin_read(in->device, in->pin, state);
    //printk("Returning from input_read\n");
    return 0;
}
