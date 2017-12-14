//FX library
//there are 2 kinds of fx: modifier and generators
// mod_ will take the screen and change it
// gen_ will completely replace the screen
// fx_ are general functions that affect everything else

#include "bz_video.h"
#include "global.h"
#include "bz_math.h"

int gTime;
//-------------------------------FX
void	fx_updateTime(int time)
{
	gTime = time;
}

//-------------------------------MOD
//accessor method
unsigned int timGetPix32b(bmp32_t *img,int x, int y)
{
	unsigned int col;
	int *t;
	int offset;
	t=(int*)img->pix;
	offset = (x+y*img->w);//*SCR_C;//32b is 4 bytes
	col	= t[offset];
	return col;
}
//blurs a region in the screen
void	mod_blur(unsigned char* scr, short x1, short y1, short x2, short y2, unsigned char rad)
{
	unsigned char* bufx=malloc((x2-x1)*SCR_C);
	unsigned char* bufy=malloc((y2-y1)*SCR_C);
	int x,y;
	if(rad<=0)return;
	if(x1<0)x1=0;
	if(x2>SCR_W)x2=SCR_W;
	if(y1<0)y1=0;
	if(y2>SCR_H)y2=SCR_H;
	//horz blur
	for(y=y1;y<y2;y++)
	{
	for(x=x1;x<x2;x++)
	{
		int ofs=(x+y*SCR_W)*SCR_C;
		int ofd=(x-x1)*SCR_C;
		short r=0,g=0,b=0;
		
		int i,n=0;
		for(i=-rad;i<=rad;i++)
		if(x+i>=x1 && x+i<x2)
		{
			r+=scr[ofs+i*SCR_C  ];
			g+=scr[ofs+i*SCR_C+1];
			b+=scr[ofs+i*SCR_C+2];
			n++;
		}
		r/=n;
		g/=n;
		b/=n;

		if(r<0x00)r=0x00;
		if(g<0x00)g=0x00;
		if(b<0x00)b=0x00;
		if(r>0xFF)r=0xFF;
		if(g>0xFF)g=0xFF;
		if(b>0xFF)b=0xFF;
		bufx[ofd  ]=(char)r;
		bufx[ofd+1]=(char)g;
		bufx[ofd+2]=(char)b;
	}
	for(x=x1;x<x2;x++)
	{
		int ofs=(x+y*SCR_W)*SCR_C;
		int ofd=(x-x1)*SCR_C;
		scr[ofs  ]=bufx[ofd  ];
		scr[ofs+1]=bufx[ofd+1];
		scr[ofs+2]=bufx[ofd+2];
	}
	}
	free(bufx);
	//vert blur		
	for(x=x1;x<x2;x++)
	{
	for(y=y1;y<y2;y++)
	{
		int ofs=(x+y*SCR_W)*SCR_C;
		int ofd=(y-y1)*SCR_C;
		short r=0,g=0,b=0;
		
		int i,n=0;
		for(i=-rad;i<=rad;i++)
		if(y+i>=y1 && y+i<y2)
		{
			r+=scr[ofs+i*SCR_W*SCR_C  ];
			g+=scr[ofs+i*SCR_W*SCR_C+1];
			b+=scr[ofs+i*SCR_W*SCR_C+2];
			n++;
		}
		r/=n;
		g/=n;
		b/=n;

		if(r<0x00)r=0x00;
		if(g<0x00)g=0x00;
		if(b<0x00)b=0x00;
		if(r>0xFF)r=0xFF;
		if(g>0xFF)g=0xFF;
		if(b>0xFF)b=0xFF;
		bufy[ofd  ]=(unsigned char)r;
		bufy[ofd+1]=(unsigned char)g;
		bufy[ofd+2]=(unsigned char)b;
	}
	for(y=y1;y<y2;y++)
	{
		int ofs=(x+y*SCR_W)*SCR_C;
		int ofd=(y-y1)*SCR_C;
		scr[ofs  ]=bufy[ofd  ];
		scr[ofs+1]=bufy[ofd+1];
		scr[ofs+2]=bufy[ofd+2];
	}
	}
	free(bufy);
}
void	mod_blur_mask(unsigned char* scr, short x1, short y1, short x2, short y2, unsigned char rad,bmp32_t *mask,char lvl)
{
	int maskCol,maskAlpha,maskR;
	unsigned char* bufx=malloc((x2-x1)*SCR_C);
	unsigned char* bufy=malloc((y2-y1)*SCR_C);
	int x,y;
	if(rad<=0)return;
	if(x1<0)x1=0;
	if(x2>SCR_W)x2=SCR_W;
	if(y1<0)y1=0;
	if(y2>SCR_H)y2=SCR_H;
	//horz blur
	for(y=y1;y<y2;y++)
	{
	for(x=x1;x<x2;x++)
	{		
	
		

		int ofs=(x+y*SCR_W)*SCR_C;
		int ofd=(x-x1)*SCR_C;
		short r=0,g=0,b=0;
		
		int i,n=0;


		for(i=-rad;i<=rad;i++)
		if(x+i>=x1 && x+i<x2)
		{
			r+=scr[ofs+i*SCR_C  ];
			g+=scr[ofs+i*SCR_C+1];
			b+=scr[ofs+i*SCR_C+2];
			n++;
		}
		r/=n;
		g/=n;
		b/=n;

		if(r<0x00)r=0x00;
		if(g<0x00)g=0x00;
		if(b<0x00)b=0x00;
		if(r>0xFF)r=0xFF;
		if(g>0xFF)g=0xFF;
		if(b>0xFF)b=0xFF;
		bufx[ofd  ]=(char)r;
		bufx[ofd+1]=(char)g;
		bufx[ofd+2]=(char)b;
	}
	for(x=x1;x<x2;x++)
	{
		int ofs=(x+y*SCR_W)*SCR_C;
		int ofd=(x-x1)*SCR_C;		
		//mask
		maskCol=timGetPix32b(mask,x,y);//
		maskR=(maskCol&0xff000000)>>24;
		maskAlpha = maskCol&0xff;
		if(maskR<lvl)//level is do not draw, anything else is draw
			continue;

		scr[ofs  ]=bufx[ofd  ];
		scr[ofs+1]=bufx[ofd+1];
		scr[ofs+2]=bufx[ofd+2];
	}
	}
	free(bufx);
	//vert blur		
	for(x=x1;x<x2;x++)
	{
	for(y=y1;y<y2;y++)
	{
		int ofs=(x+y*SCR_W)*SCR_C;
		int ofd=(y-y1)*SCR_C;
		short r=0,g=0,b=0;
		
		int i,n=0;
	
		for(i=-rad;i<=rad;i++)
		if(y+i>=y1 && y+i<y2)
		{
			r+=scr[ofs+i*SCR_W*SCR_C  ];
			g+=scr[ofs+i*SCR_W*SCR_C+1];
			b+=scr[ofs+i*SCR_W*SCR_C+2];
			n++;
		}
		r/=n;
		g/=n;
		b/=n;

		if(r<0x00)r=0x00;
		if(g<0x00)g=0x00;
		if(b<0x00)b=0x00;
		if(r>0xFF)r=0xFF;
		if(g>0xFF)g=0xFF;
		if(b>0xFF)b=0xFF;
		bufy[ofd  ]=(unsigned char)r;
		bufy[ofd+1]=(unsigned char)g;
		bufy[ofd+2]=(unsigned char)b;
	}
	for(y=y1;y<y2;y++)
	{
		int ofs=(x+y*SCR_W)*SCR_C;
		int ofd=(y-y1)*SCR_C;
		//mask
		maskCol=timGetPix32b(mask,x,y);//
		maskR=(maskCol&0xff000000)>>24;
		maskAlpha = maskCol&0xff;
		if(maskR<lvl)//level is do not draw, anything else is draw
			continue;
		scr[ofs  ]=bufy[ofd  ];
		scr[ofs+1]=bufy[ofd+1];
		scr[ofs+2]=bufy[ofd+2];
	}
	}
	free(bufy);
}
// left upper
int kernelLU[3][3]=
{
{0,0,0},
{0,1,1},
{0,1,1}};
//left middle
int kernelLM[3][3]=
{
{0,1,1},
{0,1,1},
{0,1,1}};
//left bottom
int kernelLB[3][3]=
{
{0,1,1},
{0,1,1},
{0,0,0}};
//center upper 
int kernelCU[3][3]=
{
{0,0,0},
{1,1,1},
{1,1,1}};
//center middle
int kernelCM[3][3]=
{
{1,1,1},
{1,1,1},
{1,1,1}};
//center bottom
int kernelCB[3][3]=
{
{1,1,1},
{1,1,1},
{0,0,0}};
//right upper
int kernelRU[3][3]=
{
{0,0,0},
{1,1,0},
{1,1,0}};
//right middle
int kernelRM[3][3]=
{
{1,1,0},
{1,1,0},
{1,1,0}};
//right bottom
int kernelRB[3][3]=
{
{1,1,0},
{1,1,0},
{0,0,0}};
//cross
int kernelX[3][3]=
{
{0,1,0},
{1,1,1},
{0,1,0}};
int XNOR(int a, int b)
{
	if(a==0 && b == 0)
		return 1;
}
//tell us which kernel we matched
int matchKernel(unsigned char* scr,int xp, int yp)
{
	//right now we'll just see if there's nothing or anything in it

	int match[11]={0,0,0,0,0,0,0,0,0,0,0};//includes the null match
	int x,y,i;
	int col;
	int max,maxindex;
	//check each pixel against each kernel
	for(x=0;x<3;x++)
	{
		for(y=0;y<3;y++)
		{
			int ofs=(x+y*SCR_W)*SCR_C;
			short r=scr[ofs  ];
			short g=scr[ofs+1];
			short b=scr[ofs+2];
			col=0;//pixels are either on or off
			if(r>1 || g>1 || b>1 )col=1;//black background assumed
			if(col && kernelLU[x][y])match[1]+=1;
			if(col && kernelLM[x][y])match[2]+=1;
			if(col && kernelLB[x][y])match[3]+=1;
			if(col && kernelCU[x][y])match[4]+=1;
			if(col && kernelCM[x][y])match[5]+=1;
			if(col && kernelCB[x][y])match[6]+=1;
			if(col && kernelRU[x][y])match[7]+=1;
			if(col && kernelRM[x][y])match[8]+=1;
			if(col && kernelRB[x][y])match[9]+=1;
			if(col && kernelX[x][y])match[10]+=1;
		}
	}
	//now return max matches
	max=match[0];
	maxindex=0;
	for(i=0;i<10;i++)
	{
		if(match[i]>max)
		{
			max=match[i];
			maxindex=i;
		}
	}
	return maxindex;
}
void	mod_edge(unsigned char* scr, //input screen in packed RGB
					  char* out, //output ascii in high ANSI (use drawASCII)
					  short x1, short y1, //upper corner 
					  short x2, short y2, //lower right corner
					  unsigned char mid, //mid value (ignored)
					  unsigned char high //high value (ignored)
					  )
{
	int x,y,i;
	unsigned char c;
	if(x1<0)x1=0;
	if(x2>(SCR_W-3))x2=SCR_W-3;
	if(y1<0)y1=0;
	if(y2>(SCR_H-3))y2=SCR_H-3;
	//edge
	i=0;
	for(y=y1;y<y2;y+=3)
	{
		for(x=x1;x<x2;x+=3)
		{	
			c=' ';
			switch (matchKernel(scr,x,y))
			{
				case 0:;break;//no match
				case 1:c=0xc9;break;//upper left
				case 2:c=0xba;break;// left
				case 3:c=0xc8;break;//lower left
				case 4:c=0xcd;break;//upper
				case 5:break;//nothing, really
				case 6:c=0xcd;break;//lower
				case 7:c=0xbb;break;//upper right
				case 8:c=0xba;break;//mid left
				case 9:c=0xbc;break;//lower left
				case 10:c=0xce;break;//cross
			}//switch
			out[i++]=c;
		}//for
	}//for
}//mod_edge_mask

void    mod_modulate (unsigned char * scr, short x1, short y1, short x2, short y2, unsigned char amount) {
	int x,y;
	if(amount<=0)return;
	if(x1<0)x1=0;
	if(x2>SCR_W)x2=SCR_W;
	if(y1<0)y1=0;
	if(y2>SCR_H)y2=SCR_H;
	for(y=y1;y<y2;y++)
	for(x=x1;x<x2;x++)
	{
		int ofs=(x+y*SCR_W)*SCR_C;
		int r=scr[ofs  ];
		int g=scr[ofs+1];
		int b=scr[ofs+2];
		int level = (r + g + b)/3;
		r = r * amount / 255;
		g = g * amount / 255;
		b = b * amount / 255;
		if(r<0x00)r=0x00;
		if(g<0x00)g=0x00;
		if(b<0x00)b=0x00;
		if(r>0xFF)r=0xFF;
		if(g>0xFF)g=0xFF;
		if(b>0xFF)b=0xFF;
		scr[ofs  ]=(unsigned char)r;
		scr[ofs+1]=(unsigned char)g;
		scr[ofs+2]=(unsigned char)b;
	}
}

void    mod_add(unsigned char * scr, short x1, short y1, short x2, short y2, int ri, int gi, int bi) 
{
	int x,y;
	if(x1<0)x1=0;
	if(x2>SCR_W)x2=SCR_W;
	if(y1<0)y1=0;
	if(y2>SCR_H)y2=SCR_H;
	for(y=y1;y<y2;y++)
	for(x=x1;x<x2;x++)
	{
		int ofs=(x+y*SCR_W)*SCR_C;
		int r=scr[ofs  ];
		int g=scr[ofs+1];
		int b=scr[ofs+2];
		r = r + ri;
		g = g + gi;
		b = b + bi;
		if(r<0x00)r=0x00;
		if(g<0x00)g=0x00;
		if(b<0x00)b=0x00;
		if(r>0xFF)r=0xFF;
		if(g>0xFF)g=0xFF;
		if(b>0xFF)b=0xFF;
		scr[ofs  ]=(unsigned char)r;
		scr[ofs+1]=(unsigned char)g;
		scr[ofs+2]=(unsigned char)b;
	}
}

void mod_desat (unsigned char * scr, short x1, short y1, short x2, short y2, unsigned char amount) {
	int x,y;
	if(amount<=0)return;
	if(x1<0)x1=0;
	if(x2>SCR_W)x2=SCR_W;
	if(y1<0)y1=0;
	if(y2>SCR_H)y2=SCR_H;
	for(y=y1;y<y2;y++)
	for(x=x1;x<x2;x++)
	{
		int ofs=(x+y*SCR_W)*SCR_C;
		int r=scr[ofs  ];
		int g=scr[ofs+1];
		int b=scr[ofs+2];
		int level = (r + g + b)/3;
		r = r + (level - r) * amount / 255;
		g = g + (level - g) * amount / 255;
		b = b + (level - b) * amount / 255;
		if(r<0x00)r=0x00;
		if(g<0x00)g=0x00;
		if(b<0x00)b=0x00;
		if(r>0xFF)r=0xFF;
		if(g>0xFF)g=0xFF;
		if(b>0xFF)b=0xFF;
		scr[ofs  ]=(unsigned char)r;
		scr[ofs+1]=(unsigned char)g;
		scr[ofs+2]=(unsigned char)b;
	}
}

//noise on a region of the screen
void	mod_noize(unsigned char* scr, short x1, short y1, short x2, short y2, unsigned char amount)
{
	int x,y;
	if(amount<=0)return;
	if(x1<0)x1=0;
	if(x2>SCR_W)x2=SCR_W;
	if(y1<0)y1=0;
	if(y2>SCR_H)y2=SCR_H;
	for(y=y1;y<y2;y++)
	for(x=x1;x<x2;x++)
	{
		int ofs=(x+y*SCR_W)*SCR_C;
		short r=scr[ofs  ];
		short g=scr[ofs+1];
		short b=scr[ofs+2];
		char n=rand()%amount-(amount/2);
		r+=n;g+=n;b+=n;
		if(r<0x00)r=0x00;
		if(g<0x00)g=0x00;
		if(b<0x00)b=0x00;
		if(r>0xFF)r=0xFF;
		if(g>0xFF)g=0xFF;
		if(b>0xFF)b=0xFF;
		scr[ofs  ]=(unsigned char)r;
		scr[ofs+1]=(unsigned char)g;
		scr[ofs+2]=(unsigned char)b;
	}
}



//contrast on a region of the screen
void	mod_level(unsigned char* scr, short x1, short y1, short x2, short y2, short min, short max)
{
	int x,y;
	if(x1<0)x1=0;
	if(x2>SCR_W)x2=SCR_W;
	if(y1<0)y1=0;
	if(y2>SCR_H)y2=SCR_H;
	for(y=y1;y<y2;y++)
	for(x=x1;x<x2;x++)
	{
		int ofs=(x+y*SCR_W)*SCR_C;
		short r=scr[ofs  ];
		short g=scr[ofs+1];
		short b=scr[ofs+2];
		float f=256.f/(max-min);
		r=(short)(r*f-min);
		g=(short)(g*f-min);
		b=(short)(b*f-min);
		if(r<0x00)r=0x00;
		if(g<0x00)g=0x00;
		if(b<0x00)b=0x00;
		if(r>0xFF)r=0xFF;
		if(g>0xFF)g=0xFF;
		if(b>0xFF)b=0xFF;
		scr[ofs  ]=(unsigned char)r;
		scr[ofs+1]=(unsigned char)g;
		scr[ofs+2]=(unsigned char)b;
	}
}

//inverts a portion of the screen
void	mod_invert(unsigned char* scr, short x1, short y1, short x2, short y2)
{
	int x,y;
	if(x1<0)x1=0;
	if(x2>SCR_W)x2=SCR_W;
	if(y1<0)y1=0;
	if(y2>SCR_H)y2=SCR_H;
	for(y=y1;y<y2;y++)
	for(x=x1;x<x2;x++)
	{
		int ofs=(x+y*SCR_W)*SCR_C;
		unsigned short r=scr[ofs  ];
		unsigned short g=scr[ofs+1];
		unsigned short b=scr[ofs+2];
		r=0xFF-r;
		g=0xFF-g;
		b=0xFF-b;
		scr[ofs  ]=(unsigned char)r;
		scr[ofs+1]=(unsigned char)g;
		scr[ofs+2]=(unsigned char)b;
	}
}//inverts a portion of the screen

void	mod_invert_mask(unsigned char* scr, short x1, short y1, short x2, short y2,bmp32_t *mask,char lvl)
{
	int x,y,maskCol,maskAlpha,maskR;
	if(x1<0)x1=0;
	if(x2>SCR_W)x2=SCR_W;
	if(y1<0)y1=0;
	if(y2>SCR_H)y2=SCR_H;
	for(y=y1;y<y2;y++)
	for(x=x1;x<x2;x++)
	{
		int ofs=(x+y*SCR_W)*SCR_C;
		unsigned short r=scr[ofs  ];
		unsigned short g=scr[ofs+1];
		unsigned short b=scr[ofs+2];
		maskCol=timGetPix32b(mask,x,y);//
		maskR=(maskCol&0xff000000)>>24;
		maskAlpha = maskCol&0xff;
		if(maskR>lvl)//half is do not draw, anything else is draw
		{
			r=0xFF-r;//maskAlpha-r for alpha blending \o/
			g=0xFF-g;
			b=0xFF-b;
		}
		scr[ofs  ]=(unsigned char)r;
		scr[ofs+1]=(unsigned char)g;
		scr[ofs+2]=(unsigned char)b;
	}
}

//wave distortion	amp frq time
void	mod_wave(char* scr,float frqx, float frqy, float ampx, float ampy, float spd)
{
	int* s = (int*)scr;
	static int d[SCR_W*SCR_H];
	int x,y,x2,y2,ofs,ofd;
	float t=gTime*spd;
	//distort it
	for(y=0;y<SCR_H;y++)
	{
	for(x=0;x<SCR_W;x++)
	{
		y2 =(int)(y+(/*bcos(x*frqx+t)**/bcos(y*frqy+t))*ampy);
		x2 =(int)(x+(/*bsin(y*frqy+t)**/bsin(x*frqx+t))*ampx); 

		if(x2<0)x2=0;
		if(x2>SCR_W-1)x2=SCR_W-1;
		if(y2<0)y2=0;
		if(y2>SCR_H-1)y2=SCR_H-1;

		ofs=x2+y2*SCR_W;
		ofd=x+y*SCR_W;
		d[ofd] = s[ofs];
	}
	}
	//flip it
	for(x=0;x<SCR_W*SCR_H;x++)
		s[x]=d[x];
}

//wind distortion	amp frq time
void	mod_wind(char* scr,float scale1,float scale2,int up, int dn)
{
	int* s = (int*)scr;
	static int d[SCR_W*SCR_H];
	int ofs,ofd,x,y,tofs;
	ofd=ofs=tofs=0;
	for(y=0;y<SCR_H;y++)
	{
		if(!(rand()%dn)){
			int a=(int)((rand()%16 -7)*scale2);
			if(tofs)
			{
				if(tofs<0)
					if(a<tofs)a=tofs;
				if(tofs>0)
					if(a>tofs)a=tofs;
				ofs-= a;
				tofs-=a;
			}
		}
		if(!(rand()%up))
			ofs+= tofs=(int)((rand()%16 -7)*scale1);

		if(ofs>=SCR_W*SCR_H)ofs=SCR_W*SCR_H-1;
		for(x=0;x<SCR_W;x++)
		{
			if(ofs>=0)	
				d[ofd++] = 
				s[ofs++];
	//		d[ofd++] = s[ofs++];
			if(ofs>=SCR_W*SCR_H)ofs=SCR_W*SCR_H-1;
		}
	}
/*
	int x,y,x2,y2,ofs,ofd;
	float t=gTime*spd;
	//distort it
	for(y=0;y<SCR_H;y++)
	{
	for(x=0;x<SCR_W;x++)
	{
		y2 =(int)(y+bcos(x*frqx+t)+bcos(y*frqy+t)*ampy);
		x2 =(int)(x+bsin(y*frqy+t)+bsin(x*frqx+t)*ampx); 

		if(x2<0)x2=0;
		if(x2>SCR_W-1)x2=SCR_W-1;
		if(y2<0)y2=0;
		if(y2>SCR_H-1)y2=SCR_H-1;

		ofs=x2+y2*SCR_W;
		ofd=x+y*SCR_W;
		d[ofd] = s[ofs];
	}
	}							*/
	//flip it
	for(x=0;x<SCR_W*SCR_H;x++)
		s[x]=d[x];	  
}


//-------------------------------GEN

// rotozoom effect - by polaris
// spr_x,spr_y - top left corner of the sprite

void gen_rotozoom(float angle, float scale, bmp32_t* tex, char* scr)
{
    float u,v,rowU,rowV;
    float duCol,dvCol,duRow,dvRow;
    
    int x,y;
    int spr_x,spr_y;

	int* s = (int*)scr;
	int* t = (int*)tex->pix;
	   
	// pitch in target bitmap 
    duCol=(float)bcos(angle)*scale;
    dvCol=(float)bsin(angle)*scale;
    
	// (similar triangles) / aspect ratio correction 
    duRow=-dvCol*(SCR_W/SCR_H);
    dvRow=duCol*(SCR_W/SCR_H);
    
    rowU = ((float)tex->w)/2-((SCR_W/2)*duCol + (SCR_H/2)*duRow);
    rowV = ((float)tex->h)/2-((SCR_W/2)*dvCol + (SCR_H/2)*dvRow);
    	
    for(y=0;y<SCR_H;y++)
    {
        u=rowU;
        v=rowV;
        
        for(x=0;x<SCR_W;x++)
        {
			spr_x=((int)u)&(tex->h-1);
			spr_y=((int)v)&(tex->w-1);

			s[y*SCR_W+x] = t[spr_y*tex->w+spr_x];

			u+=duCol;
			v+=dvCol;        
        }
        
        rowU+=duRow;
        rowV+=dvRow;
    }
}

void gen_rotozoomAlpha(float angle, float scale, float scroll, bmp32_t* tex, char* scr)
{
    float u,v,rowU,rowV;
    float duCol,dvCol,duRow,dvRow;
    
    int x,y;
    int spr_x,spr_y;

	BYTE* s = scr;
	BYTE* t = tex->pix;
	   
	// pitch in target bitmap 
    duCol=(float)bcos(angle)*scale;
    dvCol=(float)bsin(angle)*scale;
    
	// (similar triangles) / aspect ratio correction 
    duRow=-dvCol*(SCR_W/SCR_H);
    dvRow=duCol*(SCR_W/SCR_H);
    
    rowU = ((float)tex->w)/2-((SCR_W/2)*duCol + (SCR_H/2)*duRow)+scroll;
    rowV = ((float)tex->h)/2-((SCR_W/2)*dvCol + (SCR_H/2)*dvRow)+scroll;
    	
    for(y=0;y<SCR_H;y++)
    {
        u=rowU;
        v=rowV;
        
        for(x=0;x<SCR_W;x++)
        {
			WORD a,r,g,b;
			int ofs,ofd;
			spr_x=((int)u)&(tex->h-1);
			spr_y=((int)v)&(tex->w-1);
			
			ofs=(spr_y*tex->w+spr_x)*SCR_C;
			ofd=(y*SCR_W+x)*SCR_C;

			a = t[ofs+3];
			r = t[ofs  ];
			g = t[ofs+1];
			b = t[ofs+2];

			s[ofd  ] = (s[ofd  ]*(0xff-a) + r*a)>>8;
			s[ofd+1] = (s[ofd+1]*(0xff-a) + g*a)>>8;
			s[ofd+2] = (s[ofd+2]*(0xff-a) + b*a)>>8;
	
			u+=duCol;
			v+=dvCol;        
        }
        
        rowU+=duRow;
        rowV+=dvRow;
    }
}

//basic raytraced tunnel	orgx,orgy are the center of the tunnel
//tex size MUST be power of 2
void	gen_tunnel(float orgx, float orgy, bmp32_t* tex, char* scr, int blend, float blendFactor, char* scrprev)
{
	//tunnel radius
	#define TUN_RAD 200
	//cam distance from the screen
	#define CAM_DST 80
	int x,y;
	int* s;
	int* t;
	int* sp;
	int c;
	char r,g,b,a;
	float fog;
	s=(int*)scr;
	sp=(int*)scrprev;//previous - this is our accumulation buffer
	t=(int*)tex->pix;
	for(y=0;y<SCR_H;y++)
	{
		float yoy2=(y-orgy)*(y-orgy);
	for(x=0;x<SCR_W;x++)
	{		
		float angle;
		float wobble;
		
		//vector length
		float xox2=(x-orgx)*(x-orgx);

		float vl;
		float u,v;
		int tx,ty;
		float fudge;

		if(y==x)angle=3.1416f/4.f;
		if(x==0)
			angle=3.1416f/2.f;
		else
			angle = (float)batan((1.f*(y-orgy))/(1.f*(x-orgx)));
		vl=(float)bsqrt(xox2 + yoy2);
		//u is based on the distance where the screen ray and the cylinder meet
		//since u is the distance, it's easy to add fog ;)
		
		wobble=((float)bsin(angle*6.0f));//*(0.5+0.5*bsin(gTime/1000.0f));
		wobble *=(0.5+0.5*bsin(gTime/1000.0f));
		u=(TUN_RAD/vl*CAM_DST)+100.0*wobble;

		//v is based on the angle of the vector
		if(y-orgy==0)orgy+=.1f;
		v=(float)batan((x-orgx)/(y-orgy))/6.2832f +gTime*0.0001f;

		//convert to texture coords
		tx=(int)((u+gTime*.45f)*tex->w*0.004f)&(tex->w-1);	//tiled 4 times
		ty=(int)(v*tex->h*4.f)&(tex->h-1);		//scale factor of 2 ... totally random ;)
		

		//spin=40.0*bsin((int)((float)(gTime)/1000.0)%256);
		//tx=tx+spin;
		//while(tx<0)tx+=64;
		//while(tx>64)tex-=64;
		//black fog ramp
		//far end is 1000
		fog=1-((u+1000)/2000);
		fudge=2;
		fog *= fudge;
		fog-=wobble/3.0f;
		if(fog<0)
			c=0;
		else
		{
			c = t[ty*tex->w+tx];
			r = fog*((c&0xf000)>>12);
			g = fog*((c&0x0f00)>>8);
			b = fog*((c&0x00f0)>>4);
			a = (c&0x000f);
			c = (r&0x000f)<<12;
			c +=(g&0x000f)<<8;
			c +=(b&0x000f)<<4;
		}
		//blend with previuos frame
		if(blend)
		{
			//unpack color from previous frame and blend with this one. 
			int c1=sp[y*SCR_W+x];
			r=((c&0xf000)>>13)+((c1&0xf000)>>13);
			g=((c&0x0f00)>>9)+ ((c1&0x0f00)>>9);
			b=((c&0x00f0)>>5)+ ((c1&0x00f0)>>5);
			c = (r&0x000f)<<12;
			c +=(g&0x000f)<<8;
			c +=(b&0x000f)<<4;
			//set the result color in the current output frame
			s[y*SCR_W+x] =c;
			//set the result colorin the scrprev texture as well
			sp[y*SCR_W+x] =c;
		}
		else
		{
			//copy data to screen
			s[y*SCR_W+x] =c;
		}
	}
	}
}


//basic raytraced tunnel with Alpha		orgx,orgy are the center of the tunnel
//tex size MUST be power of 2
void	gen_tunnelAlpha(float orgx, float orgy, bmp32_t* tex, char* scr)
{
	//tunnel radius
	#define TUN_RAD 200
	//cam distance from the screen
	#define CAM_DST 80
	int x,y;
	int* s;
	int* t;
	s=(int*)scr;
	t=(int*)tex->pix;
	for(y=0;y<SCR_H;y++)
	{
		float yoy2=(y-orgy)*(y-orgy);
	for(x=0;x<SCR_W;x++)
	{
		//vector length
		float xox2=(x-orgx)*(x-orgx);
		float vl=(float)bsqrt(xox2 + yoy2);
		float u,v;
		int tx,ty;
		short a,r,g,b;
		int ofs,ofd;

		//u is based on the distance where the screen ray and the cylinder meet
		//since u is the distance, it's easy to add fog ;)
		u=(TUN_RAD/vl*CAM_DST);

		//v is based on the angle of the vector
		if(y-orgy==0)orgy+=.1f;
		v=(float)batan((x-orgx)/(y-orgy))/6.2832f -gTime*0.00001f;

		//convert to texture coords
		tx=(int)((u-gTime*.1f)*tex->w*0.004f)&(tex->w-1);	//tiled 4 times
		ty=(int)(v*tex->h*4.f)&(tex->h-1);		//scale factor of 2 ... totally random ;)
		
		//copy data to screen
		ofs=(y*SCR_W+x)*4;
		ofd=(ty*tex->w+tx)*4;

		a = t[ofs+3];
		r = t[ofs  ];
		g = t[ofs+1];
		b = t[ofs+2];

		s[ofd  ] = (s[ofd  ]*(0xff-a) + r*a)>>8;
		s[ofd+1] = (s[ofd+1]*(0xff-a) + g*a)>>8;
		s[ofd+2] = (s[ofd+2]*(0xff-a) + b*a)>>8;
	}
	}
}

void	gen_fill(unsigned char* scr, short x1, short y1, short x2, short y2, unsigned int col)
{
	int x,y;
	unsigned int * d = (int*)scr;
	if(x1<0)x1=0;
	if(x2>SCR_W)x2=SCR_W;
	if(y1<0)y1=0;
	if(y2>SCR_H)y2=SCR_H;
	for(y=y1;y<y2;y++)
	for(x=x1;x<x2;x++)
	{
		int ofs=(x+y*SCR_W);
		d[ofs] = col;
	}
}

