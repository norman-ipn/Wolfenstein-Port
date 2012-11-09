/**
  \file id_heads.h

  \author Rafael Norman Saucedo Delgado

  
  Fusion of ID_HEAD.H and ID_HEADS.H 
  because they seem to define similar types.


  If your file includes ID_HEAD.H or ID_HEADS.H 
  use this new fusioned version. 
 
*/

#ifndef __ID_HEADS_H__
#define __ID_HEADS_H__

/*
  
  Include section

*/

  #include "version.h"

  /* basic headers for non japanesse, non Spear version */
  #include "gfxv_wl6.h"
  #include "audiowl6.h"
  #include "mapswl6.h"

/*
  Each Manager, and each file, need to include his own .h file 
  #include "ID_MM.H"
  #include "ID_PM.H"
  #include "ID_CA.H"
  #include "ID_VL.H"
  #include "ID_VH.H"
  #include "ID_IN.H"
  #include "ID_SD.H"
  #include "ID_US.H"

*/

/**
  Constant global variables section
*/
const char GREXT[] = "VGA"; 
const char EXTENSION[4] = "WLF"; 

/**

  Typedef section

*/
/**
    Some typedef for the game.
*/
typedef	enum {false,true} boolean;
typedef	unsigned char     byte;
typedef	unsigned int      word;
typedef	unsigned long     longword;
typedef	unsigned char *	  Ptr;

typedef	struct {
  int x;
  int y;
} Point;

typedef	struct {
  Point	ul;
  Point lr;
} Rect;

/**

  Function headers section 

*/
void	Quit (char *error);		// defined in user program

/** 
  
  Defines section 

*/
#define __ID_GLOB__
#define WOLF //< from ID_HEAD.H
#define	introscn signon
#define	nil	((void *)0)
//
// replacing refresh manager with custom routines
//
#define	PORTTILESWIDE		20      // all drawing takes place inside a
#define	PORTTILESHIGH		13	// non displayed port of this size

#define UPDATEWIDE		PORTTILESWIDE
#define UPDATEHIGH		PORTTILESHIGH

#define	MAXTICS			10
#define DEMOTICS		4

#define	UPDATETERMINATE		0x0301
/**
 Selection of tthe GRraphics Mode

 VGA is the highest here.
*/
#define	TEXTGR	0
#define	CGAGR	1
#define	EGAGR	2
#define	VGAGR	3
#define GRMODE	VGAGR

/**

  Macro section 

*/
#define SETFONTCOLOR(f,b) fontcolor=f;backcolor=b;
/*

  Extern variables section

*/
extern char signon;
extern unsigned int mapwidth;
extern unsigned int mapheight;
extern unsigned int tics;
extern boolean	compatability;
extern byte	*updateptr;
extern unsigned	uwidthtable[UPDATEHIGH];
extern unsigned	blockstarts[UPDATEWIDE*UPDATEHIGH];
extern byte fontcolor;
extern byte backcolor;


#endif//__ID_HEADS_H__

