/******************************************
  2D to ASCII output
  by BarZoule
  13/11/2005
******************************************/

#include "global.h"

//#include <windows.h>


//rem the folowing line to have a basic 16-col output,
// else it will use libcaca to have nicer shading ;)
#define USE_CACA



#ifdef USE_CACA
	#include "caca\caca.h"
	#include "caca\caca_internals.h"
	#include "caca\config.h"
	struct caca_bitmap *ca_out;
#else
	#define SIZE SCR_W * SCR_H
	CHAR_INFO	screenBuffer[SCR_W * SCR_H];			//screen backbuffer.
	SMALL_RECT	drawRect = {0, 0, SCR_W-1, SCR_H-1};	//region where we're writing
	COORD		zeroZero = {0, 0};						//upper left corner to write from
	COORD		gridSize = {SCR_W , SCR_H};				//width and height
	HANDLE		outputH;								//pointer to the screen
#endif




#ifdef USE_CACA
void output_init()
{
	if(caca_init());							//FIXME return non-zero on error
	caca_set_feature( CACA_DITHERING_RANDOM );	//looks nicer than DITHER_NONE	
	caca_set_feature( CACA_ANTIALIASING );		//nicer than CACA_ANTIALIASING_NONE

	//define our pixels (*scr)
	ca_out = caca_create_bitmap(32,SCR_W,SCR_H,4*SCR_W,0x000000FF,0x0000FF00,0x00FF0000,0x00000000);
}
void output(unsigned char *scr)
{
	//dump the pixels and update the screen
	caca_draw_bitmap(0,0,caca_get_width()-1,caca_get_height()-1,ca_out,(char*)scr);	
	caca_refresh();
}
void output_exit()
{
	caca_free_bitmap(ca_out);
	caca_end();
}
#else
void output_init()
{
	outputH = GetStdHandle(STD_OUTPUT_HANDLE);	// Get a OUTPUT handle to our screen.
}
void output(unsigned char *scr)
{
	char col = 0;
	int i	 = 0;

	//parse
	for(i=0;i<SIZE;i++)
	{
		unsigned char c,r,g,b,h;
		//convert
		if(SCR_C==4)
		{
			r = *scr; scr++;
			g = *scr; scr++;
			b = *scr; scr++; scr++;
			h = r+g+b>384?128:0;
			r = r>64?64:0;
			g = g>64?32:0;
			b = b>64?16:0;
			c = r|g|b|h;
		}
		else
		if(SCR_C==3)
		{
			r = *scr; scr++;
			g = *scr; scr++;
			b = *scr; scr++;
			h = r+g+b>384?128:0;
			r = r>64?64:0;
			g = g>64?32:0;
			b = b>64?16:0;
			c = r|g|b|h;
		}
		else
		if(SCR_C==1)
		{
			c = *scr; scr++;
		}
		//store
		screenBuffer[i].Char.AsciiChar = rand()%32+32;
		screenBuffer[i].Attributes = c;
	}
	WriteConsoleOutput(outputH, screenBuffer, gridSize, zeroZero, &drawRect);
}
void output_exit()
{}
#endif