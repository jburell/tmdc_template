/******************************************
  some kind of demo framework example for
  TMDC11
  by BarZoule, fullscreen code by The Hardliner
  13/11/2005
  12/02/2008

  NOTE: all the flow of the demo is contained in
  the .IT file through Zxy commands. Edit them
  using your favorite tracker to change the
  sequencing of the demo.

  You are free tp use/edit/do-whatever-you-want
  with this code. This code is provided as-is
  with no support and no warrenty.

  Enjoy!
*/

#define VC_EXTRALEAN

#include <windows.h>	//GetTickCount and GetAsyncKeyState
#include <stdio.h>
#include <conio.h>
#include "global.h"		//Screen size and typedefs
#include "ASCIIout.h"	//wrapper to libcaca
#include "bz_video.h"	//loading/drawing of .raw images
#include "effects.h"	//demo effects
#include "music.h"		//wrapper to fmod


#define PI 3.14159265358979323846f

int	gDemoDone=0;
unsigned char*	scr;

bmp32_t ttl[2],
		crdz,
		grtz,
		rtzm,
		tnnl;


void con_init(int width, int height, int fullscreen)
{
	HANDLE hCon;
	COORD crd = { width, height };
	SMALL_RECT sr = { 0, 0, width-1, height-1 };

	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(hCon, crd);
	SetConsoleWindowInfo(hCon, TRUE, &sr);

	if(fullscreen){
		HMODULE kernel;
		BOOL (__stdcall *SetConsoleDisplayMode)(HANDLE,DWORD,PCOORD);
		COORD newcrd;

		kernel = GetModuleHandle("kernel32.dll");
		SetConsoleDisplayMode = (BOOL(__stdcall *)(HANDLE,DWORD,PCOORD))GetProcAddress(kernel, "SetConsoleDisplayMode");
		if(!IsBadCodePtr(SetConsoleDisplayMode)){
			SetConsoleDisplayMode(hCon, 1, &newcrd);
		}
	}
}


void loadAssets()
{
	loadRAW32bAuto("nudata/title01_80x50.raw"	,&ttl[0]);
	loadRAW32bAuto("nudata/title02_80x50.raw"	,&ttl[1]);
	loadRAW32bAuto("nudata/credz_80x50.raw"		,&crdz);
	loadRAW32bAuto("nudata/greetz_80x50.raw"	,&grtz);
	loadRAW32bAuto("nudata/rotoz_64x64.raw"		,&rtzm);
	loadRAW32bAuto("nudata/tunnel_64x64.raw"	,&tnnl);
}

void killAssets()
{
	killPic32b(&ttl[0]);
	killPic32b(&ttl[1]);
	killPic32b(&crdz);
	killPic32b(&grtz);
	killPic32b(&rtzm);
	killPic32b(&tnnl);
}


void process_logix(long int t)
{
	mus_event*	mevent;
	static enum{	ENULL=-1, ETTL1=0, ETTL2, ECRDZ, EGRTZ, ETNNL, ERTZM	} scene = ENULL;
	static int	scene_part;
	static int	scene_start;
	static int	part_start;
	static int  frameTime=0;
	static int  lastFrameTime=0;

	static int  blur=0;
	static int  fade=0;

	int			telaps;
	int			newpart=0;
	while((mevent = mus_dequeue()))
	{
		unsigned char evt = mevent->param>>4;		// 0..F parameters
		unsigned char dat = mevent->param&0xF;

		switch(evt)
		{											//here we could setup much more than only the scene number
			case 0:									//for example, 0X could set scene X where 1X would set a post effect
			case 1:									//or in XY X could be a scene and Y a subscene
			case 2:									//with 256 different possibilities, there's probably more than what you need.
			case 3:
			case 4:
			case 5:
				scene=evt;
				newpart=1;
				break;
			case 0xd:
				blur=dat;
				break;
			case 0xe:
				fade=(dat-8)*32;					//normal = 0xE8 lower is fade to black, higher is fade to white
				break;
			case 0xf: 
				gDemoDone=1;
				break;
		}
		if(newpart)scene_start=t;
	}

	telaps = t-scene_start;								//time elapsed since scene started
	scr_clrc(0xFFFFFF);									//clear the scene

	switch(scene)										//draw the scene
	{
		case ETTL1:
			{
				int y=telaps/10-SCR_H;					//from top
				if(y>0)y=0;
				drawPic32b(&ttl[0],0,y);
			}
			break;
		case ETTL2:
			{
				int y=SCR_H-telaps/10;					//from bottom
				if(y<0)y=0;	 if(y)
				drawPic32b(&ttl[0],0,y-SCR_H);
				drawPic32b(&ttl[1],0,y);
			}
			break;
		case ECRDZ:
				drawPic32b(&crdz,0,0);
			break;
		case EGRTZ:
				drawPic32b(&grtz,0,0);
			break;
		case ETNNL:
				gen_tunnel(SCR_W/2,SCR_H/2,&tnnl,scr,0,1,0);
			break;
		case ERTZM:
				gen_rotozoom(telaps/100.f,500.f/++telaps,&rtzm,scr);
			break;
	}

	if(blur)mod_blur(scr,0,0,SCR_W,SCR_H,blur);
	if(fade)mod_level(scr,0,0,SCR_W,SCR_H,-fade,255-fade);
	// Display it in all its ASCII glory!
	output(scr);        
}

int main(int argc, char **argv)
{
	long int timeStart;
	long int time;
	//init
	scr=scr_init();
	SetConsoleTitle("TMDC11 nd 2k8 | alt+enter=fullscreen");
	con_init(80,50,1);
	output_init();
	
	srand(GetTickCount());
	//load
	loadAssets();
	//load and start music
	mus_play("nudata/JOES11.it");	//plz note, this module is totally ripped from Jesus On E's, shame on us!
	//mainloop
	timeStart = GetTickCount();		//might give better sync if called after PlayTune();

	while (!gDemoDone)
	{
		//process timed stuff
		time=GetTickCount()-timeStart;
		fx_updateTime(time);
		process_logix(time);
		//wait for ESC
		if (_kbhit())
			gDemoDone=(getch()==27);	// test for esc
	}
	//clean
	killAssets();
	mus_stop();
	//exit
	output_exit();
	return 0;
}
