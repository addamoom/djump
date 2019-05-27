#include "types.h"
#include "displayfunk.h"
#include "AsciiDisplayFunk.h"

/*Addressera ascii-display och ettställ bitar som är i x*/
void ascii_ctrl_bit_set(unsigned char x)
{
	GPIO_E.odrLow |= x;
	GPIO_E.odrLow |= B_SELECT;
}

/*Addressera ascii-display och nollställ bitar som är i x*/
void ascii_ctrl_bit_clear(unsigned char x)
{
	GPIO_E.odrLow &= ~x;
	GPIO_E.odrLow |= B_SELECT;
}

void ascii_write_controller(unsigned char command)
{
	ascii_ctrl_bit_set(B_E);
	GPIO_E.odrHigh = command;
	delay_250ns();
	ascii_ctrl_bit_clear(B_E);
}

unsigned char ascii_read_controller(void)
{
	ascii_ctrl_bit_set(B_E);
	delay_250ns();
	delay_250ns();
	unsigned char rv = GPIO_E.idrHigh;
	ascii_ctrl_bit_clear(B_E);
	return rv;
}

void ascii_write_cmd(unsigned char command)
{
	ascii_ctrl_bit_clear(B_RS);
	ascii_ctrl_bit_clear(B_RW);
	ascii_write_controller(command);
}

void ascii_write_data(unsigned char data)
{
	ascii_ctrl_bit_set(B_RS);
	ascii_ctrl_bit_clear(B_RW);
	ascii_write_controller(data);
}

unsigned char ascii_read_status(void)
{
	// omkonfigurera till ingång temporärt
	GPIO_E.moder &= 0x0000ffff;
	
	ascii_ctrl_bit_clear(B_RS);
	ascii_ctrl_bit_set(B_RW);
	
	unsigned char rv = ascii_read_controller();
	
	// tillbaka till utgång
	GPIO_E.moder |= 0x55550000;
	
	return rv;
}

unsigned char ascii_read_data(void)
{
	// omkonfigurera till ingång temporärt
	GPIO_E.moder &= 0x0000ffff;
	
	ascii_ctrl_bit_set(B_RS);
	ascii_ctrl_bit_set(B_RW);
	
	unsigned char rv = ascii_read_controller();
	
	// tillbaka till utgång
	GPIO_E.moder |= 0x55550000;
	
	return rv;
}

void ascii_init(void)
{
	// clear display
	while(ascii_read_status() & 0x80)
	{}
	delay_mikro(8);
	ascii_write_cmd(0x01);
	delay_milli(2);
	
	// function set
	while(ascii_read_status() & 0x80)
	{}
	delay_mikro(8);
	ascii_write_cmd(0x38);
	delay_mikro(39);
	
	// display controll
	while(ascii_read_status() & 0x80)
	{}
	delay_mikro(8);
	ascii_write_cmd(0x0e);
	delay_mikro(39);
	
	// entry mode set
	while(ascii_read_status() & 0x80)
	{}
	delay_mikro(8);
	ascii_write_cmd(0x04);
	delay_mikro(39);
}

void ascii_write_char(unsigned char c)
{
	while(ascii_read_status() & 0x80)
	{}
	delay_mikro(8);
	ascii_write_data(c);
	delay_mikro(43);
}

void ascii_gotoxy(int x, int y)
{
	int address = x-1;
	if (y == 2)
	{
		address = address + 0x40;
	}
	
	while(ascii_read_status() & 0x80)
	{}
	delay_mikro(8);
	ascii_write_cmd(0x80|address);
	delay_mikro(39);
}

void ascii_clear_display(void)
{
	while(ascii_read_status() & 0x80)
	{}
	delay_mikro(8);
	ascii_write_cmd(0x01);
	delay_milli(2);
} 