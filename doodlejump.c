#include <stdlib.h>
#include <stdio.h>
typedef struct tPoint
{
	unsigned char x;
	unsigned char y;
}POINT;

#define MAX_POINTS (unsigned char) 20
typedef struct tGeometry
{
	int numpoints;
	int sizex;
	int sizey;
	POINT px[ MAX_POINTS ];
} GEOMETRY, *PGEOMETRY;

typedef struct tObj{
	PGEOMETRY geo;
	int dirx,diry;
	int posx,posy;
	void (* draw) (struct tObj *);
	void (* clear ) (struct tObj *);
	void (* move ) (struct tObj *);
	void (* set_speed ) (struct tObj *,int,int);
} OBJECT, *POBJECT;

void genNewPlatform(POBJECT plat, char i){
	//this function moves the platform to a new position
	POINT positions[] = {{50,7}, {50,15}, {40,36}, {45,47}};
	
	i = i%4;
	
	plat->posx = positions[i].x;
	plat->posy = positions[i].y;
	
	draw_platform(plat);
	
}
void draw_platform(POBJECT o){
	
	POINT p;

	for(int i=0;i<10;i++){
		p = o->geo->px[i];
		pixel(o->posx+p.x,o->posy+p.y,1);
	}
}
void platColDetect(void){
	
}
void doodleAcceleration (POBJECT o) {
	if(o->diry > -8 )
	{
		o->diry -= 1;
	}
	return;
}
void game_over(POBJECT o) {
	// tillfällig
	o->diry = ~(o->diry);
	o->posy += o->diry;
}