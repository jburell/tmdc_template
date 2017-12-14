//============================================================================//
//version heavily modded for TMDC
//============================================================================//

#ifdef WIN32
#include "global.h"
#ifdef ASCII_OUT
//============================================================================//
#include "ASCIIout.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bz_video.h"
//----------------------------------------------------------------------------//
#define 	SCRSIZE		SCR_W*SCR_H*SCR_C
#if(SCR_C==4)
BYTE*		scr1;
#else //==2
WORD*		scr1;
#endif
//----------------------------------------------------------------------------//

#if(SCR_C==4)
BYTE* scr_init()
#else //==2
WORD* scr_init()
#endif
{
	//init and setup stuff
	output_init();
	//get backbuffer
	scr1=malloc(SCRSIZE);								//get offscreen buffer
	if(!scr1)return 0;
	return scr1;
}
//----------------------------------------------------------------------------//
#if(SCR_C==4)
BYTE* scr_getBuf(void)
#else //==2
WORD* scr_getBuf(void)
#endif
{
	return &scr1[0];
}
//----------------------------------------------------------------------------//
void scr_exit()
{
	//clean stuff if needed
	free(scr1);
	scr1= NULL;
}
//----------------------------------------------------------------------------//
void scr_flip()
{	
	output(scr1);
}
//============================================================================//
#else //WIN32
//============================================================================//
#include "..\include\allegro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bz_video.h"
#include "bz_timer.h"
#include "bz_font.h"
//----------------------------------------------------------------------------//
#define 	SCRSIZE		SCR_W*SCR_H*SCR_C
#if(SCR_C==4)
BYTE		*scr1;
#else //==2
WORD		*scr1;
#endif
BITMAP		*backbuffer;
BITMAP		*temp;			//for scaling and vsynch stuff in allegro
//----------------------------------------------------------------------------//

unsigned short int* scr_init()
{
	//init and setup stuff
	set_color_depth(SCR_C*8);
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, WINDOW_W, WINDOW_H, 0, 0) != 0)return 0;
	//get screen buffer
	backbuffer = create_bitmap_ex(SCR_C*8,SCR_W,SCR_H);
	scr1 = backbuffer->dat;
	if(!scr1)return 0;
	//get temp buffer (for 16->32->scaleup)
	temp = create_bitmap(SCR_W,SCR_H);
	if(!temp)return 0;
	return scr1;
}
//----------------------------------------------------------------------------//
WORD*	scr_getBuf(void)
{
	return &scr1[0];
}
//----------------------------------------------------------------------------//
void scr_exit()
{
	//clean stuff if needed
	destroy_bitmap(backbuffer);
	destroy_bitmap(temp);
}
//----------------------------------------------------------------------------//
void scr_flip()
{	
	/*
	DWORD t=timer_elapsed();
	t=40-t;												//target 25 FPS
	if(t>0 && t<100)rest(t);							//give unused time back to the OS  ( >50 = error )
	blit(backbuffer,temp,0,0,0,0,SCR_W,SCR_H);			//move pixels from offscreen buffer to screen buffer (dbl buffering)
	vsync();											//vsynch or not vsynch?
	stretch_blit(temp,screen,0,0,SCR_W,SCR_H,0,0,WINDOW_W,WINDOW_H);	//stretch up :)
	*/ //FAST
	blit(backbuffer,temp,0,0,0,0,SCR_W,SCR_H);			//move pixels from offscreen buffer to screen buffer (dbl buffering)
	stretch_blit(temp,screen,0,0,SCR_W,SCR_H,0,0,WINDOW_W,WINDOW_H);	//stretch up :)
}
//============================================================================//
#endif
#else
//============================================================================//
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "bz_video.h"
//----------------------------------------------------------------------------//
int fbdev;
WORD 		*scr0;
#define 	SCRSIZE		SCR_W*SCR_H*SCR_C
WORD		*scr1;
//----------------------------------------------------------------------------//

unsigned short int* scr_init()
{
	//init and setup stuff
	//get screen buffer
	fbdev=open("/dev/fb0", O_RDWR);
	scr0=(WORD *)mmap(0, SCRSIZE, PROT_WRITE, MAP_SHARED, fbdev, 0);
	//	fprintf(stderr, "fb opened at %x, fd %d\n", scr0, fbdev);
	if(!scr0)return 0;
	scr1=malloc(SCRSIZE);								//get offscreen buffer
	if(!scr1)return 0;
	return scr1;
}
//----------------------------------------------------------------------------//
WORD*	scr_getBuf(void)
{
	return &scr1[0];
}
//----------------------------------------------------------------------------//
void scr_exit()
{
	close(fbdev);
	//clean stuff if needed
	free(scr1);
	scr1= NULL;
}
//----------------------------------------------------------------------------//
void scr_flip()
{	
	//vsynch or not vsynch?			
	//vsynch is possible by accessing memory adresses/registers
	//but we won't do it here cuz it will slow us down anyway
#if(SCR_C==4)
	//TODO: REWRITE ME !!! (32b->16b convertion)
#else
	int i=SCRSIZE>>2;									//SCRSZE is in bytes, 4 bytes per int
	DWORD *S= (DWORD*)scr1;
	DWORD *D= (DWORD*)scr0;
	while(i--)
	{
		*D = *S;
		D++;S++;
	}
#endif
}
//============================================================================//
#endif

//=COMMON=ROUTINES============================================================//
inline WORD scr_RGB2W(BYTE r, BYTE g, BYTE b)
{
	WORD c=0;											//color code
	r>>=3;	c|=r;	c<<=6;								//rrrrr000000
	g>>=2;	c|=g;	c<<=5;								//rrrrrgggggg00000
	b>>=3;	c|=b;										//rrrrrggggggbbbbb
	return c;
}
inline DWORD scr_W2RGBA(WORD c)
{
	DWORD d=0;											//color code
	BYTE r,g,b;
	b=c&0xFF;	b<<=3;	c>>=5;							//rrrrrggggggbbbbb
	g=c&0xFF;	b<<=2;	c>>=6;							//rrrrrgggggg00000
	r=c&0xFF;	b<<=3;									//rrrrr000000
	return d;
}
//----------------------------------------------------------------------------//

inline void scr_putpixel(WORD x, WORD y, BYTE r, BYTE g, BYTE b)
{
	if(x>SCR_W)return;
	if(y>SCR_H)return;
	((int*)scr1)[x+y*SCR_W]=r<<16|g<<8|b;
//	scr1[x+(y<<6)+(y<<8)]=scr_RGB2W(r,g,b);				//send to virtual buffer
}
//----------------------------------------------------------------------------//
inline void scr_putpixelc(WORD x, WORD y, DWORD c)
{
	if(x>SCR_W)return;									//WORD! can't be negative ;)
	if(y>SCR_H)return;
	((int*)scr1)[x+y*SCR_W]=c;
//	scr1[x+(y<<6)+(y<<8)]=c;							//send to virtual buffer
}
//----------------------------------------------------------------------------//
inline void scr_clrc(DWORD c)
{
	int i = SCR_W*SCR_H;								//SCRSIZE is in bytes, use int
	DWORD *s= (DWORD*)scr1;
	DWORD col = c;
	while(i--)*(s++)=col;								//fetch ints
}
//----------------------------------------------------------------------------//
inline void scr_clr(BYTE r, BYTE g, BYTE b)
{
	scr_clrc(b<<16|g<<8|r);
//	scr_clrc(scr_RGB2W(r,g,b));
}
//----------------------------------------------------------------------------//
void	extractSize(char* filename,int *x,int *y)
{
	int i,wi,hi,n;
	char w[6],h[6];										//so max==65536, nuff?
	//we try to autodetect the H and W from the filename _123x123.raw
	i=0;
	while(filename[i++]);								//go to end
	while(filename[--i]!='.');n=5;h[5]=0;				//skip extension
	while((h[--n]=filename[--i])!='x');n++;				//copy to w
	while(n) h[--n]=' ';	  n=5;w[5]=0;				//skip extension
	while((w[--n]=filename[--i])!='_');n++;				//copy to h
	while(n) w[--n]=' ';
	sscanf(h,"%d",&hi);
	sscanf(w,"%d",&wi);
	*x = wi;
	*y = hi;
}
//----------------------------------------------------------------------------//
void	loadRAW32b(char* filename,bmp32_t* pic,int w, int h)
{
	FILE* f;
	f=fopen(filename,"rb");
	if(f == NULL) {
	    printf("EEEEEEEEEEEK!!!     file not found!!");
	    return;
	}

	pic->w = w;
	pic->h = h;
	{
		int mul=pic->w;										//find width multiplier/left-shifter
		for(;mul>>=1;pic->shl++);
	}
	pic->pix = malloc(w*h*4);	//RGBA
	if(pic->pix == NULL)printf("EEEEEEEEEEEK!!!     not nuff mem!!");
	fread(pic->pix,w*h*4,1,f);
				 
	fclose(f);		
}
//----------------------------------------------------------------------------//
void	loadRAW32bAuto(char* filename,bmp32_t* pic)
{
	int wi,hi;
	extractSize(filename,&wi,&hi);
	loadRAW32b(filename,pic,wi,hi);
}
//----------------------------------------------------------------------------//
void	killPic32b(bmp32_t* pic)
{
	free(pic->pix);
	pic->pix = NULL;
	pic->h=0;
	pic->w=0;
}
//----------------------------------------------------------------------------//
void	drawPic32b(bmp32_t* pic,int x, int y)
{
	int i=0;
	unsigned char* d = scr1;
	unsigned char* s = pic->pix;
	unsigned short r,g,b,a;

	//outta screen
	if(x+pic->w<0)return;
	if(y+pic->h<0)return;
	if(x>SCR_W)return;
	if(y>SCR_H)return;

	for(i=0;i<(pic->w*pic->h);i++)	//RGBAassumed
	{
		int x1=i%pic->w;
		int y1=i/pic->w;
		int ofs;
		if(x1+x<0)continue;
		if(y1+y<0)continue;
		if(x1+x>=SCR_W)continue;
		if(y1+y>=SCR_H)continue;
		ofs = (x+x1 + (y+y1)*SCR_W)*SCR_C;
		a = s[i*SCR_C+3];
		r = s[i*SCR_C  ];
		g = s[i*SCR_C+1];
		b = s[i*SCR_C+2];

		d[ofs  ] = (d[ofs  ]*(0xff-a) + r*a)>>8;
		d[ofs+1] = (d[ofs+1]*(0xff-a) + g*a)>>8;
		d[ofs+2] = (d[ofs+2]*(0xff-a) + b*a)>>8;
	}
}
//----------------------------------------------------------------------------//
void	loadRAW24b(char* filename,bmp24_t* pic,int w, int h)
{
	FILE* f;
	f=fopen(filename,"rb");
	if(f == NULL)printf("EEEEEEEEEEEK!!!     file not found!!");

	pic->w = w;
	pic->h = h;
	pic->shl=0;
	{
		int mul=pic->w;										//find width multiplier/left-shifter
		for(;mul>>=1;pic->shl++);
	}
	pic->pix = malloc(w*h*3);	//RGB
	if(pic->pix == NULL)printf("EEEEEEEEEEEK!!!     not nuff mem!!");
	fread(pic->pix,w*h*3,1,f);
				 
	fclose(f);		
}
//----------------------------------------------------------------------------//
void	loadRAW24bAuto(char* filename,bmp24_t* pic)
{
	int wi,hi;
	extractSize(filename,&wi,&hi);
	loadRAW24b(filename,pic,wi,hi);
}
//----------------------------------------------------------------------------//
void	killPic24b(bmp24_t* pic)
{
	free(pic->pix);
	pic->pix = NULL;
	pic->h=0;
	pic->w=0;
}
//----------------------------------------------------------------------------//
void	drawPic24b(bmp24_t* pic,int x, int y)
{
	int i=0;
	DWORD* d = (DWORD*)scr1;
	BYTE* s  = pic->pix;

	//outta screen
	if(x+pic->w<0)return;
	if(y+pic->h<0)return;
	if(x>SCR_W)return;
	if(y>SCR_H)return;

	for(i=0;i<(pic->w*pic->h);i++)	//RGB
	{
		int x1=i%pic->w;
		int y1=i/pic->w;
		int ofs;
		if(x1+x<0)continue;
		if(y1+y<0)continue;
		if(x1+x>=SCR_W)continue;
		if(y1+y>=SCR_H)continue;
		ofs = x+x1 + (y+y1)*SCR_W;
		d[ofs] = (s[i*3]<<11)|(s[i*3+1]<<6)|s[i*3+2];
	}
}         
//----------------------------------------------------------------------------//
void	loadRAW16b(char* filename,bmp16_t* pic,int w, int h)   //24bit RGB raw file loaded to 16b
{                                                           //this one is the fastest and most efficient way to proceed
	int i;
	FILE* f;
	f=fopen(filename,"rb");
	if(f == NULL){printf("EEEEEEEEEEEK!!!     file not found!!");scr_flip();}

	pic->w = w;
	pic->h = h;
	pic->shl=0;
	{
		int mul=pic->w;										//find width multiplier/left-shifter
		for(;mul>>=1;pic->shl++);
	}

	pic->pix = malloc(w*h*2);								//16b
	if(pic->pix == NULL){printf("EEEEEEEEEEEK!!!     not nuff mem!!");scr_flip();}
	for(i=0;i<w*h;i++)
	{
		BYTE c[3];											//load 24b
		fread(c,3,1,f);
		pic->pix[i] = scr_RGB2W(c[0],c[1],c[2]);			//convert to 16b
	}
				 
	fclose(f);		
}
//----------------------------------------------------------------------------//
void	loadRAW16bAuto(char* filename,bmp16_t* pic)
{
	int wi,hi;
	extractSize(filename,&wi,&hi);
	loadRAW16b(filename,pic,wi,hi);
}
//----------------------------------------------------------------------------//
void	killPic16b(bmp16_t* pic)
{
	free(pic->pix);
	pic->pix = NULL;
	pic->h=0;
	pic->w=0;
}
//----------------------------------------------------------------------------//
void	drawPic16b(bmp16_t* pic,int x, int y)
{
	int i=0;
	DWORD* d = (int*)scr1;
	WORD* s = pic->pix;

	//outta screen
	if(x+pic->w<0)return;
	if(y+pic->h<0)return;
	if(x>SCR_W)return;
	if(y>SCR_H)return;

	for(i=0;i<(pic->w*pic->h);i++)	//RGB
	{
		int x1=i%pic->w;
		int y1=i/pic->w;
		int ofs;
		if(x1+x<0)continue;
		if(y1+y<0)continue;
		if(x1+x>=SCR_W)continue;
		if(y1+y>=SCR_H)continue;
		ofs = x+x1 + (y+y1)*SCR_W;
		if(s[i]!=MASKCOL)
			d[ofs] = scr_W2RGBA(s[i]);
	}
}
