#include <stdlib.h>
#include <stdio.h>

void genNewPlatform(void){
	//this function moves the platform to a new position
	POINT positions[] = {{50,}, {0,2}, {1,0}, {1,1}, {1,2},{1,3}, {2,0}, {2,1}, {2,2}, {2,3},{3,1},{3,2};
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