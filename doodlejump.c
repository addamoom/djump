#include <stdlib.h>
#include <stdio.h>
#include "doodlejump.h"
#include "types.h"

extern int score;

void draw_platform(POBJECT o){
	
	POINT p;

	for(int i=0;i<10;i++){
		p = o->geo->px[i];
		pixel(o->posx+p.x,o->posy+p.y,1); //bör försöka lösa implicit varningen här genom att flytta runt functionerna (funkar dock ändå eftersom pixel returnerar void)
	}
}

void genNewPlatform(POBJECT plat, char i){
	//this function moves the platform to a new position
	POINT positions[] = {{50,7}, {50,15}, {40,36}, {45,47}};
	
	i = i%4;
	
	plat->posx = (int) positions[(int) i].x;
	plat->posy = (int) positions[(int) i].y;
	plat->clear(plat);
	draw_platform(plat);
	
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

void platColDetect(POBJECT ball, POBJECT platform){
	
	POINT curr;
	curr.x=ball->posx;
	curr.y=ball->posy;
	
	POINT next;
	next.x= ball->posx+ball->dirx;
	next.y= ball->posy+ball->diry;
	
	LINE ballLine;
	
	ballLine = calcLine(curr,next);
	
	for(int i =0;i<10;i++){
		if((ball->posx==(platform->posx+i)) & (ball->posy==platform->posy)){
			ball->set_speed(ball, ball->dirx, -ball->diry+2);
			genNewPlatform(platform, score);
			
		}
	}
}
void doodleAcceleration (POBJECT o) {
	if(o->diry < 8 )
	{
		o->diry += 1;
	}
	return;
}
void drawGameOver(POBJECT o) {
	// tillfällig
	o->diry = -(o->diry);
	o->posy += o->diry;
}