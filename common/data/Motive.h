#ifndef __MOTIVE_H__
#define __MOTIVE_H__


enum Motive
{
	Motive_Birth = 'B',
	Motive_Buy = '+',
	Motive_Sell = '-',
	Motive_Import = 'I',
	Motive_Export = 'E',
	Motive_Death = 'D',
	Motive_Kill = 'K',
	Motive_Slaugher = 'S',
	Motive_None = '\0'
};

enum Motive_Direction
{
	MotiveDirection_In = 1,
	MotiveDirection_Out = -1,
	MotiveDirection_None  = 0
};


#endif