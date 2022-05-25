/****************************************************************
*	EE 304 Spring Semester 1995 UNIVERSITY OF NORTH DAKOTA
*
*	Final Project
*
*	Names: William Hovey & Christopher Young
*
*	Title:  Final Project
*
*	Date Completed:   5/23/95
*
*	Date Due:  ?/?/95
*
*	Lab description:
*				This module contains the functions that are used to
*					draw the landing pads, the landscape and determine
*					when the ship has come in contact with either.
*				Due to the poor performance of srand(), random(), and rand(),
*					I wrote my own linear congruential generator that uses the
*					hundredth second as a seed.
*
*	File Name: surface.cpp
*
*****************************************************************/

#include <graphics.h>
#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>

#define NUMPADS 5			//Number of Landing pads to generate+2 for the ends

/***************************
 *		Function Protoypes   *
 ***************************/

void landingPad(float pad[NUMPADS][3],int maxPad);
void drawHills(float pad[NUMPADS][3], int maxHill);
float cyrand(int seed);
int contact(float x,float y, int radius);

/***************************
 *		   Functions         *
 ***************************/

 /***************************************************************
  *	This function generates the landing pads and draws them
  *		on the screen.
  *	pad[][] is and array that keeps track of the left-hand
  *		x-position (pad[][0])and y-position (pad[][1] and the
  *		length of the pad (pad[][2]).
  ***************************************************************/

 void landingPad(float pad[NUMPADS][3],int maxPad)

{
	//  Set up and initialize variables
	int i,xmax,ymax;
	float ran;

	setcolor(RED);
	ran=cyrand(1);
	xmax=getmaxx();
	ymax=getmaxy();

	// Left-most point of surface

	pad[0][0]=0;
	pad[0][1]=ymax-(ymax-maxPad)*cyrand(0);
	pad[0][2]=0.0;

	//  Generate NUMPADS-2 landing pads

	for (i=1;i<=NUMPADS-2;i++)
	{
		pad[i][2]=30.0;

		ran=cyrand(0);
		pad[i][0]=ran*((xmax-pad[i][2])/NUMPADS-2)+((i*xmax)/NUMPADS-2);

		ran=cyrand(0);
		pad[i][1]=ymax-(ran*(ymax-maxPad))-5.0;

		//  Draw Landing Pad 3 pixels thick

		moveto(pad[i][0],pad[i][1]);
		linerel(pad[i][2],0);
		moveto(pad[i][0],pad[i][1]+1);
		linerel(pad[i][2],0);
		moveto(pad[i][0],pad[i][1]+2);
		linerel(pad[i][2],0);
	}

	//  Generate Right-most point of surface

	pad[NUMPADS-1][0]=xmax;
	pad[NUMPADS-1][1]=pad[0][1];
}

 /***************************************************************
  *	This function generates the Martian Surface by randomly
  *		3 heights between each landing pad and drawing lines
  *		between them.
  ***************************************************************/


void drawHills(float pad[NUMPADS][3],int maxHill)
{
	//  Declare and initialize variables
	int i,j,ymax,xmax;

	xmax=getmaxx();
	ymax=getmaxy();

	//  For each pad, generate the heights and draw the lines

	for (i=0;i<=NUMPADS-2;i++)
	{
		setcolor(RED);
		moveto(pad[i][0]+pad[i][2],pad[i][1]);
		lineto(pad[i][0]+pad[i][2]+1.0*((pad[i+1][0]-(pad[i][0]+pad[i][2]))/4.0),ymax-(ymax-maxHill)*cyrand(0));
		lineto(pad[i][0]+pad[i][2]+2.0*((pad[i+1][0]-(pad[i][0]+pad[i][2]))/4.0),ymax-(ymax-maxHill)*cyrand(0));
		lineto(pad[i][0]+pad[i][2]+3.0*((pad[i+1][0]-(pad[i][0]+pad[i][2]))/4.0),ymax-(ymax-maxHill)*cyrand(0));
		lineto(pad[i+1][0],pad[i+1][1]);
	}

	//  Make the surface solid red
	setfillstyle(SOLID_FILL,RED);
	floodfill(xmax/2,ymax,RED);

	//	  Highlight the landingn pads in CYAN
	setcolor(CYAN);
	for (j=1;j<=NUMPADS-2;j++)
	{
		moveto(pad[j][0],pad[j][1]);
		linerel(pad[j][2],0);
		moveto(pad[j][0],pad[j][1]+1);
		linerel(pad[j][2],0);
		moveto(pad[j][0],pad[j][1]+2);
		linerel(pad[j][2],0);
	}
}

 /***************************************************************
  *	This function generates the random numbers used in
  *		landingPad() and drawHills.
  *	The hundredth second term in the system time is used as the
  *		seed if a '1' is passed as the argument.
  ***************************************************************/


float cyrand(int seed)
{
	static unsigned long jran;   //Making jran static allows the generation of a random series
	unsigned long ia=9301,ic=49297,im=233280;
	struct dostime_t t;

	if (seed)
	{
		_dos_gettime(&t);
		jran=(unsigned long)t.hsecond;
	}
	jran=(jran*ia+ic)%im;
	return((float)jran/(float)im);
}


/***************************************************************
 *	This function scans a row of pixels below the ship to see if
 *		any are turned on.  The function returns a value only when
 *		a RED or CYAN pixel is detected and returns the color found.
 *	 A '0' is returned of no RED or CYAN pixels are turned on.
 *
 *	 x and y are the ship's current position, radius is a measure
 *		of the ship's size.
 ***************************************************************/


int contact(float x, float y, int radius)
{
	int i;

	//  Scans a line right below the ship

	for (i=x-(radius+2);i<=x+(radius+2);i++)
	{
		if (getpixel(i,y+radius+1)==RED)
		{
			return(RED);
		}
		if (getpixel(i,y+radius+1)==CYAN)
		{
			return(CYAN);
		}
	}
	return(0);
}


