// NEWMM.C

#include "id_mm.h" 

/*
 * only functions on this .c file.
 *
 * All the definitions on .h files.
 * */

/*
-----------------------------------------------------------------------------
Im working with this file, im trying to change it from C to c.
But i need to know how to upload a file to submit the .c code and dont modify to much this.
Eduardo Adolfo Arroyo Lopez

Im getting troubles to upload a file, but by the lines sthat i've already checked, two of the very common errors that i 
have found are the word "far" for some variables (i dont know what is far, i've posted on the help page from wiki if 
someone see this). And the other error is because the mmblocktype structure has a field called *useptr in a commit
i deleted the commit part and now those errors are not showing anymore

*Update:
Another error that i found is the memptr, is not recognized by any function like a type, and some variables are not 
declared in here.
Now im working on that memptr...


File edited by Eduardo Adolfo Arroyo Lopez

The commits that start with  are the ones for the editions by me, to try to run the file.c

*Just cheched it up to the line 224, the assambly parts are the ones that are giving me errors until now.	

*Update I just finished to check all the code (in a fast way), the most common errors i found are, the word "far", 
the assambly code, some variables that are not declared, and that the memptr is not recognized by any function like a 
type.

*New update, a lot of errors were in the assambly code, so i erased (just put them in a comment) the functions because
they has no calls in any other part of the program.

***Using the library im getting many errors more, check them for the momemt
-----------------------------------------------------------------------------

-- Deleted pragma directives.
-- All defines were moved to id_mm.h 
-- All includes were moved to id_mm.h
-- All typedefs were moved to id_mm.h
-- All macros were moved to id_mm.h
-- All global variables were moved to id_mm.h



=============================================================================

		   ID software memory manager
		   --------------------------

Primary coder: John Carmack

RELIES ON
---------
Quit (char *error) function


WORK TO DO
----------
MM_SizePtr to change the size of a given pointer

Multiple purge levels utilized

EMS / XMS unmanaged routines

=============================================================================
*/

#include "id_mm.h"
#include <stdio.h> //This fixed many problems, including the ones that said that NULL was not declared here





//==========================================================================

/*
======================
=
= MML_CheckForXMS
=
= Check for XMM driver
=
=======================
*/
/*
boolean MML_CheckForXMS (void)
{
	numUMBs = 0;

//asm 						//The part for assambly marks an error
asm	mov	ax,0x4300
asm	int	0x2f				// query status of installed diver
asm	cmp	al,0x80
asm	je	good
	

	return false;
good:
	return true;
}
*/						//*Erased this function, is not used in any other part of the program

/*
======================
=
= MML_SetupXMS
=
= Try to allocate all upper memory block
=
=======================
*/
/*
void MML_SetupXMS (void)
{
	unsigned	base,size;

asm	{					//Assambly gets an error
	mov	ax,0x4310
	int	0x2f
	mov	[WORD PTR XMSaddr],bx
	mov	[WORD PTR XMSaddr+2],es		// function pointer to XMS driver
	}

getmemory:
asm	{
	mov	ah,XMS_ALLOCUMB
	mov	dx,0xffff					// try for largest block possible
	call	[DWORD PTR XMSaddr]
	or	ax,ax
	jnz	gotone

	cmp	bl,0xb0						// error: smaller UMB is available
	jne	done;

	mov	ah,XMS_ALLOCUMB
	call	[DWORD PTR XMSaddr]		// DX holds largest available UMB
	or	ax,ax
	jz	done						// another error...
	}

gotone:
asm	{
	mov	[base],bx
	mov	[size],dx
	}
	MML_UseSpace (base,size);
	mminfo.XMSmem += size*16;
	UMBbase[numUMBs] = base;
	numUMBs++;
	if (numUMBs < MAXUMBS)
		goto getmemory;

done:;
}
*/		//*Deleted this function, is not used in any part of the program

/*
======================
=
= MML_ShutdownXMS
=
======================
*/
/*
void MML_ShutdownXMS (void)
{
	int	i;
	unsigned	base;

	for (i=0;i<numUMBs;i++)
	{
		base = UMBbase[i];

asm	mov	ah,XMS_FREEUMB		//The asm section is gettin a lot of errors
asm	mov	dx,[base]
asm	call	[DWORD PTR XMSaddr]
	}
}
*/					//*
//==========================================================================

/*
======================
=
= MML_UseSpace
=
= Marks a range of paragraphs as usable by the memory manager
= This is used to mark space for the near heap, far heap, ems page frame,
= and upper memory blocks
=
======================
*/

void MML_UseSpace (unsigned segstart, unsigned seglength)
{
	mmblocktype     *scan = NULL;
	mmblocktype     *last = NULL;	//*The word far was deleted (in both variables)
	unsigned	oldend;
	long		extra;

	scan = last = mmhead;
	mmrover = mmhead;		// reset rover to start of memory

//
// search for the block that contains the range of segments
//
	while (scan->start+scan->length < segstart)
	{
		last = scan;
		scan = scan->next;
	}

//
// take the given range out of the block
//
	oldend = scan->start + scan->length;
	extra = oldend - (segstart+seglength);
	if (extra < 0)
		Quit ("MML_UseSpace: Segment spans two blocks!");

	if (segstart == scan->start)
	{
		last->next = scan->next;			// unlink block
		FREEBLOCK(scan);	//*says that mmblocktype doest have a member called useptr and NULL is no declared here (i think is from the header but im check it at the end).
//*The mmblocktype is already fixed
		scan = last;
	}
	else
		scan->length = segstart-scan->start;	// shorten block

	if (extra > 0)
	{
		GETNEWBLOCK;
		mmnew->useptr = NULL;

		mmnew->next = scan->next;
		scan->next = mmnew;
		mmnew->start = segstart+seglength;
		mmnew->length = extra;
		mmnew->attributes = LOCKBIT;
	}

}

//==========================================================================

/*
====================
=
= MML_ClearBlock
=
= We are out of blocks, so free a purgable block
=
====================
*/

void MML_ClearBlock (void)
{
	mmblocktype *scan = NULL;

	scan = mmhead->next;

	while (scan)
	{
		if (!(scan->attributes&LOCKBIT) && (scan->attributes&PURGEBITS) )
		{
			MM_FreePtr(scan->useptr);
			return;
		}
		scan = scan->next;
	}

	Quit ("MM_ClearBlock: No purgable blocks!");
}


//==========================================================================

/*
===================
=
= MM_Startup
=
= Grabs all space from turbo with malloc/farmalloc
= Allocates bufferseg misc buffer
=
===================
*/

static	char *ParmStrings[] = {"noems","noxms",""};

void MM_Startup (void)
{
	int i;
	unsigned 	long length;
	unsigned 	*start;				//*Deleted the word far		
	//*Changed the type from *start (from void to unsigned)
	unsigned 	segstart,seglength,endfree;

	if (mmstarted)
		MM_Shutdown ();


	mmstarted = 1;
	bombonerror = 1;	//Changed TRUE by 1 because TRUE and FALSE are not declared in c sintaxis, they are 1 and 0
//
// set up the linked list (everything in the free list;
//
	mmhead = NULL;			//*Put '' in NULL
	mmfree = &mmblocks[0];
	for (i=0;i<100-1;i++)			//*Changed MAXBLOCKS by 100 just for testing
		mmblocks[i].next = &mmblocks[i+1];
	mmblocks[i].next = NULL;		//*Put '' in NULL

//
// locked block of all memory until we punch out free space
//
	GETNEWBLOCK;
	mmhead = mmnew;				// this will allways be the first node
	mmnew->start = 0;
	mmnew->length = 0xffff;
	mmnew->attributes = LOCKBIT;
	mmnew->next = NULL;			//*Put '' in NULL
	mmrover = mmhead;


//
// get all available near conventional memory segments
//
	length=coreleft();
	start = (void *)(nearheap = malloc(length));	//*Deleted far

	length -= 16-(FP_OFF(start)&15);
	length -= 1000;			//*SAVENEARHEAP is not declared, chaged by a 1000 just for testing
	seglength = length / 16;			// now in paragraphs
	segstart = FP_SEG(start)+(FP_OFF(start)+15)/16;
	MML_UseSpace (segstart,seglength);
	mminfo.nearheap = length;

//
// get all available far conventional memory segments
//
	length=farcoreleft();
	start = farheap = farmalloc(length);
	length -= 16-(FP_OFF(start)&15);
	length -= 1000;		//*SAVEFARHEAP is not declared, changed by 1000 just for testing
	seglength = length / 16;			// now in paragraphs
	segstart = FP_SEG(start)+(FP_OFF(start)+15)/16;
	MML_UseSpace (segstart,seglength);
	mminfo.farheap = length;
	mminfo.mainmem = mminfo.nearheap + mminfo.farheap;

//
// allocate the misc buffer
//
	mmrover = mmhead;		// start looking for space after low block

	MM_GetPtr (&bufferseg,BUFFERSIZE);	//*bufferseg and BUFFERSIZE are not declared, changed BUFFERSIZE by 1000 just for testing, the &bufferseg stills without any change (dont know what can be instead of this).
}

//==========================================================================

/*
====================
=
= MM_Shutdown
=
= Frees all conventional, EMS, and XMS allocated
=
====================
*/

void MM_Shutdown (void)
{
  if (!mmstarted)
	return;

  farfree (farheap);
  free (nearheap);
//  MML_ShutdownXMS ();
}

//==========================================================================

/*
====================
=
= MM_GetPtr
=
= Allocates an unlocked, unpurgable block
=
====================
*/

void MM_GetPtr (memptr *baseptr,unsigned long size)	//*The memptr is not recognized
{
	mmblocktype  *scan, *lastscan, *endscan, *purge, *next;	//Erased the "far"
	int			search;
	unsigned	needed,startseg;

	needed = (size+15)/16;		// convert size from bytes to paragraphs

	GETNEWBLOCK;				// fill in start and next after a spot is found
	mmnew->length = needed;
	mmnew->useptr = baseptr;
	mmnew->attributes = BASEATTRIBUTES;

tryagain:
	for (search = 0; search<3; search++)
	{
	//
	// first search:	try to allocate right after the rover, then on up
	// second search: 	search from the head pointer up to the rover
	// third search:	compress memory, then scan from start
		if (search == 1 && mmrover == mmhead)
			search++;

		switch (search)
		{
		case 0:
			lastscan = mmrover;
			scan = mmrover->next;
			endscan = NULL;
			break;
		case 1:
			lastscan = mmhead;
			scan = mmhead->next;
			endscan = mmrover;
			break;
		case 2:
			MM_SortMem ();
			lastscan = mmhead;
			scan = mmhead->next;
			endscan = NULL;
			break;
		}

		startseg = lastscan->start + lastscan->length;

		while (scan != endscan)
		{
			if (scan->start - startseg >= needed)
			{
			//
			// got enough space between the end of lastscan and
			// the start of scan, so throw out anything in the middle
			// and allocate the new block
			//
				purge = lastscan->next;
				lastscan->next = mmnew;
				mmnew->start = *(unsigned *)baseptr = startseg;
				mmnew->next = scan;
				while ( purge != scan)
				{	// free the purgable block
					next = purge->next;
					FREEBLOCK(purge);
					purge = next;		// purge another if not at scan
				}
				mmrover = mmnew;
				return;	// good allocation!
			}

			//
			// if this block is purge level zero or locked, skip past it
			//
			if ( (scan->attributes & LOCKBIT)
				|| !(scan->attributes & PURGEBITS) )
			{
				lastscan = scan;
				startseg = lastscan->start + lastscan->length;
			}


			scan=scan->next;		// look at next line
		}
	}

	if (bombonerror)
	{

extern char configname[];
extern	boolean	insetupscaling;
extern	int	viewsize;
boolean SetViewSize (unsigned width, unsigned height);
#define HEIGHTRATIO		0.50
//
// wolf hack -- size the view down
//
		if (!insetupscaling && viewsize>10)
		{
			mmblocktype  *savedmmnew;	//Erased "far"
			savedmmnew = mmnew;
			viewsize -= 2;
			SetViewSize (viewsize*16,viewsize*16*HEIGHTRATIO);
			mmnew = savedmmnew;
			goto tryagain;
		}

//		unlink(configname);
		Quit ("MM_GetPtr: Out of memory!");
	}
	else
		mmerror = true;
}

//==========================================================================

/*
====================
=
= MM_FreePtr
=
= Deallocates an unlocked, purgable block
=
====================
*/

void MM_FreePtr (memptr *baseptr)		//*the memptr is not recognized
{
	mmblocktype *scan, *last;	//*Deleted the word far in both variables

	last = mmhead;
	scan = last->next;

	if (baseptr == mmrover->useptr)	// removed the last allocated block
		mmrover = mmhead;

	while (scan->useptr != baseptr && scan)
	{
		last = scan;
		scan = scan->next;
	}

	if (!scan)
		Quit ("MM_FreePtr: Block not found!");

	last->next = scan->next;

	FREEBLOCK(scan);
}
//==========================================================================

/*
=====================
=
= MM_SetPurge
=
= Sets the purge level for a block (locked blocks cannot be made purgable)
=
=====================
*/

void MM_SetPurge (memptr *baseptr, int purge)		//*memptr is not recognized
{
	mmblocktype  *start;	//erassed "far"

	start = mmrover;

	do
	{
		if (mmrover->useptr == baseptr)
			break;

		mmrover = mmrover->next;

		if (!mmrover)
			mmrover = mmhead;
		else if (mmrover == start)
			Quit ("MM_SetPurge: Block not found!");

	} while (1);

	mmrover->attributes &= ~PURGEBITS;
	mmrover->attributes |= purge;
}

//==========================================================================

/*
=====================
=
= MM_SetLock
=
= Locks / unlocks the block
=
=====================
*/

void MM_SetLock (memptr *baseptr, boolean locked)	//*memptr is not recognized
{
	mmblocktype *start;	//erased "far"

	start = mmrover;

	do
	{
		if (mmrover->useptr == baseptr)
			break;

		mmrover = mmrover->next;

		if (!mmrover)
			mmrover = mmhead;
		else if (mmrover == start)
			Quit ("MM_SetLock: Block not found!");

	} while (1);

	mmrover->attributes &= ~LOCKBIT;
	mmrover->attributes |= locked*LOCKBIT;
}

//==========================================================================

/*
=====================
=
= MM_SortMem
=
= Throws out all purgable stuff and compresses movable blocks
=
=====================
*/

void MM_SortMem (void)
{
	mmblocktype  *scan, *last, *next;
	unsigned	start,length,source,dest;
	int			playing;

	//
	// lock down a currently playing sound
	//
	playing = SD_SoundPlaying ();
	if (playing)
	{
		switch (SoundMode)	//*SoundMode is not declared (dont know how to change it)
		{
		case sdm_PC:		//*sdm_PC is not declared
			playing += STARTPCSOUNDS;
			break;
		case sdm_AdLib:		//*sdm_AdLib is not declared
			playing += STARTADLIBSOUNDS;
			break;
		}
		MM_SetLock(&(memptr)audiosegs[playing],true);	//*audiosegs is not declarde
	}


	SD_StopSound();

	if (beforesort)
		beforesort();

	scan = mmhead;

	last = 'NULL';		// shut up compiler warning
	//*put '' in NULL

	while (scan)
	{
		if (scan->attributes & LOCKBIT)
		{
		//
		// block is locked, so try to pile later blocks right after it
		//
			start = scan->start + scan->length;
		}
		else
		{
			if (scan->attributes & PURGEBITS)
			{
			//
			// throw out the purgable block
			//
				next = scan->next;
				FREEBLOCK(scan);
				last->next = next;
				scan = next;
				continue;
			}
			else
			{
			//
			// push the non purgable block on top of the last moved block
			//
				if (scan->start != start)
				{
					length = scan->length;
					source = scan->start;
					dest = start;
					while (length > 0xf00)
					{
						movedata(source,0,dest,0,0xf00*16);
						length -= 0xf00;
						source += 0xf00;
						dest += 0xf00;
					}
					movedata(source,0,dest,0,length*16);

					scan->start = start;
					*(unsigned *)scan->useptr = start;
				}
				start = scan->start + scan->length;
			}
		}

		last = scan;
		scan = scan->next;		// go to next block
	}

	mmrover = mmhead;

	if (aftersort)
		aftersort();

	if (playing)
		MM_SetLock(&(memptr)audiosegs[playing],false);		//*again the memptr is gettin error
}


//==========================================================================

/*
=====================
=
= MM_ShowMemory
=
=====================
*/

void MM_ShowMemory (void)
{
	mmblocktype *scan;		//*Deleted the far....says scan is not declared
	unsigned color,temp,x,y;
	long	end,owner;
	char    scratch[80],str[10];

	temp = bufferofs;		//*bufferofs is not declared
	bufferofs = displayofs;		//*displayofs is not declared
	scan = mmhead;

	end = -1;

	while (scan)
	{
		if (scan->attributes & PURGEBITS)
			color = 5;		// dark purple = purgable
		else
			color = 9;		// medium blue = non purgable
		if (scan->attributes & LOCKBIT)
			color = 12;		// red = locked
		if (scan->start<=end)
			Quit ("MM_ShowMemory: Memory block order currupted!");
		end = scan->length-1;
		y = scan->start/320;
		x = scan->start%320;
		VW_Hlin(x,x+end,y,color);
		VW_Plot(x,y,15);
		if (scan->next && scan->next->start > end+1)
			VW_Hlin(x+end+1,x+(scan->next->start-scan->start),y,0);	// black = free

		scan = scan->next;
	}

	VW_FadeIn ();
	IN_Ack();

	bufferofs = temp;
}

//==========================================================================

/*
=====================
=
= MM_DumpData
=
=====================
*/

void MM_DumpData (void)
{
	mmblocktype *scan, *best;	//*Deleted he far word
	long	lowest,oldlowest;
	unsigned	owner;
	char	lock,purge;
	FILE	*dumpfile;


	free (nearheap);
	dumpfile = fopen ("MMDUMP.TXT","w");
	if (!dumpfile)
		Quit ("MM_DumpData: Couldn't open MMDUMP.TXT!");

	lowest = -1;
	do
	{
		oldlowest = lowest;
		lowest = 0xffff;

		scan = mmhead;
		while (scan)
		{
			owner = (unsigned)scan->useptr;

			if (owner && owner<lowest && owner > oldlowest)
			{
				best = scan;
				lowest = owner;
			}

			scan = scan->next;
		}

		if (lowest != 0xffff)
		{
			if (best->attributes & PURGEBITS)
				purge = 'P';
			else
				purge = '-';
			if (best->attributes & LOCKBIT)
				lock = 'L';
			else
				lock = '-';
			fprintf (dumpfile,"0x%p (%c%c) = %u\n"
			,(unsigned)lowest,lock,purge,best->length);
		}

	} while (lowest != 0xffff);

	fclose (dumpfile);
	Quit ("MMDUMP.TXT created.");
}

//==========================================================================


/*
======================
=
= MM_UnusedMemory
=
= Returns the total free space without purging
=
======================
*/

long MM_UnusedMemory (void)
{
	unsigned free;
	mmblocktype *scan;		//*Deleted far

	free = 0;
	scan = mmhead;

	while (scan->next)
	{
		free += scan->next->start - (scan->start + scan->length);
		scan = scan->next;
	}

	return free*16l;
}

//==========================================================================


/*
======================
=
= MM_TotalFree
=
= Returns the total free space with purging
=
======================
*/

long MM_TotalFree (void)
{
	unsigned free;
	mmblocktype *scan;		//*Deleted far

	free = 0;
	scan = mmhead;

	while (scan->next)
	{
		if ((scan->attributes&PURGEBITS) && !(scan->attributes&LOCKBIT))
			free += scan->length;
		free += scan->next->start - (scan->start + scan->length);
		scan = scan->next;
	}

	return free*16l;
}

//==========================================================================

/*
=====================
=
= MM_BombOnError
=
=====================
*/

void MM_BombOnError (boolean bomb)
{
	bombonerror = bomb;
}

