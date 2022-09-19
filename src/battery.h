#pragma once

#define CHARGE_PIN 24
#define VSYS 29



#define VSYS_CONVERT(v) (((float)v) * (3 * 3.27 / ((uint16_t)1 << 12)))

void setup_battery();
int get_bat_charge();
bool get_bat_charging();