/***********************
	GLOBAL.H
***********************/
#ifndef _GLOBAL_H
#define _GLOBAL_H

#define ASCII_OUT
// the ASCII_OUT version is RGBA and stuff is sent to CACA instead of the allegro output.
// *try* to keep GP2X compatibility cuz it would rule.
// tho for that we'll cahnge the buffer format only at the last stage instead of the
// first stage (last blitting rather than texture loading. will polly be slow as hell.

//WIN32 platform redef
#ifdef WIN32
  #ifndef inline
	//define inline as _inline or as *nothing* ?
	#define inline
  #endif
	#define WINDOW_W	640
	#define WINDOW_H	480
	typedef unsigned __int64	QWORD;
	typedef __int64 INT64;
#else
	typedef unsigned long long	QWORD;
	typedef long long INT64;
#endif


// file reading
#include <stdio.h>		
// for malloc
#include <stdlib.h>
//for sintables (sinf/cosf) and sqrt
#include "bz_math.h"


// screen width, height, channels ? (8bit channels)
#define SCR_W	80			//you can set that to 160x100
#define SCR_H	50			//to have nice antialiasing with libcaca
#define SCR_C	4

//common typedefs
typedef unsigned char 		BYTE;
typedef unsigned short int	WORD;
typedef unsigned long int	DWORD;
typedef int 				BOOL;
#define FALSE 0
#ifndef TRUE
 #define TRUE -1	
#endif

extern char debug[1024];
//#define TIME_DRAW

#endif//_GLOBAL_H
