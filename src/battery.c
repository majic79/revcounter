#include <stdio.h>
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "battery.h"

#define BAT_FULL 4.2
#define BAT_EMPTY 2.8

void setup_battery() {
    adc_init();

    adc_gpio_init(VSYS);
    //adc_select_input(0);

    gpio_init(CHARGE_PIN);
    gpio_set_dir(CHARGE_PIN, GPIO_IN);
}

int get_bat_charge() {
    adc_select_input(3);
    uint16_t vsys = adc_read();
    float vsys_f = VSYS_CONVERT(vsys);
    int p = (int)(100.0 * ((vsys_f - BAT_EMPTY) / (BAT_FULL - BAT_EMPTY)));
    if(p > 100) p = 100;
    printf("VSYS: %d\nVSYS_F: %f\n", vsys, vsys_f);
    return p;
}

bool get_bat_charging() {
    bool v = gpio_get(CHARGE_PIN);
    return v;
}