//FX library
//there are 2 kinds of fx: modifier and generators
// mod_ will take the screen and change it
// gen_ will completely replace the screen

#include "global.h"
#include "bz_video.h"

//-------------------------------FX
void	fx_updateTime(int time);
//-------------------------------MOD
//blurs a region in the screen
void	mod_blur  (unsigned char* scr, short x1, short y1, short x2, short y2, char rad);
void	mod_blur_mask  (unsigned char* scr, short x1, short y1, short x2, short y2, char rad,bmp32_t *mask,char lvl);

// darken a region of the screen
void    mod_modulate (unsigned char * scr, short x1, short y1, short x2, short y2, unsigned char amount);

//blend a color onto the screen
void    mod_add (unsigned char * scr, short x1, short y1, short x2, short y2, int red,int blue,int green);

// desaturates a region of the screen
void    mod_desat (unsigned char * scr, short x1, short y1, short x2, short y2, unsigned char amount);

//replaces edges with ansi pipettes 
void	mod_edge 
(unsigned char* scr, //input screen in packed RGB
					  char* out, //output ascii in high ANSI (use drawASCII)
					  short x1, short y1, //upper corner 
					  short x2, short y2, //lower right corner
					  unsigned char mid, //mid value (ignored)
					  unsigned char high //high value (ignored)
					  );
//noise on a region of the screen
void	mod_noize (unsigned char* scr, short x1, short y1, short x2, short y2, char amount);
//contrast on a region of the screen
void	mod_level (unsigned char* scr, short x1, short y1, short x2, short y2, short min, short max);
//inverts a portion of the screen
void	mod_invert(unsigned char* scr, short x1, short y1, short x2, short y2);
void	mod_invert_mask(unsigned char* scr, short x1, short y1, short x2, short y2,bmp32_t *mask,char lvl);
//wave distortion	amp frq time speed
void	mod_wave  (unsigned char* scr, float frqx, float frqy, float ampx, float ampy, float spd);
//wind distortion	scale
void	mod_wind(char* scr,float scale1,float scale2,int up, int dn);

//-------------------------------GEN										 
//basic raytraced tunnel	orgx,orgy are the center of the tunnel
void	gen_tunnel(float orgx, float orgy, bmp32_t* tex, char* scr, int blend, float blendFactor, char* scrprev);
void	gen_tunnelAlpha(float orgx, float orgy, bmp32_t* tex, char* scr);
void	gen_rotozoom(float angle, float scale, bmp32_t* tex, char* scr);
void	gen_rotozoomAlpha(float angle, float scale, float scroll, bmp32_t* tex, char* scr);
void	gen_fill(unsigned char* scr, short x1, short y1, short x2, short y2, unsigned int col);