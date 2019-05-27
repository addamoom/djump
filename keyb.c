#include "types.h"
#include <stdlib.h>
#include "keyb.h"

#define GPIO_D (*((volatile GPIO*) 0x40020C00))

void ActivateRow(int row)
{
	switch (row) 
	{
		case 1:
			GPIO_D.odrHigh &= 0b00001111;
			GPIO_D.odrHigh |= 0b00010000;
			break; 
		case 2:
			GPIO_D.odrHigh &= 0b00001111;
			GPIO_D.odrHigh |= 0b00100000;
			break;
		case 3:
			GPIO_D.odrHigh &= 0b00001111;
			GPIO_D.odrHigh |= 0b01000000;
			break;
		case 4:
			GPIO_D.odrHigh &= 0b00001111;
			GPIO_D.odrHigh |= 0b10000000;
			break;
		default:
			GPIO_D.odrHigh &= 0b00001111;
			GPIO_D.odrHigh |= 0x0;
			break;
	}
}

int ReadColumn(void)
{
	unsigned char c;
	unsigned char mask = 0x01;
	c = GPIO_D.idrHigh;
	
	for(int i = 1; i <= 4; i++)
	{
		if(c&mask) 
		{ 
			return i; 
		}
		mask = mask << 1; 
	} 
	return 0; 
}

unsigned char keyb(void)
{
	unsigned char keyValue[] = {1,2,3,10,4,5,6,11,7,8,9,12,14,0,15,13};
	int row = 1; 
	int column;
	
	while(row <= 4)
	{
		ActivateRow(row);
		column = ReadColumn();
		if (column) 
		{
			ActivateRow(0);
			return keyValue[(4*(row-1)+column)-1];
		}
		row++; 
	}
	return 0xff;
}