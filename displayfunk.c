#include "displayfunk.h"
#include <stdlib.h>


typedef struct {
	unsigned int ctrl;
	unsigned int load;
	unsigned int val;
	unsigned int calib;
} systick;

typedef struct {
	unsigned int moder;				// 32 bit
	unsigned short otyper;			// 16 bit
	unsigned short otReserved;		// 16 bit, reserverad
	unsigned int ospeedr;			// 32 bit
	unsigned int pupdr;				// 32 bit
	unsigned char idrLow;			// 8 bit
	unsigned char idrHigh;			// 8 bit
	unsigned short idrReserved;		// 16 bit, reserverad
	unsigned char odrLow;			// 8 bit
	unsigned char odrHigh;			// 8 bit
	unsigned short odrReserved; 	// 16 bit, reserverad
} GPIO;

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

void delay_250ns(void){
		stick.ctrl &= 0xfffffff7;
		
		stick.load &= 0xff000000;
		stick.load |= 0x0000002A;

		stick.ctrl |= 0x00000005;
		int a;
		
		while(!a){
			a = stick.ctrl;
			a &= 0x00010000;
		}
}
void delay_mikro(unsigned int us){
	for(int i=0;i<=us*4;i++){
		delay_250ns();
	}
}
void delay_milli(unsigned int ms){
		//ms=ms/1000;
		//ms++;
	
	for(int i=0;i<=(4*ms);i++){ //ändra 4 till 4000 när det ska till armen
		delay_250ns();
	}
}
void delay_500ns(void){
		delay_250ns();
		delay_250ns();
}
void init_gpio(){
	GPIO_E.moder = 0x55555555;
	GPIO_E.otyper = 0x0000;
	GPIO_E.pupdr = 55555555;
	GPIO_E.odrLow |= B_SELECT;
}

//typedef unsigned char unsigned char;
void graphic_ctrl_bit_set(unsigned char x){
	GPIO_E.odrLow &= ~B_SELECT;
	GPIO_E.odrLow |= x;
}
void graphic_ctrl_bit_clear(unsigned char x){
	GPIO_E.odrLow &= ~B_SELECT;
	GPIO_E.odrLow &= ~x;
}
void select_controller(unsigned char controller){
	switch (controller){
		case 0:
			graphic_ctrl_bit_clear(B_CS1);
			graphic_ctrl_bit_clear(B_CS2);
			break;
		case B_CS1:
			graphic_ctrl_bit_set(B_CS1);
			graphic_ctrl_bit_clear(B_CS2);
			break;
		case B_CS2:
			graphic_ctrl_bit_clear(B_CS1);
			graphic_ctrl_bit_set(B_CS2);
			break;
		case B_CS1 | B_CS2:
			graphic_ctrl_bit_set(B_CS1);
			graphic_ctrl_bit_set(B_CS2);
			break;
		//default:
			//printf(%s,"Nu skickade du nått konstigt till controller");
	}
	
}
void graphic_wait_ready(void){
	
	unsigned char c;
	graphic_ctrl_bit_clear(B_E);
	GPIO_E.moder =  0x00005555;
	graphic_ctrl_bit_clear(B_RS);
	graphic_ctrl_bit_set(B_RW);
	delay_500ns();
	
	while(1){
		graphic_ctrl_bit_set(B_E);
		delay_500ns();
		c = GPIO_E.idrHigh;
		graphic_ctrl_bit_clear(B_E);
		delay_500ns();
		if((c & 0x80) == 0)
			break;
		
	}
	graphic_ctrl_bit_set(B_E);
	GPIO_E.moder =  0x55555555;
	
}
unsigned char graphic_read(unsigned char controller){
	graphic_ctrl_bit_clear(B_E);
	GPIO_E.moder =  0x00005555;
	graphic_ctrl_bit_set(B_RS);
	graphic_ctrl_bit_set(B_RW);
	select_controller(controller);
	delay_500ns();
	graphic_ctrl_bit_set(B_E);
	delay_500ns();
	unsigned char RV = GPIO_E.idrHigh;
	graphic_ctrl_bit_clear(B_E);
	GPIO_E.moder = 55555555;
	if(controller == B_CS1){
		select_controller(B_CS1);
		graphic_wait_ready();
	}
	if(controller == B_CS2){
		select_controller(B_CS2);
		graphic_wait_ready();
	}
	return RV;
}
void graphic_write(unsigned char value,unsigned char controller){
	GPIO_E.odrHigh = value;
	select_controller(controller);
	delay_500ns();
	graphic_ctrl_bit_set(B_E);
	delay_500ns();
	graphic_ctrl_bit_clear(B_E);
	if(controller & B_CS1){
		select_controller(B_CS1);
		graphic_wait_ready();
	}
	if(controller & B_CS2){
		select_controller(B_CS2);
		graphic_wait_ready();
	}
	GPIO_E.odrHigh = 0;
	graphic_ctrl_bit_set(B_E);
	select_controller(0);
}
void graphic_write_command(unsigned char cmd,unsigned char controller){
	graphic_ctrl_bit_set(B_E);
	select_controller(controller);
	graphic_ctrl_bit_clear(B_RS);
	graphic_ctrl_bit_clear(B_RW);
	graphic_write(cmd, controller);
}
void graphic_write_data(unsigned char data,unsigned char controller){
	graphic_ctrl_bit_set(B_E);
	select_controller(controller);
	graphic_ctrl_bit_set(B_RS);
	graphic_ctrl_bit_clear(B_RW);
	graphic_write(data, controller);
}

unsigned char graphic_read_data(unsigned char controller){
	(void) graphic_read(controller); //första läsningen mislsyckas av nån anledning så kasta den
	return graphic_read(controller); //denna gången fungerar det
}

void graphic_initialize (void){
	graphic_ctrl_bit_set(B_E);
	delay_mikro(10);
	graphic_ctrl_bit_clear(B_CS1);
	graphic_ctrl_bit_clear(B_CS2);
	graphic_ctrl_bit_clear(B_RST);
	graphic_ctrl_bit_clear(B_E);
	delay_milli(30);
	graphic_ctrl_bit_set(B_RST);
	graphic_write_command(LCD_OFF,B_CS1|B_CS2);
	graphic_write_command(LCD_ON,B_CS1|B_CS2);
	graphic_write_command(LCD_DISP_START,B_CS1|B_CS2);
	graphic_write_command(LCD_SET_ADD,B_CS1|B_CS2);
	graphic_write_command(LCD_SET_PAGE,B_CS1|B_CS2);
	select_controller(0);
}
void graphics_clear_screen(void){
	for(int i=0; i<8;i++){
		graphic_write_command(LCD_SET_PAGE | i , B_CS1|B_CS2);
		graphic_write_command(LCD_SET_ADD, B_CS1|B_CS2);
		for(int j=0; j<64;j++){
			graphic_write_data(0,B_CS1|B_CS2);
		}
		
	}
}
void pixel(unsigned char x, unsigned char y,unsigned set){
	if(x>128 || x<1)
		return;
	if(y>64 || y<1)
		return;
	unsigned char index = (y-1)/8;
	unsigned char mask;
	switch((y-1)%8){
		case 0: mask = 0x01; break;
		case 1: mask = 0x02; break;
		case 2: mask = 0x04; break;
		case 3: mask = 0x08; break;
		case 4: mask = 0x10; break;
		case 5: mask = 0x20; break;
		case 6: mask = 0x40; break;
		case 7: mask = 0x80; break;
	}
	if(set==0)
		mask = ~mask;
		
	unsigned char x_fysisk;
	unsigned char controller;
	
	if(x>64){
		controller = B_CS2;
		x_fysisk = x-65;
	}
	else{
		controller = B_CS1;
		x_fysisk = x-1;
	}
	graphic_write_command(LCD_SET_ADD | x_fysisk, controller);
	graphic_write_command(LCD_SET_PAGE | index, controller);
	unsigned char temp = graphic_read_data(controller);
	graphic_write_command(LCD_SET_ADD|x_fysisk,controller);
	if(set==1)
		mask |= temp;
	else
		mask &= temp;
	graphic_write_data(mask,controller);
}