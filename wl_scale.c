/**
\filename wl_scale.c 

This file is editing by Oscar Aaron Revilla Escalona.
  
*/


#include "wl_def.h"
#pragma hdrstop

//#define OP_RETF	0xcb
/*constant*/ unsigned char OP_RETF = 0xcb;
/*
=============================================================================

						  GLOBALS

=============================================================================
*/

/*
 Maybe we can send the global variables by reference 
*/

t_compscale *scaledirectory[MAXSCALEHEIGHT+1];
long fullscalefarcall[MAXSCALEHEIGHT+1];
int maxscale = 0;
int maxscaleshl2 = 0;
boolean	insetupscaling;

/*
=============================================================================

						  LOCALS

=============================================================================
*/

t_compscale  *work = NULL;
unsigned BuildCompScale (int height, memptr *finalspot);

int stepbytwo = 0;

//===========================================================================

/*
==============
=
= BadScale
=
==============
*/

void BadScale (void)
{
	Quit ("BadScale called!");
}


/*
==========================
=
= SetupScaling
=
==========================
*/

void SetupScaling (int maxscaleheight)
{
	int i = 0;
	int x = 0;
	int y = 0;
	unsigned char *dest = NULL;

	insetupscaling = TRUE;

	maxscaleheight/=2;			// one scaler every two pixels

	maxscale = maxscaleheight-1;
	maxscaleshl2 = maxscale<<2;

//
// free up old scalers
//
	for (i=1;i<MAXSCALEHEIGHT;i++)
	{
		if (scaledirectory[i])
			{
			MM_FreePtr (/*&*/(memptr)scaledirectory[i]);
			}
		if (i>=stepbytwo)
			{
			i += 2;
			}
	}
	memset (scaledirectory,0,sizeof(scaledirectory));

	MM_SortMem ();

//
// build the compiled scalers
//
	stepbytwo = viewheight/2;	// save space by double stepping
	MM_GetPtr (/*&*/(memptr)work,20000);

	for (i=1;i<=maxscaleheight;i++)
	{
		BuildCompScale (i*2,/*&*/(memptr)scaledirectory[i]);
		if (i>=stepbytwo)
			i+= 2;
	}
	MM_FreePtr (/*&*/(memptr)work);

//
// compact memory and lock down scalers
//
	MM_SortMem ();
	for (i=1;i<=maxscaleheight;i++)
	{
		MM_SetLock (/*&*/(memptr)scaledirectory[i],TRUE);
		fullscalefarcall[i] = (unsigned)scaledirectory[i];
		fullscalefarcall[i] <<=16;
		fullscalefarcall[i] += scaledirectory[i]->codeofs[0];
		if (i>=stepbytwo)
		{
			scaledirectory[i+1] = scaledirectory[i];
			fullscalefarcall[i+1] = fullscalefarcall[i];
			scaledirectory[i+2] = scaledirectory[i];
			fullscalefarcall[i+2] = fullscalefarcall[i];
			i+=2;
		}
	}
	scaledirectory[0] = scaledirectory[1];
	fullscalefarcall[0] = fullscalefarcall[1];

//
// check for oversize wall drawing
//
	for (i=maxscaleheight;i<MAXSCALEHEIGHT;i++)
		fullscalefarcall[i] = (long)BadScale;

	insetupscaling = FALSE;
}

//===========================================================================

/*
========================
=
= BuildCompScale
=
= Builds a compiled scaler object that will scale a 64 tall object to
= the given height (centered vertically on the screen)
=
= height should be even
=
= Call with
= ---------
= DS:SI		Source for scale
= ES:DI		Dest for scale
=
= Calling the compiled scaler only destroys AL
=
========================
*/

unsigned BuildCompScale (int height, memptr *finalspot)
{
	char far;
	unsigned char *code = NULL;

	int i = 0;
	long fix = 0;
	long step = 0;
	unsigned src = 0;
	unsigned totalscaled = 0;
	unsigned totalsize = 0;
	int startpix = 0;
	int endpix = 0;
	int toppix = 0;


	step = ((long)height<<16) / 64;
	code = &work->code[0];
	toppix = (viewheight-height)/2;
	fix = 0;

	for (src=0;src<=64;src++)
	{
		startpix = fix>>16;
		fix += step;
		endpix = fix>>16;

		if (endpix>startpix)
			work->width[src] = endpix-startpix;
		else
			work->width[src] = 0;

//
// mark the start of the code
//
		work->codeofs[src] = FP_OFF(code);

//
// compile some code if the source pixel generates any screen pixels
//
		startpix+=toppix;
		endpix+=toppix;

		if (startpix == endpix || endpix < 0 || startpix >= viewheight || src == 64)
			continue;

	//
	// mov al,[si+src]
	//
		*code++ = 0x8a;
		*code++ = 0x44;
		*code++ = src;

		for (;startpix<endpix;startpix++)
		{
			if (startpix >= viewheight)
				break;						// off the bottom of the view area
			if (startpix < 0)
				continue;					// not into the view area

		//
		// mov [es:di+heightofs],al
		//
			*code++ = 0x26;
			*code++ = 0x88;
			*code++ = 0x85;
			*((unsigned*)code++) = startpix*SCREENBWIDE;
		}

	}

//
// retf
//
	*code++ = 0xcb;

	totalsize = FP_OFF(code);
	MM_GetPtr (finalspot,totalsize);
	_fmemcpy ((byte *)(*finalspot),(byte *)work,totalsize);

	return totalsize;
}


/*
=======================
=
= ScaleLine
=
= linescale should have the high word set to the segment of the scaler
=
=======================
*/

//Becouse this variables are extern the declaration needs to be in the file where are used.
extern	int slinex ;/* = 0;*/
extern	int slinewidth ;/*= 0;*/
extern	unsigned int far; /*= 0;*/  //is this far variable name different of the other fars?
extern	unsigned *linecmds; /*= NULL;*/
extern	long int linescale ;/*= 0;*/
extern	unsigned int maskword; /*= 0;*/

byte	mask1;
byte	mask2;
byte	mask3;



/*
 *  NOTE: near modifier is used to 
 *  ask the compiler to put the code of the function 'ScaleLine'
 *  to be allacated in the near memory locations.
 *  far is used to refear no far memory locations.
 * */
void  ScaleLine (void)  /*Check how it works this tipe of fuction sign*/
{

/*
  assembly code will change for C language equivalent code. 

              mov destiny, source       destiny <- source
*/

 int CX = 0;
 int ES = 0;
 int BP = 0;
 int DX = 0;
 int BX = 0;
 int SC_INDEX = 0;
 int DI = 0;//destination register
 int AL = 0;
 int AX = 0;
 int SS = 0;
 int DS = 0;

//asm	mov	cx,WORD PTR [linescale+2]    /// 
//No sign lacks the pointer here, as in line 306: CX =(int) *(linescale+2);
CX =(int) (linescale+2);

//asm	mov	es,cx						// segment of scaler
ES = CX;

//asm	mov 	bp,WORD PTR [linecmds]
BP = *(linecmds);

//asm	mov	dx,SC_INDEX+1	// to set SC_MAPMASK
DX = SC_INDEX+1;

//asm	mov	bx,[slinex]
BX = /*'*'*/(slinex);

//asm	mov	di,bx
DI = BX;

//asm	shr	di,2						// X in bytes
DI>>2;

//asm	add	di,[bufferofs]
DI +=/*'*'*/(bufferofs);

//asm	and	bx,3
BX = BX & 0x3;

//asm	shl	bx,3
BX<<2;

//asm	add	bx,[slinewidth]				// bx = (pixel*8+pixwidth)
BX = BX +  /*'*'*/(slinewidth);

AL = *(mapmasks3-1+BX);
DS = *(unsigned short*)(linecmds+2);
if((AL|AL) == 0)
	{
	//nothreebyte
	AL = *(unsigned char*)(mapmasks2-1+BX);
	if((AL|AL) != 0)
		{
		//twobyte
		mask2 = AL;//in case of erro of the var mask2 maybe it make reference to the mapmasks
		AL = *(unsigned char*)(mapmasks1-1+BX);
		mask1 = AL; // same as two lines before
		//scaledouble
		//mov bx,[ds:bp]
		if((BX|BX) == 0)
			{
			//linedone
			AX = SS;//in what time this segmes its loaded?
			DS = AX;
			//return ;
			}
		else
			{
			//if not occurs the jump
			}
		}
	else
		{
		// if not occurs the jump
		}
	}
else
	{
	//threebyte
	mask3 = AL;
	AL = *(unsigned char*)(mapmasks2+-1+BX);
	mask2 = AL;
	AL = *(unsigned char*)(mapmasks1-1+BX);
	mask1 = AL;
	//scaletriple
	//mov bx,[ds:bp]
	if((BX|BX) == 0)
		{
		//linedone
		AX = SS;
		DS = AX;
		//return ;
		}
	else
		{
		//if not occurs the jump
		}
	}

//asm(	"mov	al,BYTE[mapmasks3-1+BX]"	// -1 because pixwidth of 1 is first
//	"mov	ds,WORD PTR[linecmds+2]"
//	"or	al,al"
//	"jz	notthreebyte"				// scale across three bytes
	//"jmp	threebyte"

//"notthreebyte:"
//	"mov	al,BYTE PTR ss:[mapmasks2-1+BX]"	// -1 because pixwidth of 1 is first
// 	"or	al,al"
// 	"jnz	twobyte"						// scale across two bytes

//
// one byte scaling
//
	"mov	al,BYTE PTR ss:[mapmasks1-1+BX]"	// -1 because pixwidth of 1 is first
	"out	dx,al"						// set map mask register

"scalesingle:"

	"mov	bx,[ds:bp]"					// table location of rtl to patch
	"or	bx,bx"
	"jz	linedone"					// 0 signals end of segment list
	"mov	bx,[es:BX]"
	"mov	dl,[es:BX]"					// save old value
	"mov	BYTE PTR es:[BX],OP_RETF"	// patch a RETF in
	"mov	si,[ds:bp+4]"				// table location of entry spot
	"mov	ax,[es:si]"
	"mov	WORD PTR ss:[linescale],ax"	// call here to start scaling
	"mov	si,[ds:bp+2]"				// corrected top of shape for this segment
	"add	bp,6"						// next segment list

	"mov	ax,SCREENSEG"
	"mov	es,ax"
	"call ss:[linescale]"				// scale the segment of pixels

	"mov	es,cx"						// segment of scaler
	"mov	BYTE PTR es:[BX],dl"			// unpatch the RETF
	"jmp	scalesingle"					// do the next segment


//
// done
//
//"linedone:"
//	"mov	ax,ss"
//	"mov	ds,ax"
//"return;"	//this returns goes back from the last jz? or just do nothing?

//
// two byte scaling
//

//"twobyte:"
	//"mov	ss:[mask2],al"
	//"mov	al,BYTE PTR ss:[mapmasks1-1+bx]"	// -1 because pixwidth of 1 is first
	//"mov	ss:[mask1],al"

//"scaledouble:"

//	"mov	bx,[ds:bp]"	//Im not sure if it is an offset bx=bx>> bp	// table location of rtl to patch
//	"or	bx,bx"
//	"jz	linedone"					// 0 signals end of segment list
	"mov	bx,[es:bx]"
	"mov	cl,[es:bx]"					// save old value
	"mov	BYTE PTR es:[bx],OP_RETF"	// patch a RETF in
	"mov	si,[ds:bp+4]"				// table location of entry spot
	"mov	ax,[es:si]"
	"mov	WORD PTR ss:[linescale],ax"	// call here to start scaling
	"mov	si,[ds:bp+2]"				// corrected top of shape for this segment
	"add	bp,6"						// next segment list

	"mov	ax,SCREENSEG"
	"mov	es,ax"
	"mov	al,ss:[mask1]"
	"out	dx,al"						// set map mask register
	"call ss:[linescale]"				// scale the segment of pixels
	"inc	di"
	"mov	al,ss:[mask2]"
	"out	dx,al"						// set map mask register
	"call ss:[linescale]"				// scale the segment of pixels
	"dec	di"
	
	"mov	es,WORD PTR ss:[linescale+2]" // segment of scaler
	"mov	BYTE PTR es:[bx],cl"			// unpatch the RETF
	"jmp	scaledouble"					// do the next segment


//
// three byte scaling
//
//"threebyte:"
//	"mov	ss:[mask3],al"
//	"mov	al,BYTE PTR ss:[mapmasks2-1+bx]"	// -1 because pixwidth of 1 is first
//	"mov	ss:[mask2],al"
//	"mov	al,BYTE PTR ss:[mapmasks1-1+bx]"	// -1 because pixwidth of 1 is first
//	"mov	ss:[mask1],al"

//"scaletriple:"
	"mov	bx,[ds:bp]"					// table location of rtl to patch
	"mov	bx,bx"
	"jz	linedone"					// 0 signals end of segment list
	"mov	bx,[es:bx]"
	"mov	cl,[es:bx]"					// save old value
	"mov	BYTE PTR es:[bx],OP_RETF"	// patch a RETF in
	"mov	si,[ds:bp+4]"				// table location of entry spot
	"mov	ax,[es:si]"
	"mov	WORD PTR ss:[linescale],ax"	// call here to start scaling
	"mov	si,[ds:bp+2]"				// corrected top of shape for this segment
	"add	bp,6"						// next segment list

"	mov	ax,SCREENSEG"
"	mov	es,ax"
"	mov	al,ss:[mask1]"
"	out	dx,al"						// set map mask register
"	call ss:[linescale]"				// scale the segment of pixels
"	inc	di"
"	mov	al,ss:[mask2]"
"	out	dx,al"					// set map mask register
"	call ss:[linescale]"				// scale the segment of pixels
"	inc	di"
"	mov	al,ss:[mask3]"
"	out	dx,al"						// set map mask register
"	call ss:[linescale]"				// scale the segment of pixels
"	dec	di"
"	dec	di"

"	mov	es,WORD PTR ss:[linescale+2]" // segment of scaler
"	mov	BYTE PTR es:[bx],cl"			// unpatch the RETF
"	jmp	scaletriple"					// do the next segment

);

}


/*
=======================
=
= ScaleShape
=
= Draws a compiled shape at [scale] pixels high
=
= each vertical line of the shape has a pointer to segment data:
= 	end of segment pixel*2 (0 terminates line) used to patch rtl in scaler
= 	top of virtual line with segment in proper place
=	start of segment pixel*2, used to jsl into compiled scaler
=	<repeat>
=
= Setup for call
= --------------
= GC_MODE			read mode 1, write mode 2
= GC_COLORDONTCARE  set to 0, so all reads from video memory return 0xff
= GC_INDEX			pointing at GC_BITMASK
=
=======================
*/

static	long longtemp = 0;

void ScaleShape (int xcenter, int shapenum, unsigned height)
{
	t_compshape *shape = NULL;
	t_compscale  *comptable = NULL;
	unsigned scale = 0;
	unsigned srcx = 0;
	unsigned stopx = 0;
	unsigned tempx = 0;
	int 	 t = 0;
	unsigned	*cmdptr = NULL;
	boolean		leftvis,rightvis;/*Search where is the definition of boolean*/


	shape = PM_GetSpritePage (shapenum);

	scale = height>>3;						// low three bits are fractional
	if (!scale || scale>maxscale)
		return;								// too close or far away
	comptable = scaledirectory[scale];

	*(((unsigned *)&linescale)+1) = (unsigned) comptable;	// seg of far call
	*(((unsigned *)&linecmds)+1) = (unsigned) shape;		// seg of shape

//
// scale to the left (from pixel 31 to shape->leftpix)
//
	srcx = 32;
	slinex = xcenter;
	stopx = shape->leftpix;
	cmdptr = &shape->dataofs[31-stopx];

	while ( --srcx >=stopx && slinex>0)
	{
		*((unsigned *)&linecmds) = *cmdptr--;
		if ( !(slinewidth = comptable->width[srcx]) )
			continue;

		if (slinewidth == 1)
		{
			slinex--;
			if (slinex<viewwidth)
			{
				if (wallheight[slinex] >= height)
					continue;		// obscured by closer wall
				ScaleLine ();
			}
			continue;
		}

		//
		// handle multi pixel lines
		//
		if (slinex>viewwidth)
		{
			slinex -= slinewidth;
			slinewidth = viewwidth-slinex;
			if (slinewidth<1)
				continue;		// still off the right side
		}
		else
		{
			if (slinewidth>slinex)
				slinewidth = slinex;
			slinex -= slinewidth;
		}


		leftvis = (wallheight[slinex] < height);
		rightvis = (wallheight[slinex+slinewidth-1] < height);

		if (leftvis)
		{
			if (rightvis)
				ScaleLine ();
			else
			{
				while (wallheight[slinex+slinewidth-1] >= height)
					slinewidth--;
				ScaleLine ();
			}
		}
		else
		{
			if (!rightvis)
				continue;		// totally obscured

			while (wallheight[slinex] >= height)
			{
				slinex++;
				slinewidth--;
			}
			ScaleLine ();
			break;			// the rest of the shape is gone
		}
	}


//
// scale to the right
//
	slinex = xcenter;
	stopx = shape->rightpix;
	if (shape->leftpix<31)
	{
		srcx = 31;
		cmdptr = &shape->dataofs[32-shape->leftpix];
	}
	else
	{
		srcx = shape->leftpix-1;
		cmdptr = &shape->dataofs[0];
	}
	slinewidth = 0;

	while ( ++srcx <= stopx && (slinex+=slinewidth)<viewwidth)
	{
		*((unsigned *)&linecmds) = *cmdptr++;
		if ( !(slinewidth = comptable->width[srcx]) )
			continue;

		if (slinewidth == 1)
		{
			if (slinex>=0 && wallheight[slinex] < height)
			{
				ScaleLine ();
			}
			continue;
		}

		//
		// handle multi pixel lines
		//
		if (slinex<0)
		{
			if (slinewidth <= -slinex)
				continue;		// still off the left edge

			slinewidth += slinex;
			slinex = 0;
		}
		else
		{
			if (slinex + slinewidth > viewwidth)
				slinewidth = viewwidth-slinex;
		}


		leftvis = (wallheight[slinex] < height);
		rightvis = (wallheight[slinex+slinewidth-1] < height);

		if (leftvis)
		{
			if (rightvis)
			{
				ScaleLine ();
			}
			else
			{
				while (wallheight[slinex+slinewidth-1] >= height)
					slinewidth--;
				ScaleLine ();
				break;			// the rest of the shape is gone
			}
		}
		else
		{
			if (rightvis)
			{
				while (wallheight[slinex] >= height)
				{
					slinex++;
					slinewidth--;
				}
				ScaleLine ();
			}
			else
				continue;		// totally obscured
		}
	}
}



/*
=======================
=
= SimpleScaleShape
=
= NO CLIPPING, height in pixels
=
= Draws a compiled shape at [scale] pixels high
=
= each vertical line of the shape has a pointer to segment data:
= 	end of segment pixel*2 (0 terminates line) used to patch rtl in scaler
= 	top of virtual line with segment in proper place
=	start of segment pixel*2, used to jsl into compiled scaler
=	<repeat>
=
= Setup for call
= --------------
= GC_MODE			read mode 1, write mode 2
= GC_COLORDONTCARE  set to 0, so all reads from video memory return 0xff
= GC_INDEX			pointing at GC_BITMASK
=
=======================
*/

void SimpleScaleShape (int xcenter, int shapenum, unsigned height)
{
	t_compshape *shape = NULL;
	t_compscale *comptable = NULL;
	unsigned	scale = 0;
	unsigned 	srcx = 0;
	unsigned	stopx = 0;
	unsigned	tempx = 0;
	int		t = 0;
	unsigned	/*far*/ *cmdptr = NULL;
	boolean		leftvis;
	boolean 	rightvis; /*more booleans*/


	shape = PM_GetSpritePage (shapenum);

	scale = height>>1;
	comptable = scaledirectory[scale];

	*(((unsigned *)&linescale)+1)=(unsigned)comptable;	// seg of far call
	*(((unsigned *)&linecmds)+1)=(unsigned)shape;		// seg of shape

//
// scale to the left (from pixel 31 to shape->leftpix)
//
	srcx = 32;
	slinex = xcenter;
	stopx = shape->leftpix;
	cmdptr = &shape->dataofs[31-stopx];

	while ( --srcx >=stopx )
	{
		*((unsigned *)&linecmds) = *cmdptr--;
		if ( !(slinewidth = comptable->width[srcx]) )
			continue;

		slinex -= slinewidth;
		ScaleLine ();
	}


//
// scale to the right
//
	slinex = xcenter;
	stopx = shape->rightpix;
	if (shape->leftpix<31)
	{
		srcx = 31;
		cmdptr = &shape->dataofs[32-shape->leftpix];
	}
	else
	{
		srcx = shape->leftpix-1;
		cmdptr = &shape->dataofs[0];
	}
	slinewidth = 0;

	while ( ++srcx <= stopx )
	{
		*((unsigned *)&linecmds) = *cmdptr++;
		if ( !(slinewidth = comptable->width[srcx]) )
			continue;

		ScaleLine ();
		slinex+=slinewidth;
	}
}




//
// bit mask tables for drawing scaled strips up to eight pixels wide
//
// down here so the STUPID inline assembler doesn't get confused!
//


byte	mapmasks1[4][8] = {
{1 ,3 ,7 ,15,15,15,15,15},
{2 ,6 ,14,14,14,14,14,14},
{4 ,12,12,12,12,12,12,12},
{8 ,8 ,8 ,8 ,8 ,8 ,8 ,8} };

byte	mapmasks2[4][8] = {
{0 ,0 ,0 ,0 ,1 ,3 ,7 ,15},
{0 ,0 ,0 ,1 ,3 ,7 ,15,15},
{0 ,0 ,1 ,3 ,7 ,15,15,15},
{0 ,1 ,3 ,7 ,15,15,15,15} };

byte	mapmasks3[4][8] = {
{0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},
{0 ,0 ,0 ,0 ,0 ,0 ,0 ,1},
{0 ,0 ,0 ,0 ,0 ,0 ,1 ,3},
{0 ,0 ,0 ,0 ,0 ,1 ,3 ,7} };


unsigned	wordmasks[8][8] = {
{0x0080,0x00c0,0x00e0,0x00f0,0x00f8,0x00fc,0x00fe,0x00ff},
{0x0040,0x0060,0x0070,0x0078,0x007c,0x007e,0x007f,0x807f},
{0x0020,0x0030,0x0038,0x003c,0x003e,0x003f,0x803f,0xc03f},
{0x0010,0x0018,0x001c,0x001e,0x001f,0x801f,0xc01f,0xe01f},
{0x0008,0x000c,0x000e,0x000f,0x800f,0xc00f,0xe00f,0xf00f},
{0x0004,0x0006,0x0007,0x8007,0xc007,0xe007,0xf007,0xf807},
{0x0002,0x0003,0x8003,0xc003,0xe003,0xf003,0xf803,0xfc03},
{0x0001,0x8001,0xc001,0xe001,0xf001,0xf801,0xfc01,0xfe01} };

int	slinex = 0;
int 	slinewidth = 0;
unsigned	*linecmds = NULL;
long int	linescale = 0;
unsigned	maskword = 0;

