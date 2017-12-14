/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * simple video library for GP2X                                             *
 * by BarZoule                                                               *
 * april 5th, 2006                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * usage:				 		                                             *
 * init, then with the backbuffer, flip it to the screen, exit when done.    *
 * 						 		                                             *
 * Use at your own risk.                                                     *
 * This is greetware, use it freely as long as you greet me in your program. *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef _BZ_VIDEO_H
#define _BZ_VIDEO_H
//----------------------------------------------------------------------------//
#include "global.h"
#if(SCR_C==4)
  #define MASKCOL 0xFF00FF							//transparency color: magenta.
#else
  #define MASKCOL 0xF81F							//transparency color: magenta.
#endif
//----------------------------------------------------------------------------//
typedef struct{
	BYTE* pix;
	WORD h,w;
	BYTE shl;	
}bmp32_t;
typedef struct{
	BYTE* pix;
	WORD h,w;
	BYTE shl;	
}bmp24_t;
typedef struct{
	WORD* pix;
	WORD h,w;
	BYTE shl;	
}bmp16_t;
//----------------------------------------------------------------------------//
#if(SCR_C==4)
BYTE* 	scr_init();				//returns ptr to linear 32-bit buffer 320x240.
BYTE*	scr_getBuf();			//returns backbuffer again.
#else
WORD* 	scr_init();				//returns ptr to linear 16-bit buffer 320x240.
WORD*	scr_getBuf();			//returns backbuffer again.
#endif
void	scr_exit();
void	scr_flip();
inline WORD scr_RGB2W(BYTE r, BYTE g, BYTE b);		//24b to 16b.
inline void	scr_putpixel(WORD x, WORD y, BYTE r, BYTE g, BYTE b); //rgb pixel.
inline void scr_putpixelc(WORD x, WORD y, DWORD c);	//same with precalc color.
inline void scr_clr(BYTE r, BYTE g, BYTE b);		//clear screen to rgb.
inline void scr_clrc(DWORD c);                       //same with precalc color.
//----------------------------------------------------------------------------//
void	loadRAW32b(char* filename,bmp32_t* pic,int w, int h);
void	loadRAW32bAuto(char* filename,bmp32_t* pic);
void	killPic32b(bmp32_t* pic);
void	drawPic32b(bmp32_t* pic,int x, int y);
//----------------------------------------------------------------------------//
void	loadRAW24b(char* filename,bmp24_t* pic,int w, int h);
void	loadRAW24bAuto(char* filename,bmp24_t* pic);
void	killPic24b(bmp24_t* pic);
void	drawPic24b(bmp24_t* pic,int x, int y);
//----------------------------------------------------------------------------//
void	loadRAW16b(char* filename,bmp16_t* pic,int w, int h);
void	loadRAW16bAuto(char* filename,bmp16_t* pic);
void	killPic16b(bmp16_t* pic);
void	drawPic16b(bmp16_t* pic,int x, int y);
//----------------------------------------------------------------------------//
#endif//_BZ_VIDEO_H
