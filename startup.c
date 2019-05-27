/*
 * 	startup.c
 *
 */
 
#include <stdlib.h>
#include <stdio.h>
#include "displayfunk.h"
#include "doodlejump.h"
#include "AsciiDisplayFunk.h"
#include "keyb.h"

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
		20,
		4,4,
		{
			{0,0}, {1,0}, {2,0}, {3,0}, {4,0},
			{5,0}, {6,0}, {7,0}, {8,0}, {9,0},
			{10,0}, {11,0}, {12,0}, {13,0}, {14,0},
			{15,0}, {16,0}, {17,0}, {18,0}, {19,0}
		}
};

static unsigned char new_game_flag;
static unsigned char game_over_flag;
static unsigned char collision_flag;
static int score;
// värden för collision_flag
#define HOR_EDGE 1
#define VERT_EDGE 2

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
	for(int i=0;i<o->geo->numpoints;i++){
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
			o->diry = -(o->diry)-3;
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
	
		if(o->posx < 1){
			o->posx = 1;
			collision_flag = VERT_EDGE;
		}
	
		if((o->posx > (129-sizex))) {
			o->posx = 129;
			collision_flag = VERT_EDGE;
		}
		if(o->posy < 1){
			o->posy = 1;
			collision_flag = HOR_EDGE;
		}
	
		if(o->posy>(65-sizey))
			game_over_flag = 1;
	}
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

void init_game(POBJECT ball, POBJECT plat)
{
	char *s;
	char init_message1[] = "PRESS ANY KEY";
	char init_message2[] = "TO START!";
	score = 0;
	
	#ifndef SIMULATOR
		graphics_clear_screen();
	#endif 
	ascii_clear_display();
	ascii_gotoxy(1,1);
	s = init_message1;
	while(*s)
		ascii_write_char(*s++);
	ascii_gotoxy(1,2);
	s = init_message2;
	while(*s)
		ascii_write_char(*s++);
		
	//flags + score
	game_over_flag = 0;
	collision_flag = 0;
	new_game_flag = 1;
	score = 0;
	
	// initial position
	ball->posx = 40;
	ball->posy = 10;
	
	//initial hastighet
	ball->set_speed(ball, 3, 2);
	draw_platform(plat);
	
}

void main(void)
{
	char *s;
	char start_game_message1[] = "JUMP ON THE";
	char start_game_message2[] = "PLATFORMS, OR DIE!";
	
	POBJECT p = &ball;
	POBJECT plat = &platform;
	
	init_gpio();
	ascii_init();
	graphic_initialize();
	#ifndef SIMULATOR
		graphics_clear_screen();
	#endif
	
	init_game(p,plat);
	unsigned char c;
	while(1)
	{
		if(new_game_flag)
		{
			while(1){
				c = keyb();
				if(c != 0xff) break;
			}
			ascii_clear_display();
			s = start_game_message1;
			while(*s)
				ascii_write_char(*s++);
			ascii_gotoxy(1,2);
			s = start_game_message2;
			while(*s)
				ascii_write_char(*s++);
			new_game_flag = 0;
		}
		p->move(p);
		if(!collision_flag){
			collision_flag = platColDetect(p,plat,score);
			if(collision_flag)
			{
				score++;
				ascii_clear_display();
				ascii_write_char((char) score);
			}
		}
			
		draw_object(p);
		delay_milli(40);
		c=keyb();
		switch(c){
			case 3: 	p->set_speed(p,4,p->diry); break;
			case 1: 	p->set_speed(p,-4,p->diry); break;
		}
		
		if(game_over_flag)
		{
			drawGameOver(p);
			game_over_flag = 0;
			while(1)
			{
				c = keyb();
				if(c == 5) 
				{
					// initiera igen
					init_game(p,plat);
					break;
				}
			}
		}
	}
	
}




