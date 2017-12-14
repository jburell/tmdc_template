/*******************************
  Sin/Cos Table + square root,
  cuz the standard math.h
  implementation for GP2X
  doesn't seem to work.
*******************************/

#ifndef _BZ_MATH_H
#define _BZ_MATH_H

#include "global.h"

float bsin(float f);
float bcos(float f);
float batan(float f);
inline float bsqr(float f);
float bsqrt(float f);
long isqrt(long i);
float babs(float f);

#define USE_FLOAT
#ifdef USE_FLOAT
#ifndef FORCEINLINE
#define FORCEINLINE
#endif
typedef float FIXP;
	#define fp_mul(a,b)			(a*b)
	#define fp_muli(a,b)		(a*b)
	#define fp_mulf(a,b)		(a*b)

	#define fp_div(a,b)			(a/b)
	#define fp_divi(a,b)		(a/b)
	#define fp_divf(a,b)		(a/b)

	#define fp_add(a,b)			(a+b)
	#define fp_addi(a,b)		(a+b)
	#define fp_addf(a,b)		(a+b)

	#define fp_sub(a,b)			(a-b)
	#define fp_subi(a,b)		(a-b)
	#define fp_subf(a,b)		(a-b)

	#define fp_convi(a)			a
	#define fp_convf(a)			a

	#define fp_iconv(a)			a
	#define fp_fconv(a)			a

	#define fp_optmul( a, b) (a*b)
#else

typedef long int FIXP;
#define PRECISION 15
//precision over 15 causes bugs	
//precision below 8 makes some subtil glitches in textures
//precision below 4 is unusable for textures

//#define FORCEINLINE		  //doesn't seem to work properly

#ifdef FORCEINLINE
						  //todo: fix me!!
	#define fp_mul(a,b)			((a*b)>>PRECISION)
	#define fp_muli(a,b)		(a*b)
	#define fp_mulf(a,b)		((FIXP)(a*b))

	#define fp_div(a,b)			((a<<PRECISION)/b)
	#define fp_divi(a,b)		(a/b)
	#define fp_divf(a,b)		((FIXP)(a/b))

	#define fp_add(a,b)			(a+b)
	#define fp_addi(a,b)		(a+(b<<PRECISION))
	#define fp_addf(a,b)		(a+(FIXP)(b*(1<<PRECISION)))

	#define fp_sub(a,b)			(a-b)
	#define fp_subi(a,b)		(a-(b<<PRECISION))
	#define fp_subf(a,b)		(a-(FIXP)(b*(1<<PRECISION)))

	#define fp_convi(a)			(a<<PRECISION)
	#define fp_convf(a)			((FIXP)(a*(float)(1<<PRECISION)))

	#define fp_iconv(a)			(a>>PRECISION)
	#define fp_fconv(a)			(a*(1.f/(float)(1<<PRECISION)))

#else

	 FIXP fp_optmul(FIXP a,FIXP b);		//special version for matrix opts
	 FIXP fp_mul(FIXP a,FIXP b);
	 FIXP fp_muli(FIXP a,int b);
	 FIXP fp_mulf(FIXP a,float b);

	 FIXP fp_div(FIXP a,FIXP b);
	 FIXP fp_divi(FIXP a,int b);
	 FIXP fp_divf(FIXP a,float b);

	 FIXP fp_add(FIXP a,FIXP b);
	 FIXP fp_addi(FIXP a,int b);
	 FIXP fp_addf(FIXP a,float b);

	 FIXP fp_sub(FIXP a,FIXP b);
	 FIXP fp_subi(FIXP a,int b);
	 FIXP fp_subf(FIXP a,float b);

	 FIXP fp_convi(int a);
	 FIXP fp_convf(float a);
	 int  fp_iconv(FIXP a);
	 float fp_fconv(FIXP a);

#endif
#endif

#endif//_BZ_MATH_H_
