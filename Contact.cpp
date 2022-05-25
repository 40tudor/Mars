#include <graphics.h>

int contact(float x, float y, int radius)
{
	int i,contact=0;

	for (i=x-radius;i<=x+radius;i++)
	{
		contact+=getpixel(i,y+radius+1);
	}
	contact+=getpixel(x+(radius+1),y+radius);
	contact+=getpixel(x-(radius+1),y+radius);

	return(contact);
}
