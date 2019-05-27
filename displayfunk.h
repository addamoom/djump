#ifndef DISPLAYFUNK_H
#define DISPLAYFUNK_H

#include "types.h"

void delay_250ns(void);
void delay_mikro(unsigned int us);
void delay_milli(unsigned int ms);
void delay_500ns(void);
void init_gpio(void);
void graphic_ctrl_bit_set(unsigned char x);
void graphic_ctrl_bit_clear(unsigned char x);
void select_controller(unsigned char controller);
void graphic_wait_ready(void);
unsigned char graphic_read(unsigned char controller);
void graphic_write(unsigned char value,unsigned char controller);
void graphic_write_command(unsigned char cmd,unsigned char controller);
void graphic_write_data(unsigned char data,unsigned char controller);
unsigned char graphic_read_data(unsigned char controller);
void graphic_initialize (void);
void graphics_clear_screen(void);
void pixel(unsigned char x, unsigned char y,unsigned set);

#define stick (*((volatile systick *) 0xE000E010))
#define GPIO_E (*((volatile GPIO *) 0x40021000))
#define B_E (unsigned char) 0x40
#define B_SELECT (unsigned char) 0x04
#define B_RW (unsigned char) 0x02
#define B_RS (unsigned char) 0x01
#define B_CS1 (unsigned char) 0x08
#define B_CS2 (unsigned char) 0x10
#define B_RST (unsigned char) 0x20
#define LCD_ON (unsigned char) 0x3F
#define LCD_OFF (unsigned char) 0x3E
#define LCD_SET_ADD (unsigned char) 0x40
#define LCD_SET_PAGE (unsigned char) 0xB8
#define LCD_DISP_START (unsigned char) 0xC0
#define LCD_BUSY (unsigned char) 0x80

#define HOR_EDGE 1
#define VERT_EDGE 2

#endif