#include "hardware/gpio.h"
#include "opto.h"

void setup_opto() {
    gpio_init(OPTO_PIN);
    gpio_set_dir(OPTO_PIN, GPIO_IN);
}