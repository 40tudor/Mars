/****************************************************************
 Ship Motion Test
*****************************************************************/

#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>

#define RADIUS 5
#define SHIP_COLOR 15
#define TEXT_COLOR 14
#define BG_COLOR 0
#define MAINTHRUST 0.04
#define SIDETHRUST 0.03
#define GRAVITY 0.004
#define ATMOSPHERE 0.0001
#define NUMPADS 5

void drawBorder();
void drawship(float x,float y);
void eraseship(float xold,float yold);
void drawStatusHeadings(void);
void updateFuel(int fuelSupply);
void updateVelocity(int vy,int vx);
void readkeybd(float *ythrust,float *xthrust,int *fuelSupply,float *t);
void crash(float x,float y);
int congrats();
void quit();
extern void landingPad(float pad[NUMPADS][3],int maxPad);
extern void drawHills(float pad[NUMPADS][3],int maxHill);
extern int contact(float x, float y, int radius);

int sc=4;
main()
{
	float vx=0.0,vy=0.0,ax=0.0,at=0.0,ythrust=0.0;
	float x=0.0,y=0.0,xold=0.0,yold=0.0,xthrust=0.0;
	float t=0.0;
	int gdriver=DETECT, gmode, xmax, ymax;
	int eTime=0,fuelSupply=10000,maxPad=150,maxHill=450;
	int level=1;
	float pad[NUMPADS][3];

startOver:
	initgraph(&gdriver, &gmode, "\e:\\bc45\\bgi");
	drawBorder();
	landingPad(pad,maxPad);
	drawHills(pad,maxHill);
	drawStatusHeadings();
	updateVelocity(vy,vx);
	updateFuel(fuelSupply);

	x=xold=RADIUS+10.0;
	y=yold=RADIUS+60.0;

	do
	{

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

		if (contact(x,y,5)==RED)
		{
			vx=0.0;
			vy=0.0;
			ax=0.0;
			y=yold;
			t=0.0;
			crash(x,y);
		//	delay(1000);
		}
		if (contact(x,y,5)==CYAN)
		{
			vx=0.0;
			vy=0.0;
			ax=0.0;
			y=yold;
			t=0.0;
			if (congrats()==1)
			{
				maxHill-=50;
				maxPad+=50;
				x=xold=RADIUS+10.0;
				y=yold=RADIUS+60.0;
				closegraph();
				goto startOver;
			}
		}
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
if (vx >= 75.0)
	{
	vx=75.0;                /*check for max speed*/
	xthrust=0.0;
	}
if (vx <= -75.0)
	{
	vx=-75.0;
	xthrust=0.0;
	}
at = -2.0 * ATMOSPHERE * vx*vx;
if (vx < 0.0)
	{
	at*=-1.0;
	}
ax = (xthrust + at);
vx+=  (ax*t);
x=xold + vx+ ax*t*t;


/********************************************
 Calculate new Y position
********************************************/
 vy+= GRAVITY + ythrust;
 y= yold +(vy) + (GRAVITY + ythrust)*t;

 t+=0.1;
 eTime+=1;

 if(eTime%5==0)
 {
	readkeybd(&ythrust, &xthrust, &fuelSupply, &t);
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
	outtextxy(210,20,"Velocity= ");
	setcolor(SHIP_COLOR);

	return;
}

void updateFuel(int fuelSupply)
{
	char fs[6];
	char blank[6]={'\xDB','\xDB','\xDB','\xDB','\xDB','\0'};

	setcolor(BG_COLOR);
	outtextxy(160,20,blank);

	setcolor(TEXT_COLOR);
	outtextxy(160,20,itoa(fuelSupply,fs,10));
	setcolor(SHIP_COLOR);

	return;
}

void updateVelocity(int vy,int vx)
{
	char vely[6],velx[6];
	char blank[6]={'\xDB','\xDB','\xDB','\xDB','\xDB','\0'};

	setcolor(BG_COLOR);
	outtextxy(260,20,blank);
//	outtextxy(360,20,blank);

	setcolor(TEXT_COLOR);
	outtextxy(260,20,itoa(vy,vely,10));
//	outtextxy(360,20,itoa(vx,velx,10));
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
			if (fuelSupply>0)
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
			if (fuelSupply>0)
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
			if (fuelSupply>0)
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

/*
Crash Function
*/
void crash(float x,float y)
{
double alpha1=2.3,alpha2=0.17,alpha3=1.48;
double alpha4=2.96,alpha5=0.78,alpha6=3.32;
float a=0,b=0,c=0,d=0;
double theta[12]={0.1,0.5,0.75,1.0,1.25,1.5,1.75,2.0,2.25,2.5,2.75,3.0};
int i=0,k=0;

for (i=0;i<90;i++)
	{
	 putpixel((x+i*cos(alpha1)),(y-i*sin(alpha1)),2);
	 putpixel((x+(i-2)*cos(alpha1)),(y-(i-2)*sin(alpha1)),0);
	 putpixel((x+i*cos(alpha2)),(y-i*sin(alpha2)),5);
	 putpixel((x+(i-2)*cos(alpha2)),(y-(i-2)*sin(alpha2)),0);

	for (k=0;k<12;k++)
		{setcolor(14);
		moveto((x+i*cos(alpha3))+4*cos(theta[k]),(y-i*sin(alpha3))+4*sin(theta[k]));
		lineto((x+i*cos(alpha3))-4*cos(theta[k]),(y-i*sin(alpha3))-4*sin(theta[k]));
		setcolor(0);
		moveto((x+i*cos(alpha3))+4*cos(theta[k]),(y-i*sin(alpha3))+4*sin(theta[k]));
		lineto((x+i*cos(alpha3))-4*cos(theta[k]),(y-i*sin(alpha3))-4*sin(theta[k]));
		}

	 putpixel((x+i*cos(alpha4)),(y-i*sin(alpha4)),15);
	 putpixel((x+(i-3)*cos(alpha4)),(y-(i-3)*sin(alpha4)),0);


	for (k=0;k<12;k++)
		{setcolor(14);
		moveto((x+i*cos(alpha5))+5*cos(theta[k]),(y-i*sin(alpha5))+5*sin(theta[k]));
		lineto((x+i*cos(alpha5))-5*cos(theta[k]),(y-i*sin(alpha5))-5*sin(theta[k]));
		setcolor(0);
		moveto((x+i*cos(alpha5))+5*cos(theta[k]),(y-i*sin(alpha5))+5*sin(theta[k]));
		lineto((x+i*cos(alpha5))-5*cos(theta[k]),(y-i*sin(alpha5))-5*sin(theta[k]));
		}

	for (k=0;k<12;k++)
		{setcolor(14);
		moveto((x+i*cos(alpha6))+4*cos(theta[k]),(y-i*sin(alpha6))+4*sin(theta[k]));
		lineto((x+i*cos(alpha6))-4*cos(theta[k]),(y-i*sin(alpha6))-4*sin(theta[k]));
		setcolor(0);
		moveto((x+i*cos(alpha6))+4*cos(theta[k]),(y-i*sin(alpha6))+4*sin(theta[k]));
		lineto((x+i*cos(alpha6))-4*cos(theta[k]),(y-i*sin(alpha6))-4*sin(theta[k]));
		}
	}

return;
}

int congrats()
{
	int xmax=getmaxx();
	int ymax=getmaxy();

	setcolor(7);
	outtextxy(xmax/3.5,ymax/3,"Congrats!!  You landed safely.");
	outtextxy(xmax/3,ymax/3+20,"Continue to next level?");
	if (getch()=='Y'||getch()=='y')
	{
		return(1);
	}
	else
	{
		(quit());
	}
	return(0);
}

void drawBorder()
{
	int xmax=getmaxx(), ymax=getmaxy();
   setcolor(WHITE);
	moveto(0,50);
	lineto(0,ymax);
	lineto(xmax,ymax);
	lineto(xmax, 50);
	lineto(0,50);
}
void quit()
{
	closegraph();
	exit(1);
}
