/**
 \file id_vh.h
 
  new version of ID_VH.H

  

*/

#ifndef __ID_VH_H__
#define __ID_VH_H__
/**

   Include section

*/
#include "id_heads.h" // type definitions
#include "ID_IN.H"    // Input Manager Header, used for ControlInfo structure


/**
   
  Define section

*/
#define WHITE			15			// graphics mode independant colors
#define BLACK			0
#define FIRSTCOLOR		1
#define SECONDCOLOR		12
#define F_WHITE			15
#define F_BLACK			0
#define F_FIRSTCOLOR	1
#define F_SECONDCOLOR	12

#define MAXSHIFTS	1

/*
 All define values need to be changed to 'const' values

 Ex 

 cont short int SCREENWIDTH = 80;


*/

#define SCREENWIDTH             80
#define CHARWIDTH               2
#define TILEWIDTH               4
#define GRPLANES                4
#define BYTEPIXELS              4

#define SCREENXMASK             (~3)
#define SCREENXPLUS             (3)
#define SCREENXDIV              (4)

#define VIEWWIDTH               80

#define PIXTOBLOCK              4               // 16 pixels to an update block



//
// wolfenstein EGA compatability stuff
//
#define VW_Startup		VL_Startup
#define VW_Shutdown		VL_Shutdown
#define VW_SetCRTC		VL_SetCRTC
#define VW_SetScreen		VL_SetScreen
#define VW_Bar			VL_Bar
#define VW_Plot			VL_Plot
#define VW_Hlin(x,z,y,c)	VL_Hlin(x,y,(z)-(x)+1,c)
#define VW_Vlin(y,z,x,c)	VL_Vlin(x,y,(z)-(y)+1,c)
#define VW_DrawPic		VH_DrawPic
#define VW_SetSplitScreen	VL_SetSplitScreen
#define VW_SetLineWidth		VL_SetLineWidth
#define VW_ColorBorder	VL_ColorBorder
#define VW_WaitVBL		VL_WaitVBL
#define VW_FadeIn()		VL_FadeIn(0,255,&gamepal,30);
#define VW_FadeOut()	VL_FadeOut(0,255,0,0,0,30);
#define VW_ScreenToScreen	VL_ScreenToScreen
#define VW_SetDefaultColors	VH_SetDefaultColors
#define EGAMAPMASK(x)	VGAMAPMASK(x)
#define EGAWRITEMODE(x)	VGAWRITEMODE(x)
#define MS_Quit			Quit

#define NUMLATCHPICS	100

/**
  Macros section
*/
#define LatchDrawChar(x,y,p) VL_LatchToScreen(latchpics[0]+(p)*16,2,8,x,y)
#define LatchDrawTile(x,y,p) VL_LatchToScreen(latchpics[1]+(p)*64,4,16,x,y)

#define UNCACHEGRCHUNK(chunk)   {MM_FreePtr(&grsegs[chunk]);grneeded[chunk]&=~ca_levelbit;}


/*
  Struct section
*/
typedef struct
{
  int width;
  int height;
  int orgxi;
  int orgy;
  int xl;
  int yl;
  int xh;
  int yh;
  int shifts;
} spritetabletype;

typedef	struct
{
	unsigned	sourceoffset[MAXSHIFTS];
	unsigned	planesize[MAXSHIFTS];
	unsigned	width[MAXSHIFTS];
	unsigned char 	*data;
} spritetype;		// the memptr for each sprite points to this

typedef struct
{
	int width;
	int height;
} pictabletype;


typedef struct
{
	int height;
	int location[256];
	char width[256];
} fontstruct;


/**

   Extern variables section

*/
extern	pictabletype	 *picmtable;
extern	spritetabletype  *spritetable;
extern void *grsegs[NUMCHUNKS]; /**<< different value definition for NUMCHUNKS, take care */

//
// wolfenstein EGA compatability stuff
//
extern byte gamepal;
extern	unsigned int latchpics[NUMLATCHPICS];
extern	unsigned int freelatch;
extern	unsigned int bufferofs;
extern	unsigned int ylookup[];

extern ControlInfo c;


/**

  Header's function section

*/

//
// Double buffer management routines
//
void VW_InitDoubleBuffer (void);
int  VW_MarkUpdateBlock (int x1, int y1, int x2, int y2);
void VW_UpdateScreen (void);

//
// mode independant routines
// coordinates in pixels, rounded to best screen res
// regions marked in double buffer
//
void VWB_DrawTile8 (int x, int y, int tile);
void VWB_DrawTile8M (int x, int y, int tile);
void VWB_DrawTile16 (int x, int y, int tile);
void VWB_DrawTile16M (int x, int y, int tile);
void VWB_DrawPic (int x, int y, int chunknum);
void VWB_DrawMPic(int x, int y, int chunknum);
void VWB_Bar (int x, int y, int width, int height, int color);

void VWB_DrawPropString	 (char  *string);
void VWB_DrawMPropString (char  *string);
void VWB_DrawSprite (int x, int y, int chunknum);
void VWB_Plot (int x, int y, int color);
void VWB_Hlin (int x1, int x2, int y, int color);
void VWB_Vlin (int y1, int y2, int x, int color);


//
// wolfenstein EGA compatability stuff
//
void VH_SetDefaultColors (void);
void VW_MeasurePropString (char  *string, word *width, word *height);

void LatchDrawPic (unsigned x, unsigned y, unsigned picnum);
void LoadLatchMem (void);
boolean	FizzleFade (unsigned source, unsigned dest,
unsigned width,unsigned height, unsigned frames,boolean abortable);

void    VWL_UpdateScreenBlocks (void);


/**

  Global variables section

*/
byte update[UPDATEHIGH][UPDATEWIDE];
pictabletype *pictable;
int px; 
int py;
byte fontcolor;
byte backcolor;
int fontnumber;
int bufferwidth;
int bufferheight;




#endif//__ID_VH_H__

