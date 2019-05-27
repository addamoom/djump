#include <stdlib.h>
#include <stdio.h>
#include "doodlejump.h"
#include "displayfunk.h"
#include "types.h"

void draw_platform(POBJECT o,int score){
	
	POINT p;

	for(int i=0;i<20-score;i++){
		p = o->geo->px[i];
		pixel(o->posx+p.x,o->posy+p.y,1); //bör försöka lösa implicit varningen här genom att flytta runt functionerna (funkar dock ändå eftersom pixel returnerar void)
	}
}

void genNewPlatform(POBJECT plat,int score){
	//this function moves the platform to a new position
	POINT positions[] = {{30,50}, {90,40}, {60,45}, {15,50},{80,55}, {60,40}, {20,30}, {70,50}};
	int i = score%8;
	
	plat->posx = (int) positions[(int) i].x;
	plat->posy = (int) positions[(int) i].y;
	plat->clear(plat);
	draw_platform(plat, inte score);
	
	
}
struct colLine calcLine(POINT curr, POINT next){
	
	LINE temp;
	int i = 0;				
	int dx = next.x-curr.x;
	int dy = next.y-curr.y;
	int x = curr.x; 			
	int y = curr.y;				
	
	int p=2*dy-dx;
	
	while(x<next.x){
		temp.px[i].x =	x;	
		temp.px[i].y =  y;
		pixel(x,y,1);	//Rita linjen i debug syfte, ta bort sen
		i++;
		
		if(p>=0){
			y++;
			p=p+2*dy-2*dx;
		}
		else
			p=p+2*dy;
		x++;
	}
	temp.numpoints = i;
	
	return temp;
	
	
	/*int i = 0;
	LINE temp;
	int x = curr.x; 			
	int y = curr.y;
	
	int m = 2*(next.y-curr.y);
	int err = m - (next.x-curr.x);
	
	for(;x<=next.x;x++){
		temp.px[i].x =	x;	
		temp.px[i].y =  y;
		i++;
		pixel(x,y,1);	//Rita linjen i debug syfte, ta bort sen
		err += m;
		
		if(err >=0){
			y++;
			err -= 2*(next.x-curr.x);
		}
	}
	temp.numpoints = i;
	
	return temp;*/
}

unsigned char platColDetect(POBJECT ball, POBJECT platform,int score);{
	
	/*POINT curr;
	curr.x=ball->posx;
	curr.y=ball->posy;
	
	POINT next;
	next.x= ball->posx+ball->dirx;
	next.y= ball->posy+ball->diry;
	
	//LINE ballLine;
	

	ballLine = calcLine(curr,next);*/
	
	for(int i =0;i<platform->geo->numpoints-score;i++){
		for(int j =0;j<ball->geo->sizex;j++){
			for(int k =0;k<ball->geo->sizey;k++){
				if((ball->posx+j==(platform->posx+i)) & (ball->posy+k==platform->posy)){
					clear_object(platform);
					genNewPlatform(platform, score);
					return 1;
				}
			}
		}
	}
	return 0;
}
void doodleAcceleration (POBJECT o) {
	if(o->diry < 8 )
	{
		o->diry += 1;
	}
	return;
}
void drawGameOver(POBJECT o) {
	char *s; 
	char game_over_mess1[] = "GAME OVER!";
	char game_over_mess2[] = "PRESS 5 TO RETRY.";
	
	for(int i=0; i<8;i++){
		graphic_write_command(LCD_SET_PAGE | i , B_CS1|B_CS2);
		graphic_write_command(LCD_SET_ADD, B_CS1|B_CS2);
		for(int j=0; j<64;j++){
			graphic_write_data(0xff,B_CS1|B_CS2);
		}
		
	}
	
	ascii_clear_display();
	ascii_gotoxy(1,1);
	s = game_over_mess1;
	while(*s)
		ascii_write_char(*s++);
	ascii_gotoxy(1,2);
	s = game_over_mess2;
	while(*s)
		ascii_write_char(*s++);
}