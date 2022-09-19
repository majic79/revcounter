#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "ssd1306.h"

uint8_t *buf;

void ssd1306_setup_i2c() {
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_SSD1306_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_SSD1306_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_SSD1306_I2C_SDA_PIN);
    gpio_pull_up(PICO_SSD1306_I2C_SCL_PIN);
}

void ssd1306_repeat_fill(uint8_t c) {
    unsigned char buf[OLED_BUF_LEN + 1];
    for(int a = 0; a < OLED_BUF_LEN; a++) {
        buf[a + 1] = c;
    }
    buf[0] = SSD1306_CTRL_DATA;
    i2c_write_blocking(PICO_SSD1306_I2C, SSD1306_I2C_ADDR, buf, OLED_BUF_LEN + 1, false);
}

void ssd1306_send_cmd(uint8_t cmd) {
    uint8_t buf[2] = {SSD1306_CTRL_CMD, cmd};
    i2c_write_blocking(PICO_SSD1306_I2C, SSD1306_I2C_ADDR, buf, 2, false);
}

void ssd1306_send_data(uint8_t sbuf[], int buflen) {
    unsigned char buf[OLED_BUF_LEN + 1];

    for(int a = 0; a < buflen; a++) {
        buf[a + 1] = sbuf[a];
    }
    buf[0] = SSD1306_CTRL_DATA;
    i2c_write_blocking(PICO_SSD1306_I2C, SSD1306_I2C_ADDR, buf, buflen + 1, false);
}

void ssd1306_address_mode(uint8_t ssd1306_address_mode) {

    ssd1306_send_cmd(SSD1306_CMD_MEM_ADDRESS_MODE); 
    ssd1306_send_cmd(ssd1306_address_mode);
}

// e.g. ssd1306_precharge(0xF1)
void ssd1306_precharge(uint8_t c) {
    ssd1306_send_cmd(SSD1306_CMD_PRECHARGE);
    ssd1306_send_cmd(c);
}

void ssd1306_init() {
    // uint8_t buf[1] = { 0x00 };
    ssd1306_send_cmd(SSD1306_CMD_DISPLAY_OFF); // turn display off
    sleep_ms(100);

    ssd1306_send_cmd(SSD1306_CMD_MUX_RATIO); // set multiplex ratio
    ssd1306_send_cmd(OLED_HEIGHT - 1); // our display is only 32 pixels high

    ssd1306_send_cmd(SSD1306_CMD_DISPLAY_OFFSET); // set display offset
    ssd1306_send_cmd(0x00); // no offset

    ssd1306_send_cmd(SSD1306_CMD_DISPLAY_START_LINE | 0x00); // set display start line to 0

    ssd1306_send_cmd(SSD1306_CMD_SEGMENT_REMAP | 0x00); // set segment re-map
//    ssd1306_send_cmd(SSD1306_CMD_SEGMENT_REMAP | 0x01); // set segment re-map
    // column address 127 is mapped to SEG0

    ssd1306_send_cmd(SSD1306_CMD_COM_SCAN_DIR | SSD1306_COM_SCAN_NORMAL); // set COM (common) output scan direction
    // scan from top down

    // buf[0] = SSD1306_COM_PIN_SEQ | SSD1306_COM_PIN_DISABLE_LR;
    ssd1306_send_cmd(SSD1306_CMD_COM_PIN_CFG); // set COM (common) pins hardware configuration
    ssd1306_send_cmd(SSD1306_COM_PIN_SEQ | SSD1306_COM_PIN_DISABLE_LR);

    // display
    ssd1306_send_cmd(SSD1306_CMD_CONTRAST); // set contrast control
    ssd1306_send_cmd(0x7F);

    ssd1306_send_cmd(SSD1306_CMD_DISPLAY_RAM); // set entire display to follow RAM content

    ssd1306_send_cmd(SSD1306_CMD_DISPLAY_NORMAL); // set normal (non-inverted) display

    // timing and driving scheme
    ssd1306_send_cmd(SSD1306_CMD_OSC_CLK_DIV); // set display clock divide ratio
    ssd1306_send_cmd(0x80); // RESET value (from datasheet)

    ssd1306_send_cmd(SSD1306_CMD_PRECHARGE);
    ssd1306_send_cmd(0x22);
    
    ssd1306_send_cmd(SSD1306_CMD_VCOM_DESEL); // set VCOMH deselect level
    ssd1306_send_cmd(SSD1306_VCOM_065);


 //   ssd1306_send_cmd(SSD1306_CMD_SCROLL_STOP); // deactivate horizontal scrolling if set

    // Switch on the charge pump and turn the display on
    ssd1306_send_cmd(SSD1306_CMD_CHARGE_PUMP);
    ssd1306_send_cmd(SSD1306_CHARGE_PUMP_EN);

    ssd1306_send_cmd(SSD1306_CMD_DISPLAY_ON); // turn display on
    sleep_ms(100);
}

void ssd1306_set_page_addr(uint8_t start, uint8_t end) {
    ssd1306_send_cmd(SSD1306_CMD_PAGE_ADDR);
    ssd1306_send_cmd(start & 0x07);
    ssd1306_send_cmd(end & 0x07);
}

void ssd1306_set_page_start(uint8_t start) {
    ssd1306_send_cmd(SSD1306_CMD_PAGE_START | ( start & 0x07 ));
}

void ssd1306_set_column_addr(uint8_t start, uint8_t end) {
    ssd1306_send_cmd(SSD1306_CMD_COL_ADDR);
    ssd1306_send_cmd(start & 0x7F);
    ssd1306_send_cmd(end & 0x7F);
}
