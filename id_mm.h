// ID_MM.H

#ifndef __ID_MM_H__ // << correct name.
#define __ID_MM_H__

/**
  Include section
*/
#include "id_heads.h"

#include <stdlib.h>
#include <stdio.h>



/**

  Define section

*/
#define LOCKBIT         0x80    // if set in attributes, block cannot be moved
#define PURGEBITS       3               // 0-3 level, 0= unpurgable, 3= purge first
#define PURGEMASK       0xfffc
#define BASEATTRIBUTES  0       // unlocked, non purgable

#define MAXUMBS         10


#define SAVENEARHEAP	0x400		// space to leave in data segment
#define SAVEFARHEAP		0			// space to leave in far heap

#define	BUFFERSIZE		0x1000		// miscelanious, allways available buffer

#define MAXBLOCKS		700


//--------

#define	EMS_INT			0x67

#define	EMS_STATUS		0x40
#define	EMS_GETFRAME	0x41
#define	EMS_GETPAGES	0x42
#define	EMS_ALLOCPAGES	0x43
#define	EMS_MAPPAGE		0x44
#define	EMS_FREEPAGES	0x45
#define	EMS_VERSION		0x46

//--------

#define	XMS_INT			0x2f
#define	XMS_CALL(v)		_AH = (v);\
						asm call [DWORD PTR XMSDriver]

#define	XMS_VERSION		0x00

#define	XMS_ALLOCHMA	0x01
#define	XMS_FREEHMA		0x02

#define	XMS_GENABLEA20	0x03
#define	XMS_GDISABLEA20	0x04
#define	XMS_LENABLEA20	0x05
#define	XMS_LDISABLEA20	0x06
#define	XMS_QUERYA20	0x07

#define	XMS_QUERYFREE	0x08
#define	XMS_ALLOC		0x09
#define	XMS_FREE		0x0A
#define	XMS_MOVE		0x0B
#define	XMS_LOCK		0x0C
#define	XMS_UNLOCK		0x0D
#define	XMS_GETINFO		0x0E
#define	XMS_RESIZE		0x0F

#define	XMS_ALLOCUMB	0x10
#define	XMS_FREEUMB		0x11
#define GETNEWBLOCK {if(!mmfree)MML_ClearBlock();mmnew=mmfree;mmfree=mmfree->next;}

#define FREEBLOCK(x) {*x->useptr=NULL;x->next=mmfree;mmfree=x;}

//==========================================================================

typedef void * memptr;

typedef struct
{
	long	nearheap,farheap,EMSmem,XMSmem,mainmem;
} mminfotype;

typedef struct mmblockstruct
{
        unsigned        start,length;
        unsigned        attributes;
        memptr          *useptr;        // pointer to the segment start
        struct mmblockstruct  *next;
} mmblocktype;

/*
 *  Global variables section.
 *
 * */
mminfotype      mminfo;
memptr          bufferseg;
boolean         mmerror;

void            (* beforesort) (void);
void            (* aftersort) (void);
boolean         mmstarted;

void        *farheap;
void            *nearheap;

mmblocktype      mmblocks[MAXBLOCKS]
                        , *mmhead, *mmfree, *mmrover, *mmnew;

boolean         bombonerror;

//unsigned      totalEMSpages,freeEMSpages,EMSpageframe,EMSpagesmapped,EMShandle;

void            (* XMSaddr) (void);             // far pointer to XMS driver

unsigned        numUMBs,UMBbase[MAXUMBS];





//==========================================================================

extern	mminfotype	mminfo;
extern	memptr		bufferseg;
extern	boolean		mmerror;

extern	void		(* beforesort) (void);
extern	void		(* aftersort) (void);

//==========================================================================

void MM_Startup (void);
void MM_Shutdown (void);
void MM_MapEMS (void);

void MM_GetPtr (memptr *baseptr,unsigned long size);
void MM_FreePtr (memptr *baseptr);

void MM_SetPurge (memptr *baseptr, int purge);
void MM_SetLock (memptr *baseptr, boolean locked);
void MM_SortMem (void);

void MM_ShowMemory (void);

long MM_UnusedMemory (void);
long MM_TotalFree (void);

void MM_BombOnError (boolean bomb);

void MML_UseSpace (unsigned segstart, unsigned seglength);

//
//// local prototypes
////
//
boolean         MML_CheckForEMS (void);
void            MML_ShutdownEMS (void);
void            MM_MapEMS (void);
boolean         MML_CheckForXMS (void);
void            MML_ShutdownXMS (void);
void            MML_UseSpace (unsigned segstart, unsigned seglength);
void            MML_ClearBlock (void);


#endif
