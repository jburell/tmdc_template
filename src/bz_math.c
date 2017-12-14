/*******************************
  Sin/Cos Table and retrieving 
  functions, cuz the standard
  math.h implementation for GP2X
  doesn't seem to work.
--------------------------------
  so far it supports 256 angles,
  with added linear lerping
  to smooth things up.
*******************************/

//#define QUICK_N_DIRTY
//QUICK_N_DIRTY = less precision but more speed

#include "bz_math.h"
	

float SinTable[256]={
0.000000f, 0.024541f, 0.049068f, 0.073565f, 0.098017f, 0.122411f, 0.146730f, 0.170962f,
0.195090f, 0.219101f, 0.242980f, 0.266713f, 0.290285f, 0.313682f, 0.336890f, 0.359895f,
0.382683f, 0.405241f, 0.427555f, 0.449611f, 0.471397f, 0.492898f, 0.514103f, 0.534998f,
0.555570f, 0.575808f, 0.595699f, 0.615232f, 0.634393f, 0.653173f, 0.671559f, 0.689541f,
0.707107f, 0.724247f, 0.740951f, 0.757209f, 0.773010f, 0.788346f, 0.803208f, 0.817585f,
0.831470f, 0.844854f, 0.857729f, 0.870087f, 0.881921f, 0.893224f, 0.903989f, 0.914210f,
0.923880f, 0.932993f, 0.941544f, 0.949528f, 0.956940f, 0.963776f, 0.970031f, 0.975702f,
0.980785f, 0.985278f, 0.989177f, 0.992480f, 0.995185f, 0.997290f, 0.998795f, 0.999699f,
1.000000f, 0.999699f, 0.998795f, 0.997290f, 0.995185f, 0.992480f, 0.989177f, 0.985278f,
0.980785f, 0.975702f, 0.970031f, 0.963776f, 0.956940f, 0.949528f, 0.941544f, 0.932993f,
0.923880f, 0.914210f, 0.903989f, 0.893224f, 0.881921f, 0.870087f, 0.857729f, 0.844854f,
0.831470f, 0.817585f, 0.803208f, 0.788346f, 0.773010f, 0.757209f, 0.740951f, 0.724247f,
0.707107f, 0.689541f, 0.671559f, 0.653173f, 0.634393f, 0.615232f, 0.595699f, 0.575808f,
0.555570f, 0.534998f, 0.514103f, 0.492898f, 0.471397f, 0.449611f, 0.427555f, 0.405241f,
0.382683f, 0.359895f, 0.336890f, 0.313682f, 0.290285f, 0.266713f, 0.242980f, 0.219101f,
0.195090f, 0.170962f, 0.146730f, 0.122411f, 0.098017f, 0.073565f, 0.049068f, 0.024541f,
0.000000f,-0.024541f,-0.049068f,-0.073565f,-0.098017f,-0.122411f,-0.146730f,-0.170962f,
-0.195090f,-0.219101f,-0.242980f,-0.266713f,-0.290285f,-0.313682f,-0.336890f,-0.359895f,
-0.382683f,-0.405241f,-0.427555f,-0.449611f,-0.471397f,-0.492898f,-0.514103f,-0.534998f,
-0.555570f,-0.575808f,-0.595699f,-0.615232f,-0.634393f,-0.653173f,-0.671559f,-0.689541f,
-0.707107f,-0.724247f,-0.740951f,-0.757209f,-0.773010f,-0.788346f,-0.803208f,-0.817585f,
-0.831470f,-0.844854f,-0.857729f,-0.870087f,-0.881921f,-0.893224f,-0.903989f,-0.914210f,
-0.923880f,-0.932993f,-0.941544f,-0.949528f,-0.956940f,-0.963776f,-0.970031f,-0.975702f,
-0.980785f,-0.985278f,-0.989177f,-0.992480f,-0.995185f,-0.997290f,-0.998795f,-0.999699f,
-1.000000f,-0.999699f,-0.998795f,-0.997290f,-0.995185f,-0.992480f,-0.989177f,-0.985278f,
-0.980785f,-0.975702f,-0.970031f,-0.963776f,-0.956940f,-0.949528f,-0.941544f,-0.932993f,
-0.923880f,-0.914210f,-0.903989f,-0.893224f,-0.881921f,-0.870087f,-0.857729f,-0.844854f,
-0.831470f,-0.817585f,-0.803208f,-0.788346f,-0.773010f,-0.757209f,-0.740951f,-0.724247f,
-0.707107f,-0.689541f,-0.671559f,-0.653173f,-0.634393f,-0.615232f,-0.595699f,-0.575808f,
-0.555570f,-0.534998f,-0.514103f,-0.492898f,-0.471397f,-0.449611f,-0.427555f,-0.405241f,
-0.382683f,-0.359895f,-0.336890f,-0.313682f,-0.290285f,-0.266713f,-0.242980f,-0.219101f,
-0.195090f,-0.170962f,-0.146730f,-0.122411f,-0.098017f,-0.073565f,-0.049068f,-0.024541f};

#define YAY 40.743665431525205956834243423364f
//(YAY = 256/2PI)

float atanTable[256] = {
0.000000f, 0.003906f, 0.007812f, 0.011718f, 0.015624f, 0.019529f, 0.023433f, 0.027337f,
0.031240f, 0.035142f, 0.039043f, 0.042942f, 0.046841f, 0.050738f, 0.054633f, 0.058527f,
0.062419f, 0.066309f, 0.070197f, 0.074083f, 0.077967f, 0.081848f, 0.085727f, 0.089603f,
0.093477f, 0.097348f, 0.101215f, 0.105080f, 0.108942f, 0.112800f, 0.116655f, 0.120507f,
0.124355f, 0.128199f, 0.132040f, 0.135876f, 0.139709f, 0.143537f, 0.147361f, 0.151181f,
0.154997f, 0.158808f, 0.162614f, 0.166415f, 0.170212f, 0.174004f, 0.177790f, 0.181572f,
0.185348f, 0.189119f, 0.192884f, 0.196644f, 0.200399f, 0.204147f, 0.207890f, 0.211627f,
0.215358f, 0.219083f, 0.222801f, 0.226514f, 0.230220f, 0.233919f, 0.237612f, 0.241299f,
0.244979f, 0.248652f, 0.252318f, 0.255977f, 0.259630f, 0.263275f, 0.266913f, 0.270544f,
0.274167f, 0.277784f, 0.281392f, 0.284994f, 0.288587f, 0.292173f, 0.295752f, 0.299322f,
0.302885f, 0.306440f, 0.309986f, 0.313525f, 0.317056f, 0.320578f, 0.324092f, 0.327598f,
0.331096f, 0.334585f, 0.338066f, 0.341538f, 0.345002f, 0.348457f, 0.351904f, 0.355342f,
0.358771f, 0.362191f, 0.365602f, 0.369005f, 0.372398f, 0.375783f, 0.379159f, 0.382525f,
0.385883f, 0.389231f, 0.392570f, 0.395900f, 0.399221f, 0.402532f, 0.405834f, 0.409127f,
0.412410f, 0.415684f, 0.418949f, 0.422204f, 0.425450f, 0.428686f, 0.431912f, 0.435129f,
0.438337f, 0.441534f, 0.444722f, 0.447901f, 0.451070f, 0.454229f, 0.457378f, 0.460518f,
0.463648f, 0.466768f, 0.469878f, 0.472979f, 0.476069f, 0.479150f, 0.482221f, 0.485283f,
0.488334f, 0.491375f, 0.494407f, 0.497429f, 0.500441f, 0.503443f, 0.506435f, 0.509417f,
0.512389f, 0.515352f, 0.518304f, 0.521247f, 0.524180f, 0.527102f, 0.530015f, 0.532918f,
0.535811f, 0.538694f, 0.541568f, 0.544431f, 0.547284f, 0.550128f, 0.552962f, 0.555785f,
0.558599f, 0.561403f, 0.564198f, 0.566982f, 0.569756f, 0.572521f, 0.575276f, 0.578021f,
0.580756f, 0.583482f, 0.586198f, 0.588904f, 0.591600f, 0.594286f, 0.596963f, 0.599630f,
0.602287f, 0.604935f, 0.607573f, 0.610201f, 0.612820f, 0.615429f, 0.618029f, 0.620619f,
0.623199f, 0.625770f, 0.628332f, 0.630883f, 0.633426f, 0.635959f, 0.638482f, 0.640996f,
0.643501f, 0.645996f, 0.648482f, 0.650959f, 0.653426f, 0.655884f, 0.658333f, 0.660773f,
0.663203f, 0.665624f, 0.668036f, 0.670439f, 0.672833f, 0.675217f, 0.677593f, 0.679959f,
0.682317f, 0.684665f, 0.687004f, 0.689335f, 0.691657f, 0.693969f, 0.696273f, 0.698568f,
0.700854f, 0.703132f, 0.705400f, 0.707660f, 0.709912f, 0.712154f, 0.714388f, 0.716613f,
0.718830f, 0.721038f, 0.723238f, 0.725429f, 0.727611f, 0.729785f, 0.731951f, 0.734108f,
0.736257f, 0.738398f, 0.740530f, 0.742654f, 0.744770f, 0.746878f, 0.748977f, 0.751068f,
0.753151f, 0.755226f, 0.757293f, 0.759352f, 0.761403f, 0.763446f, 0.765480f, 0.767507f,
0.769526f, 0.771538f, 0.773541f, 0.775537f, 0.777524f, 0.779504f, 0.781477f, 0.783441f};

float bsin(float f)
{
#ifdef QUICK_N_DIRTY
	unsigned char c = (int)(f*YAY)&0xFF;
	return SinTable[c];
#else
	//add linear interpolation
	unsigned char c1,c2;
	float l;
	f*=YAY;
	c1 = (int)(f)&0xFF;
	c2 = (int)(f+1)&0xFF;
	if(f<0)f-=((int)f-2);								//(int) truncing has problems with negatives
	l= f - (int)f;
	return SinTable[c1]*(1-l)+SinTable[c2]*l;
#endif
}

float bcos(float f)
{
#ifdef QUICK_N_DIRTY
	unsigned char c = (int)(f*YAY+64)&0xFF;
	return SinTable[c];
#else
	//add linear interpolation
	unsigned char c1,c2;
	float l;
	f*=YAY;
	c1 = (int)(f+64)&0xFF;
	c2 = (int)(f+65)&0xFF;
	if(f<0)f-=((int)f-2);								//(int) truncing has problems with negatives
	l= f - (int)f;
	return SinTable[c1]*(1-l)+SinTable[c2]*l;
#endif
}


float batan(float f)
{
	int s = 0;
	int q = 0;
	int i = 0;
	
	if(f<0){f=-f;s=1;}
	q = f<1 ? 1:0;
	i = q? (int)(f*256.f) : (int)(256.f/f);

	if(i>255)i=255;
	f=atanTable[i];
	if(!q)f = 3.1415926535897932384626433832795f/2 - f;
	if(s)f = -f;
	return f;
}

inline float bsqr(float f)
{
	return f*f;
}


// Wilco Dijkstra also provided the following C code which
// produces optimised ARM code which takes 4 cycles per bit:
#define iter1(N) \
		try = root + (1 << (N)); \
		if (n >= try << (N)) { \
                 n -= try << (N); \
                 root |= 2 << (N); \
		}

long int isqrt(long int n)
{
	long int root = 0, try;
	iter1(15); iter1(14); iter1(13); iter1(12);
	iter1(11); iter1(10); iter1( 9); iter1( 8);
	iter1( 7); iter1( 6); iter1( 5); iter1( 4);
    iter1( 3); iter1( 2); iter1( 1); iter1( 0);
    return root >> 1;
}

// adapts and call previous isqrt for float precision.
float bsqrt( float f )                       
{
	long int i=(int)(f*256.f);              // i*16*16
	i=isqrt(i);
	return i*0.0625f;						// i/16
}


float babs( float f )
{
	return (f>0)?f:-f;
}
//========================================================================//

#ifndef FORCEINLINE

 FIXP fp_optmul(FIXP a,FIXP b)	{INT64 x;		//option optimized for matrix operations.
									 if(!b)				//special easy to handle cases ;)
										 return 0;
									 if(!a)
										 return 0;
									 if(b==1<<PRECISION)
										 return a;
									 if(a==1<<PRECISION)
										 return b;		
									 x=a; x*=b; return (FIXP)(x>>PRECISION);}
 FIXP fp_mul(FIXP a,FIXP b)	{INT64 x;			//don't spend time on easy cases
									 x=a; x*=b; return (FIXP)(x>>PRECISION);}
//inline FIXP fp_mul(FIXP a,FIXP b)	{return (a*b)>>PRECISION;}
 FIXP fp_muli(FIXP a,int b)	{return (a*b);}
 FIXP fp_mulf(FIXP a,float b)	{return (FIXP)(a*b);}

//inline
 FIXP fp_div(FIXP a,FIXP b)	{INT64 x=a; x<<=PRECISION;
									 return (FIXP)(x/b);}	
//inline FIXP fp_div(FIXP a,FIXP b)	{return (a<<PRECISION)/b;}
 FIXP fp_divi(FIXP a,int b)	{return (a/b);}
 FIXP fp_divf(FIXP a,float b)	{return (FIXP)(a/b);}

 FIXP fp_add(FIXP a,FIXP b)	{return	a+b;}
 FIXP fp_addi(FIXP a,int b)	{return a+(b<<PRECISION);}
 FIXP fp_addf(FIXP a,float b)	{return a+(FIXP)(b*(1<<PRECISION));}

 FIXP fp_sub(FIXP a,FIXP b)	{return a-b;}
 FIXP fp_subi(FIXP a,int b)	{return a-(b<<PRECISION);}
 FIXP fp_subf(FIXP a,float b)	{return a-(FIXP)(b*(1<<PRECISION));}

 FIXP fp_convi(int a)			{return a<<PRECISION;}
 FIXP fp_convf(float a)		{return (FIXP)(a*(float)(1<<PRECISION));}

inline int fp_iconv(FIXP a)			{return a>>PRECISION;}
/*inline int fp_iconv(FIXP a)			{if(a<0)
									 return	-((long)((a^0xffffffff)>>PRECISION)+1);
									else
									 return (a>>PRECISION)&((unsigned long)0xffffffff>>PRECISION);}
*/
//inline float fp_fconv(FIXP a)		{return a*(1.f/(float)(1<<PRECISION));}
inline float fp_fconv(FIXP a)		{return (INT64)a/(float)(1<<PRECISION);}

#endif
