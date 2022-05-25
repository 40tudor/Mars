/****************************************************************
 Ship Motion Test
*****************************************************************/

#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <dos.h>
#include <stdlib.h>

#define RADIUS 5
#define SHIP_COLOR 15
#define TEXT_COLOR 14
#define BG_COLOR 0
#define MAINTHRUST 0.02
#define SIDETHRUST 0.03
#define GRAVITY 0.004
#define ATMOSPHERE 0.00001
#define NUMPADS 5

void drawship(float x,float y);
void eraseship(float xold,float yold);
void drawStatusHeadings(void);
void updateFuel(int fuelSupply);
void updateTime(int eTime);
void readkeybd(float *ythrust,float *xthrust,int *fuelSupply,float *t);
extern void landingPad(float pad[NUMPADS][3],int maxPad);
extern void drawHills(float pad[NUMPADS][3],int maxHill);
extern int contact(float x, float y, int radius);
void crash(float x,float y);
void quit(void);

int sc=4;
main()
{

float vx=0.0,vy=0.0,ax=0.0,at=0.0,ythrust=0.0,vxold=0.0;
float x=0.0,y=0.0,xold=0.0,yold=0.0,xthrust=0.0,vyold=0.0;
float x0=0.0,y0=0.0,vx0=0.0,vy0=0.0,t=0.0;
int gdriver=DETECT, gmode, xmax, ymax;
int eTime=0,fuelSupply=10000,maxPad=200,maxHill=300;
float pad[NUMPADS][3];

initgraph(&gdriver, &gmode, "\d:\\bc45\\bgi");
xmax=getmaxx(); ymax=getmaxy();


moveto(0,50);
lineto(0,ymax);
lineto(xmax,ymax);
lineto(xmax, 50);
lineto(0,50);
landingPad(pad,maxPad);
drawHills(pad,maxHill);

x=xold=x0=RADIUS+10.0;
y=yold=y0=RADIUS+60.0;

drawStatusHeadings();
updateTime(eTime);
updateFuel(fuelSupply);

do
{

/****************************
Update Ship on Screen
*****************************/
	eraseship(xold,yold);
	drawship(x,y);
	updateTime(eTime);
	xold=x;yold=y;

/********************************************
 Calculate new X position
*********************************************/

if (vx>=ATMOSPHERE)
	{at = -ATMOSPHERE;}
if (vx<=-ATMOSPHERE)
	{at = ATMOSPHERE;}
if (vx>=-ATMOSPHERE && vx<=ATMOSPHERE)
	{at=0.0;}

if (vx >= 75)
{
vx=75;
xthrust=0.0;
}
if (vx <= -75)
{
vx=-75;
xthrust=0.0;
}
ax = (xthrust + at);
vx+=  (ax*t);

//x =(xold) + (vx) + (ax*t);
x=xold + vx+ ax*t*t;
//x = (x0) + (vx0*t) + (ax*t);


/********************************************
 Calculate new Y position
********************************************/
 vy+= GRAVITY + ythrust;
 if (vy >= 10)
	{
		vy=10;
	}
 // y= (vy*t) + (0.5*GRAVITY*t*t);
 y= yold +(vy) + (GRAVITY + ythrust)*t;
 //y=ymax/2;

/*****************************
  Detect Ship at borders
*****************************/
	if (y>=ymax-6)
		{y=ymax-6;}

	if (y<=56)
		{vy=0.0;
		ythrust=0.0;
		y=57;}

	if (x>=xmax-6)
		{x=8;}

	if (x<=6)
		{x=xmax-8;}

	if (contact(x,y,RADIUS)==1)
	{
		vx=0.0;
		vy=0.0;
		ax=0.0;
		ythrust=-1*GRAVITY;
//		crash(xold,yold);
	}

 t+=0.1;
 eTime+=1;

 if(eTime%5==0)
 {
	readkeybd(&ythrust, &xthrust, &fuelSupply, &t);
	if(t==0.0)
	{
	x0=x;
//	y0=y;
	vx0=vx;
//	vy0=vy;
	}
	updateFuel(fuelSupply);
 }
}

while(1);



//closegraph();

return(0);
}
/*****************************************

 ************     Functions       ********

 ******************************************/

void drawship(float x,float y)
{
	setcolor(14);
	moveto(x+3,y-1);
	lineto(x-3,y-1);
	lineto(x,y-5);
	lineto(x+4,y-1);
	lineto(x+4,y+3);
	lineto(x-4,y+3);
	lineto(x-4,y-1);
	moveto(x+3,y+3);
	lineto(x+5,y+5);
	moveto(x-3,y+3);
	lineto(x-5,y+5);
	delay(4);

return;
}

void eraseship(float x,float y)
{
	setcolor(getbkcolor());
	moveto(x+3,y-1);
	lineto(x-3,y-1);
	lineto(x,y-5);
	lineto(x+4,y-1);
	lineto(x+4,y+3);
	lineto(x-4,y+3);
	lineto(x-4,y-1);
	moveto(x+3,y+3);
	lineto(x+5,y+5);
	moveto(x-3,y+3);
	lineto(x-5,y+5);

	return;
}

void drawStatusHeadings(void)
{
	char blank[6]={'\xDB','\xDB','\xDB','\xDB','\xDB','\0'};

	setcolor(BG_COLOR);
	outtextxy(110,20,blank);
	outtextxy(210,20,blank);

	setcolor(TEXT_COLOR);
	outtextxy(110,20,"Fuel= ");
	outtextxy(210,20,"Time= ");
	setcolor(SHIP_COLOR);

	return;
}

void updateFuel(int fuelSupply)
{
	char fs[6],et[6];
	char blank[6]={'\xDB','\xDB','\xDB','\xDB','\xDB','\0'};

	setcolor(BG_COLOR);
	outtextxy(160,20,blank);
//	outtextxy(260,20,blank);

	setcolor(TEXT_COLOR);
	outtextxy(160,20,itoa(fuelSupply,fs,10));
//	outtextxy(260,20,itoa(eTime,et,10));
	setcolor(SHIP_COLOR);

	return;
}

void updateTime(int eTime)
{
	char fs[6],et[6];
	char blank[6]={'\xDB','\xDB','\xDB','\xDB','\xDB','\0'};

	setcolor(BG_COLOR);
//	outtextxy(160,20,blank);
	outtextxy(260,20,blank);

	setcolor(TEXT_COLOR);
//	outtextxy(160,20,itoa(fuelSupply,fs,10));
	outtextxy(260,20,itoa(eTime,et,10));
	setcolor(SHIP_COLOR);

	return;
}

/********************************************************
Function to get the keyboard input
********************************************************/

void readkeybd(float*ythrust,float*xthrust,int*fuelSupply,float*t)
{

 if(kbhit())
	{
		switch(getch())
		{
		case('i'):
			{
			if (*fuelSupply>0)
				{
				if (*ythrust<MAINTHRUST)
					{*t=0.0;
					}
				*ythrust = -MAINTHRUST;
				*fuelSupply -= 5;
				}

			break;
			}
		case('l'):
			{
			if (*fuelSupply>0)
				{
				if (*xthrust<SIDETHRUST)
					{*t=0.0;}
				*xthrust = SIDETHRUST;
				*fuelSupply -= 1;
				}

			break;
			}
		case('j'):
			{
			if (*fuelSupply>0)
				{
				if (*xthrust>-SIDETHRUST)
					{*t=0.0;}
				*xthrust = -SIDETHRUST;
				*fuelSupply -= 1;
				}

			break;
			}
		case('q'):
			{
				quit();
				break;
			}
		 }
	 }

 else
	{
	*ythrust=0.0;
	*xthrust=0.0;
	}


return;
}

void crash(float x,float y)
{
	eraseship(x,y);
	setcolor(YELLOW);
	outtextxy(x-20,y-10,"Crash!!");
	getch();
	quit();
}

void quit(void)
{
	closegraph();
	exit(1);
}
