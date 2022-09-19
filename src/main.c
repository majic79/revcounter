#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
//#include "hardware/i2c.h"
#include "pico/binary_info.h"

#include "opto.h"
#include "revs.h"
#include "ssd1306.h"
#include "font.h"
#include "battery.h"

#define LED_PIN 25

bool update_rpm = true;

repeating_timer_t timer;

bool lp = 0;
uint8_t s = 0;

uint8_t pattern = 0b00110011;

uint8_t oled_buf[OLED_BUF_LEN];

char s_rpm[16];
char s_line[] = "RevCounter";
char s_bat[8];

uint32_t tick_prev = 0;
uint32_t t_timing = 0;

void handle_gpio(uint gpio, uint32_t event_mask) {
    uint32_t tick = time_us_32();
    t_timing += tick - tick_prev;
    tick_prev = tick;
    if(gpio = OPTO_PIN) {
        if(event_mask & GPIO_IRQ_EDGE_RISE) revcount++;
    }
}

bool timer_callback(repeating_timer_t *rt) {
    calculate_revs(t_timing);
    t_timing = 0;
    update_rpm = true;
    return true;
}

void setup() {
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    setup_battery();

    ssd1306_setup_i2c();
    ssd1306_init();
    //ssd1306_address_mode(SSD1306_ADDRMODE_HORIZONTAL);
    ssd1306_address_mode(SSD1306_ADDRMODE_VERTICAL);
    ssd1306_set_page_addr(0x00, 0x03);
    ssd1306_set_column_addr(0x00, 0x7F);

    setup_opto();

    gpio_set_irq_enabled_with_callback(OPTO_PIN, GPIO_IRQ_EDGE_RISE, true, handle_gpio);

    add_repeating_timer_us(-1000000, timer_callback, NULL, &timer);

    tick_prev = time_us_32();

    for(int a=0; a < OLED_BUF_LEN; a++) oled_buf[a] = 0;
}

void loop() {
    bool o = ~gpio_get(OPTO_PIN);
    gpio_put(LED_PIN, o);
    if(lp != o) {
//        if(o) {
//            ssd1306_send_cmd(SSD1306_CMD_DISPLAY_NORMAL);
//            ssd1306_send_cmd(SSD1306_CMD_DISPLAY_RAM);
//        } else {
//            ssd1306_send_cmd(SSD1306_CMD_DISPLAY_ENTIRE_ON);
//            ssd1306_send_cmd(SSD1306_CMD_DISPLAY_INVERSE);
//        }
        lp = o;
    }
    if(update_rpm) {
        bool cs = get_bat_charging();
        int bc = get_bat_charge();
        char csi = cs ? 'C' : 'D';
        //for(int a = 0; a < OLED_NUM_PAGES * ((s & 0x7F) + 1); a ++) oled_buf[a] = s & 0x80 ? 0x00 : pattern;
        // font_to_buffer(ssd1306xled_font8x16, oled_buf,0,0, OLED_NUM_PAGES, (s % 0x60) + 0x21);

        sprintf(s_rpm,"%6drpm", revs);
        sprintf(s_bat,"%3d%%", bc);
        for(int a = 0; a < 14; a++) {
            if(s_rpm[a] == 0) {
                for(int b = a; b < 14; b++)
                    font_to_buffer(ssd1306xled_font8x16, oled_buf,b << 3,0, OLED_NUM_PAGES, ' ');
                break;
            }
            font_to_buffer(ssd1306xled_font8x16, oled_buf,a << 3,0, OLED_NUM_PAGES, s_rpm[a]);
        }
        for(int a = 0; a < 16; a++) {
            if(s_line[a] == 0) break;
            font_to_buffer(ssd1306xled_font8x16, oled_buf,a << 3,2, OLED_NUM_PAGES, s_line[a]);
        }

        for(int a = 0; a < 4; a++) {
            //if(s_bat[a] == 0) break;
            font_to_buffer(ssd1306xled_font8x16, oled_buf,(12 + a) << 3,0, OLED_NUM_PAGES, s_bat[a]);
        }

        font_to_buffer(ssd1306xled_font8x16, oled_buf, 15 << 3, 2, OLED_NUM_PAGES, csi);

        ssd1306_send_data(oled_buf, OLED_BUF_LEN);

        printf("RPM: %d\n", revs);
        update_rpm = false;
    }    
}

int main() {
    bi_decl(bi_program_description("WindTurbine Rev Counter"));
    bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));
    bi_decl(bi_1pin_with_name(OPTO_PIN, "Optointerrupter (L393) Digital In"));
    bi_decl(bi_2pins_with_func(PICO_SSD1306_I2C_SDA_PIN, PICO_SSD1306_I2C_SCL_PIN, GPIO_FUNC_I2C));
    setup();

    while(1) {
        loop();
    } 
    return 0;
}