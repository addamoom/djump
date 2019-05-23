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