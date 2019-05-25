#include <stdlib.h>
#include <stdio.h>
#include "doodlejump.h"
#include "types.h"

extern int score;

void draw_platform(POBJECT o){
	
	POINT p;

	for(int i=0;i<10;i++){
		p = o->geo->px[i];
		pixel(o->posx+p.x,o->posy+p.y,1); //bör försöka lösa implicit varningen här genom att flytta runt functionerna
	}
}

void genNewPlatform(POBJECT plat, char i){
	//this function moves the platform to a new position
	POINT positions[] = {{50,7}, {50,15}, {40,36}, {45,47}};
	
	i = i%4;
	
	plat->posx = (int) positions[i].x;
	plat->posy = (int) positions[i].y;
	
	draw_platform(plat);
	
}

void platColDetect(POBJECT ball, POBJECT platform){
	for(int i =0;i<10;i++){
		if((ball->posx==(platform->posx+i)) & (ball->posy==platform->posy)){
			ball->set_speed(ball, ball->dirx, ball->diry+2);
			genNewPlatform(platform, score);
			
		}
	}
}
void doodleAcceleration (POBJECT o) {
	if(o->diry > -8 )
	{
		o->diry += 1;
	}
	return;
}
void game_over(POBJECT o) {
	// tillfällig
	o->diry = ~(o->diry);
	o->posy += o->diry;
}