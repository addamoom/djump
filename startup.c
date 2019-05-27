/*
 * 	startup.c
 *
 */
 
#include <stdlib.h>
#include <stdio.h>
#include "displayfunk.h"
#include "doodlejump.h"

void startup(void) __attribute__((naked)) __attribute__((section (".start_section")) );

void startup ( void )
{
__asm volatile(
	" LDR R0,=0x2001C000\n"		/* set stack */
	" MOV SP,R0\n"
	" BL main\n"				/* call main */
	".L1: B .L1\n"				/* never return */
	) ;
}

#define SIMULATOR 1

GEOMETRY ball_geometry =
{
		12,
		4,4,
		{
			{0,1}, {0,2}, {1,0}, {1,1}, {1,2},
			{1,3}, {2,0}, {2,1}, {2,2}, {2,3},
			{3,1},
			{3,2}
		}
};
GEOMETRY plat_geometry =
{
		10,
		4,4,
		{
			{0,0}, {1,0}, {2,0}, {3,0}, {4,0},
			{5,0}, {6,0}, {7,0}, {8,0}, {9,0}
		}
};


/*#define stick (*((volatile systick *) 0xE000E010))
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
#define LCD_BUSY (unsigned char) 0x80*/

static unsigned char game_over_flag;
static unsigned char collision_flag;
// värden för collision_flag
#define HOR_EDGE 1
#define VERT_EDGE 2

int score;


void set_object_speed(POBJECT o,int speedx,int speedy)
{
	o->dirx = speedx;
	o->diry = speedy;
}
void draw_object(POBJECT o){
	
	POINT p;

	for(int i=0;i<12;i++){
		p = o->geo->px[i];
		pixel(o->posx+p.x,o->posy+p.y,1);
	}
}
void clear_object(POBJECT o){
	POINT p;
	for(int i=0;i<12;i++){
		p = o->geo->px[i];
		pixel(o->posx+p.x,o->posy+p.y,0);
	}
}
void move_object(POBJECT o){
	int sizex, sizey;	
	clear_object(o);
	
	if(collision_flag)
	{
		if(collision_flag == HOR_EDGE)
		{
			o->diry = -(o->diry)-5;
			o->posy += o->diry;
		}
		if(collision_flag == VERT_EDGE)
		{
			o->dirx = -(o->dirx);
			o->posx += o->dirx;
		}
		collision_flag = 0;
	} else {
		doodleAcceleration(o);
		o->posx += o->dirx; 
		o->posy	+= o->diry;
		sizex = o->geo->sizex;
		sizey = o->geo->sizey;
	
		if(o->posx<1){
			o->posx = 1;
			collision_flag = VERT_EDGE;
		}
	
		if((o->posx > (129-sizex))) {
			o->posx = 128;
			collision_flag = VERT_EDGE;
		}
		if(o->posy<1){
			o->posy = 1;
			collision_flag = HOR_EDGE;
		}
	
		if(o->posy>(65-sizey))
			drawGameOver(o);
	}
		draw_object(o);
}

static OBJECT ball ={
	&ball_geometry,
	0,0,
	50,40, // initial position
	draw_object,
	clear_object,
	move_object,
	set_object_speed
};
static OBJECT platform ={
	&plat_geometry,
	0,0,
	50,50,
	draw_object,
	clear_object,
	move_object,
	set_object_speed
};			

void main(void)
{
	POBJECT p = &ball;
	POBJECT plat = &platform;
	unsigned char key_press;
	
	init_gpio();
	graphic_initialize();
	#ifndef SIMULATOR
		graphics_clear_screen();
	#endif
<<<<<<< HEAD
	
	game_over_flag = 0;
	collision_flag = 0;
	p->set_speed(p, 6, 2);
=======
	p->set_speed(p, 3, 3);
>>>>>>> 1e9869aa7172fe1220982b5b300188af67844b57
	plat->draw(plat);
	while(1)
	{
		p->move(p);
<<<<<<< HEAD
		delay_milli(40);
		
		if(game_over_flag)
		{
			drawGameOver(p);
			while(1)
			{
				//vänta på keyb
			}
			
		}
=======
		platColDetect(p,plat);
		
		delay_milli(40);
		
		//if(win){
		//	draw_gameover()
		//	while (key a not presed){inget}
		//	intitializera
		//	win=0
		//}
>>>>>>> 1e9869aa7172fe1220982b5b300188af67844b57
	}
	
}




