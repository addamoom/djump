#ifndef TYPES_H
#define TYPES_H
//typer här

#define MAX_POINTS (unsigned char) 20

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

typedef struct tPoint
{
	unsigned char x;
	unsigned char y;
}POINT;

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

typedef struct colLine
{
	int numpoints;
	POINT px[ MAX_POINTS ];
} LINE, *PLINE;

#endif