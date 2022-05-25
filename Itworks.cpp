/****************************************************************
*EE 304 Spring Semester 1995 UNIVERSITY OF NORTH DAKOTA
*
*Final Project
*
*Names: William Hovey & Christopher Young
*
*
*Title:  Final Project
*
*Date Completed:   5/23/95
*
*Date Due:  ?/?/95
*
*Lab description:  This is a game in which the player attempts to
*						  land a spacecraft on mars with limited fuel and
*							given landing pads
*
*
*File Name: itworks.cpp
*****************************************************************/

/*  Modified MAINTHRUST from .033
 *				 SIDETHRUST from .03
 *				 GRAVITY		from .004
 *				 gravity increment in main() from .0004 (line ~196)
 *
 *				 CDY 5/2/07
 */

#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <ctype.h>

/****************************************
 Game parameter constants
****************************************/
#define SHIP_COLOR 15
#define TEXT_COLOR 14
#define BG_COLOR 0
#define MAINTHRUST 0.023
#define SIDETHRUST 0.02
#define GRAVITY 0.002
#define ATMOSPHERE 0.0001
#define NUMPADS 5
#define MAXFUEL 500
#define MAXPAD 250
#define MAXHILL 450

/**********************************
Function Protoypes
***********************************/
void intro(void);
void drawBorder();
void drawship(float x,float y);
void eraseship(float xold,float yold);
void drawStatusHeadings(void);
void updateFuel(int fuelSupply);
void updateLevel(int level);
void updateVelocity(float vy,float vx);
void readkeybd(float *ythrust,float *xthrust,int *fuelSupply,float *t);
void crash(float x,float y);
int congrats();
void quit();
int tryAgain();
extern void landingPad(float pad[NUMPADS][3],int maxPad);
extern void drawHills(float pad[NUMPADS][3],int maxHill);
extern int contact(float x, float y, int radius);

main()
{
/**********************************************
Variable decleration for main function
**********************************************/
	float vx=0.0,vy=0.0,ax=0.0,at=0.0,ythrust=0.0;
	float x=0.0,y=0.0,xold=0.0,yold=0.0,xthrust=0.0;
	float t=0.0,gravity=GRAVITY,atmosphere=ATMOSPHERE;
	int gdriver=DETECT, gmode, xmax, ymax;
	int eTime=0,fuelSupply=MAXFUEL,maxPad=MAXPAD,maxHill=MAXHILL,level=1;
	float pad[NUMPADS][3];

	if (registerfarbgidriver(EGAVGA_driver_far) < 0) exit(1);
	if (registerfarbgidriver(CGA_driver_far) < 0) exit(1);
	if (registerfarbgidriver(Herc_driver_far) < 0) exit(1);
	if (registerfarbgidriver(ATT_driver_far) < 0) exit(1);
	if (registerfarbgidriver(PC3270_driver_far) < 0) exit(1);
	if (registerfarbgidriver(IBM8514_driver_far) < 0) exit(1);
	if (registerfarbgidriver(ATT_driver_far) < 0) exit(1);
	if (registerbgifont(gothic_font) < 0) exit(1);


/************************************************
Initialize graphics
*************************************************/
	initgraph(&gdriver, &gmode, "");
	xmax=getmaxx(); ymax=getmaxy();

/************************************************
Create Title screen and Game instructions
*************************************************/
	drawBorder();
	intro();

/*************************************************
Reinitialize graphics and draw game screen
**************************************************/
	startOver:
	initgraph(&gdriver, &gmode, "");

	drawBorder();
	landingPad(pad,maxPad);
	drawHills(pad,maxHill);

	drawStatusHeadings();
	updateVelocity(vy,vx);
	updateFuel(fuelSupply);
	updateLevel(level);


/**************************************************
Set start position for the spaceship
***************************************************/
	x=xold=15.0;
	y=yold=65.0;

/***************************************************
Begin main loop
***************************************************/
	do
	{

/**********************************************************
Detect ship at borders and either wrap ship to other side
of the screen or stop it from going off of the top or bottom
***********************************************************/
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

/*****************************************************************
Detects the ships comtact with the terrain or if the ship has a
hard landing then blows up the ship if necessary.
*******************************************************************/
		if ((contact(x,y,5)==RED)||((contact(x,y,5)==CYAN)&&(sqrt(vx*vx+vy*vy)>=0.5)))
		{
			vx=0.0;
			vy=0.0;
			ax=0.0;
			y=yold;
			t=0.0;
			crash(x,y);
			if (tryAgain())
			{
				level=1;
				maxHill=MAXHILL;
				maxPad=MAXHILL;
				gravity=GRAVITY;
				atmosphere=ATMOSPHERE;
				fuelSupply=MAXFUEL;
				goto startOver;
			}
			else
			{
				quit();
			}
		}

/************************************************************
Detects the ships successful landing and congratulates the
player.
*************************************************************/
		if (contact(x,y,5)==CYAN && sqrt(vy*vy+vx*vx)<0.5)
		{
			vx=0.0;
			vy=0.0;
			ax=0.0;
			y=yold;
			t=0.0;
			if (congrats()==1)
			{
				maxHill-=60;
				gravity+=.0002;
				atmosphere+=.00001;
				fuelSupply=MAXFUEL;
				level++;
				closegraph();
				goto startOver;
			}
			else
			{
				quit();
			}
		}

/***********************************************************
 Update Ship on Screen
************************************************************/
	eraseship(xold,yold);
	drawship(x,y);
	updateVelocity(vy,vx);
	xold=x;yold=y;

/***********************************************************
 Check for max horixontal speed and limit it
************************************************************/
	if (vx >= 75.0)
		{
			vx=75.0;
			xthrust=0.0;
		}
	if (vx <= -75.0)
		{
			vx=-75.0;
			xthrust=0.0;
		}

/***********************************************************
Calcualate horizontal acceleration, velocity and position
************************************************************/
	at = -2.0 * atmosphere * vx*vx;
	if (vx < 0.0)
		{
			at*=-1.0;
		}
	ax = (xthrust + at);
	vx+=  (ax*t);
	x=xold + vx+ ax*t*t;

/****************************************************
 Calculate vertical velocity and position
*****************************************************/
	vy+= gravity + ythrust;
	y= yold +(vy) + (gravity + ythrust)*t;


/****************************************************
Increment game time and motion time variables
*****************************************************/
	t+=0.1;
	eTime+=1;

/********************************************************
This gets keyboard input and updates the fuel supply on
screen every five times throught the main loop
*********************************************************/
	if(eTime%5==0)
	{
		readkeybd(&ythrust, &xthrust, &fuelSupply, &t);
		updateFuel(fuelSupply);
	}
	}

	while(eTime>-1);

	return(0);
}

/*****************************************
******************************************
*************     Functions       ********
******************************************
 *****************************************/


/*******************************************************************
This function draws a yellow spaceship at location X,Y on the screen
********************************************************************/
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

/*******************************************************************
This function draws a black spaceship at location X,Y on the screen
********************************************************************/
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

/**************************************************************
This Function prints the game status header at the top of the
screen.
****************************************************************/

void drawStatusHeadings(void)
{
	char blank[6]={'\xDB','\xDB','\xDB','\xDB','\xDB','\0'};

	setcolor(BG_COLOR);
	outtextxy(110,20,blank);
	outtextxy(210,20,blank);

	setcolor(TEXT_COLOR);
	outtextxy(110,20,"Fuel= ");
	outtextxy(210,20,"Velocity= ");
	outtextxy(400,20,"Level= ");
	setcolor(SHIP_COLOR);

	return;
}

/**************************************************************
This function displays the current fuel level in the header
****************************************************************/
void updateFuel(int fuelSupply)
{
	char fs[6];
	char blank[6]={'\xDB','\xDB','\xDB','\xDB','\xDB','\0'};

	settextstyle (DEFAULT_FONT,HORIZ_DIR,1);
	setcolor(BG_COLOR);
	outtextxy(160,20,blank);

	setcolor(TEXT_COLOR);
	outtextxy(160,20,itoa(fuelSupply,fs,10));
	setcolor(SHIP_COLOR);

	return;
}

/*****************************************************************
This function displays the current absolute velocity in the header
*****************************************************************/
void updateVelocity(float vy,float vx)
{
	char absVelocity[10];
	char blank[6]={'\xDB','\xDB','\xDB','\xDB','\xDB','\0'};

	setcolor(BG_COLOR);
	outtextxy(300,20,blank);

	setcolor(TEXT_COLOR);
	sprintf(absVelocity,"%.2f",(sqrt(vy*vy+vx*vx)));
	outtextxy(300,20,absVelocity);
	setcolor(SHIP_COLOR);

	return;
}

/*****************************************************************
This function displays the current game level in the header
*****************************************************************/
void updateLevel(int level)
{
	char lvl[6];
	char blank[6]={'\xDB','\xDB','\xDB','\xDB','\xDB','\0'};

	setcolor(BG_COLOR);
	outtextxy(450,20,blank);

	setcolor(TEXT_COLOR);
	outtextxy(450,20,itoa(level,lvl,10));
	setcolor(SHIP_COLOR);

	return;
}

/********************************************************
Function to get the keyboard input
- If a thrust key (i,j,or l) is hit the appropriate thrust
variable is set and the fuel is decremented the appropriate
amount.
- If q is hit the game terminates
- If no key is hit all thrust variables are set to zero
********************************************************/
void readkeybd(float*ythrust,float*xthrust,int*fuelSupply,float*t)
{
 if(kbhit())
	{
		switch(toupper(getch()))
		{
		case('I'):
			{
				if (*fuelSupply>0)
				{
					if (*ythrust<MAINTHRUST)
					{
						*t=0.0;
					}
					*ythrust = -MAINTHRUST;
					(*fuelSupply-5>0)?(*fuelSupply -= 5):(*fuelSupply =0);
				}
				break;
			}
		case('L'):
			{
			if (*fuelSupply>0)
				{
				if (*xthrust<SIDETHRUST)
					{*t=0.0;}
				*xthrust = SIDETHRUST;
				(*fuelSupply-5>0)?(*fuelSupply -= 1):(*fuelSupply =0);
				}
			break;
			}
		case('J'):
			{
			if (*fuelSupply>0)
				{
				if (*xthrust>-SIDETHRUST)
					{*t=0.0;}
				*xthrust = -SIDETHRUST;
				(*fuelSupply-5>0)?(*fuelSupply -= 1):(*fuelSupply =0);
				}
				break;
			}
		case('P'):
			{
				do{
				}while(toupper(getch())!='P');
				break;
			}
		case('Q'):
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
	if (*fuelSupply<=0)
	{
		*ythrust=0.0;
		*xthrust=0.0;
	}
	return;
}

void quit()
{
	closegraph();
	exit(1);
}


/*****************************************************************
This function animatest the explosion of the spaceship
*****************************************************************/
void crash(float x,float y)
{
double alpha1=2.3,alpha2=0.17,alpha3=1.48;
double alpha4=2.96,alpha5=0.78,alpha6=3.32;

double theta[12]={0.1,0.5,0.75,1.0,1.25,1.5,1.75,2.0,2.25,2.5,2.75,3.0};
int i=0,k=0;

// erase the ship at the current location
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

// All of this makes the debris fly from the ship
for (i=0;i<90;i++)
	{
	 putpixel((x+i*cos(alpha1)),(y-i*sin(alpha1)),14);
	 putpixel((x+(i-2)*cos(alpha1)),(y-(i-2)*sin(alpha1)),0);
	 putpixel((x+i*cos(alpha2)),(y-i*sin(alpha2)),15);
	 putpixel((x+(i-2)*cos(alpha2)),(y-(i-2)*sin(alpha2)),0);
	 putpixel((x+i*0.65*cos(0.5)),(y-i*0.65*sin(0.5)),14);
	 putpixel((x+(i-2)*0.65*cos(0.5)),(y-(i-2)*0.65*sin(0.5)),0);

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
setcolor(7);
settextjustify(1,1);
settextstyle (GOTHIC_FONT,HORIZ_DIR,7);
outtextxy(getmaxx()/2,getmaxy()/3,"You were destroyed!");
settextstyle (GOTHIC_FONT,HORIZ_DIR,4);
outtextxy(getmaxx()/2,getmaxy()/3+50,"Try again?(y/n)");
return;
}

/****************************************************************
This Function displays the title screen and the game instructions
****************************************************************/
void intro(void)
{
// Title screen
setcolor(4);
settextjustify(1,1);
settextstyle (GOTHIC_FONT,HORIZ_DIR,10);
outtextxy(getmaxx()/2,getmaxy()/3,"Mars");
outtextxy(getmaxx()/2,3*getmaxy()/5,"Lander");
settextstyle (GOTHIC_FONT,HORIZ_DIR,3);
outtextxy(getmaxx()/2,7*getmaxy()/8,"Proceed if you dare!");
setcolor(CYAN);
settextstyle (GOTHIC_FONT,HORIZ_DIR,3);
outtextxy(getmaxx()/2,13*getmaxy()/16,"by William Hovey and Christopher Young");

getch();
closegraph();

//Instructions
clrscr();
printf("\nCongratulations, on your escape from the prison ship Alcatrazia.\n");
printf("You have stolen a guard ship and eluded persuit. Your ship is low\n");
printf("on fuel and you need to land as soon as possible on the nearest planet\n");
printf("-MARS!-. The terrain is hostile so, you must land on a landing pad.\n");
printf("You have only main thrust (i key) and side thrust (j&l keys) to control\n");
printf("your ship. Hurry there is not much fuel left!\n\n\n");
printf("Oh yeah, Land softly -- your velocity must be less than 0.5 m/sec. \nThose guard ships always did have weak landing gears.\n");
printf("\n\n\n\t\tHit 'q' anytime to quit, or 'p' to pause.\n\n");
printf("\t\tHit any Key to Begin.");

getch();

return;
}

/****************************************************************
This Function draws a boarder around the game screen
****************************************************************/
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

/****************************************************************
This Function displays a message of congratulations for a
safe landing
****************************************************************/
int congrats()
{
	int xmax=getmaxx();
	int ymax=getmaxy();

	setcolor(14);
	settextjustify(1,1);
	settextstyle (GOTHIC_FONT,HORIZ_DIR,3);
	outtextxy(xmax/2,ymax/3,"Congrats!! You landed safely.");
	outtextxy(xmax/2,ymax/3+75,"Continue to next level? (y/n)");
	if (tryAgain())
	{
		return(1);
	}
	else
	{
		quit();
	}
	return(0);
}

/**************************************************************
This function prompts the user to rerun the program and
returns a 1 for a Yes and a 0 for a No answer.
****************************************************************/
int tryAgain()
{
	char key;
	do{
		key=toupper(getch());
		switch (key)
		{
			case 'Y':
				return (1);
			case 'N':
				return (0);
		}
	}
	while (key!='Y'||key!='N');
	return(0);
}

