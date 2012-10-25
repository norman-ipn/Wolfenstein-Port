// WL_TEXT.C

/*
 List of functions in this file
 (NOTE: This file need his .h file)

 void RipToEOL (void);
 int ParseNumber (void);
 void ParsePicCommand (void);
void	ParseTimedCommand (void);
void	TimedPicCommand (void);
void HandleCommand (void);
void NewLine(void);
void HandleCtrls (void)
void HandleWord (void)
void PageLayout (boolean shownumber)
void BackPage (void)

void CacheLayoutGraphics (void)
void ShowArticle (char far *article)
void HelpScreens (void)
void EndText(void);

These function seems to work with global variable 'text'
to parse text commands, but Where in the game are those text
functions used?? We need them?
 

*/

#include "WL_DEF.H"
#pragma	hdrstop

/*
=============================================================================

TEXT FORMATTING COMMANDS
------------------------
^C<hex digit>  			Change text color
^E[enter]				End of layout (all pages)
^G<y>,<x>,<pic>[enter]	Draw a graphic and push margins
^P[enter]				start new page, must be the first chars in a layout
^L<x>,<y>[ENTER]		Locate to a specific spot, x in pixels, y in lines

=============================================================================
*/

/*
=============================================================================

						 LOCAL CONSTANTS

=============================================================================
*/

#define BACKCOLOR		0x11

#define WORDLIMIT		80
#define FONTHEIGHT		10
#define	TOPMARGIN		16
#define BOTTOMMARGIN	32
#define LEFTMARGIN		16
#define RIGHTMARGIN		16
#define PICMARGIN		8
#define TEXTROWS		((200-TOPMARGIN-BOTTOMMARGIN)/FONTHEIGHT)
#define	SPACEWIDTH		7
#define SCREENPIXWIDTH	320
#define SCREENMID		(SCREENPIXWIDTH/2)

/*
=============================================================================

						 LOCAL VARIABLES

=============================================================================
*/

int			pagenum,numpages;

unsigned	leftmargin[TEXTROWS],rightmargin[TEXTROWS];
char		far *text;
unsigned	rowon;

int			picx,picy,picnum,picdelay;
boolean		layoutdone;

//===========================================================================

#ifndef JAPAN
/*
=====================
=
= RipToEOL
=
=====================
*/

void RipToEOL (void)// this funcion just walking the values int the pointer "text" to the end of the line
{
	while (*text++ != '\n')		// scan to end of line
	;
}


/*
=====================
=
= ParseNumber
=
=====================
*/

int	ParseNumber (void)//this funcion returns an integer
{
	char	ch;// characher variable
	char	num[80],*numptr;//pointer and array

//
// scan until a number is found
//
	ch = *text;// itś given the value in the pointer "text" to "ch"
	while (ch < '0' || ch >'9')//this while itś cycling util the value in "ch" have a value less than 0 and greater to 9
		ch = *++text;//itś given the next value in the ponter "text" to "ch"

//
// copy the number out
//
	numptr = num;// itś assigned  the value int "num" to "numptr"
	do
	{
		*numptr++ = ch;// itś assigned the value in "ch" to the next posicion of "numptr"
		ch = *++text;//itś assigned the next value in "text" to "ch"
	} while (ch >= '0' && ch <= '9');//this while will be cyclin while the value is between 0 and 9
	*numptr = 0;

	return atoi (num);//return the value in "num" and converted to an integer
}



/*
=====================
=
= ParsePicCommand
=
= Call with text pointing just after a ^P
= Upon exit text points to the start of next line
=
=====================
*/

void	ParsePicCommand (void)
{
	picy=ParseNumber();//itś assigned the value that is returned the  funcion "ParseNumber" in "picy"
	picx=ParseNumber();//itś  assigned the value that is returned the funcon "ParseNumber" in "picx"
	picnum=ParseNumber();//itś assigned the value that is returned the funcion "parrseNumber" in "picnum"
	RipToEOL ();//ths funcion just walking the values in the ponter "text"
}


void	ParseTimedCommand (void)
{
	picy=ParseNumber();//itś assigned the value that is returned the  funcion "ParseNumber" in "picy"
	picx=ParseNumber();//itś  assigned the value that is returned the funcon "ParseNumber" in "picx"
	picnum=ParseNumber();//itś assigned the value that is returned the funcion "parrseNumber" in "picnum"
	picdelay=ParseNumber();//itś assigned the value tha is retruned the funcion"ParseNumber"in "picdelay"
	RipToEOL ();//ths funcion just walking the values in the ponter "text"
}


/*
=====================
=
= TimedPicCommand
=
= Call with text pointing just after a ^P
= Upon exit text points to the start of next line
=
=====================
*/

void	TimedPicCommand (void)
{
	ParseTimedCommand ();//Just execute to given the values to the varaibles

//
// update the screen, and wait for time delay
//
	VW_UpdateScreen ();

//
// wait for time
//
	TimeCount = 0;
	while (TimeCount < picdelay)//ths whie itś cycling whit a counter util itś less than picedlay
	;

//
// draw pic
//
	VWB_DrawPic (picx&~7,picy,picnum);
}


/*
=====================
=
= HandleCommand
=
=====================
*/

void HandleCommand (void)
{
	int	i,margin,top,bottom;//integer variables
	int	picwidth,picheight,picmid;//integer variables

	switch (toupper(*++text))//this switch execut the funcion depending the value that is returned by the fucnion "toupper"
	{
	case 'B':
		picy=ParseNumber();//itś assigned the value returned from the funcon"ParseNumber" to "picy"
		picx=ParseNumber();//itś assigned the value returned from the funcon"ParseNumber" to "picx"
		picwidth=ParseNumber();//itś assigned the value returned from the funcon"ParseNumber" to "picwidth"
		picheight=ParseNumber();//itś assigned the value returned from the funcon"ParseNumber" to "picheight"
		VWB_Bar(picx,picy,picwidth,picheight,BACKCOLOR);
		RipToEOL ();//ths funcion just walking the values in the ponter "text"
		break;
	case ';':		// comment
		RipToEOL ();//ths funcion just walking the values in the ponter "text"
		break;
	case 'P':		// ^P is start of next page, ^E is end of file
	case 'E':
		layoutdone = true;
		text--;    	// back up to the '^'
		break;

	case 'C':		// ^c<hex digit> changes text color
		i = toupper(*++text);//given th value to get "toupper" to "i"
		if (i>='0' && i<='9')//on this condition
			fontcolor = i-'0';//it's given this value to "fontcolor"
		else if (i>='A' && i<='F')//if the first condition didn't work out 
			fontcolor = i-'A'+10;//itś given this value to "fontcolor"

		fontcolor *= 16;
		i = toupper(*++text);
		if (i>='0' && i<='9')
			fontcolor += i-'0';
		else if (i>='A' && i<='F')
			fontcolor += i-'A'+10;
		text++;
		break;

	case '>':
		px = 160;//given the value fo 160 to "px"
		text++;//increment the pointer "text"
		break;

	case 'L':
		py=ParseNumber();//given the value returned by the funcion "ParseNumber" to "py"
		rowon = (py-TOPMARGIN)/FONTHEIGHT;//given the value  of the result of this fomula that contains default numbers to "rowon"
		py = TOPMARGIN+rowon*FONTHEIGHT;//given the value  of the result of this fomula that contains default numbers to "py"
		px=ParseNumber();//given the value retruned b the funcion "ParseNumebr" to "px"
		while (*text++ != '\n')		// scan to end of line
		;
		break;

	case 'T':		// ^Tyyy,xxx,ppp,ttt waits ttt tics, then draws pic
		TimedPicCommand ();
		break;

	case 'G':		// ^Gyyy,xxx,ppp draws graphic
		ParsePicCommand ();
		VWB_DrawPic (picx&~7,picy,picnum);
		picwidth = pictable[picnum-STARTPICS].width;//given the value of the width  to "picwidth"
		picheight = pictable[picnum-STARTPICS].height;//given the value of the height to "picheight"
		//
		// adjust margins
		//
		picmid = picx + picwidth/2;//just assignes the value of the result of this fomula to "picmid"
		if (picmid > SCREENMID)
			margin = picx-PICMARGIN;			// new right margin
		else
			margin = picx+picwidth+PICMARGIN;	// new left margin

		top = (picy-TOPMARGIN)/FONTHEIGHT;//given the value  of the result of this fomula that contains default numbers to "top"
		if (top<0)
			top = 0;
		bottom = (picy+picheight-TOPMARGIN)/FONTHEIGHT;//given the value  of the result of this fomula that contains default numbers to "bottom"
		if (bottom>=TEXTROWS)
			bottom = TEXTROWS-1;//given the value  of the result of this fomula that contains default numbers to "bottom"

		for (i=top;i<=bottom;i++)//this will be cycling until the top get the same value whith bottom
			if (picmid > SCREENMID)//if tha condition is true...
				rightmargin[i] = margin;//it's the value of "margin" to "roghtmargin" in "i" posicion
			else
				leftmargin[i] = margin;//it's the value of "margin" to "leftmargin" in "i" posicion

		//
		// adjust this line if needed
		//
		if (px < leftmargin[rowon])//if this condicion is true ...
			px = leftmargin[rowon];//it's assignes the value of "leftmargin" in "rowon" posicion in to "px"
		break;
	}
}


/*
=====================
=
= NewLine
=
=====================
*/

void NewLine (void)
{
	char	ch;

	if (++rowon == TEXTROWS)
	{
	//
	// overflowed the page, so skip until next page break
	//
		layoutdone = true;
		do
		{
			if (*text == '^')
			{
				ch = toupper(*(text+1));
				if (ch == 'E' || ch == 'P')
				{
					layoutdone = true;
					return;
				}
			}
			text++;

		} while (1);

	}
	px = leftmargin[rowon];
	py+= FONTHEIGHT;
}



/*
=====================
=
= HandleCtrls
=
=====================
*/

void HandleCtrls (void)
{
	char	ch;

	ch = *text++;			// get the character and advance

	if (ch == '\n')
	{
		NewLine ();
		return;
	}

}


/*
=====================
=
= HandleWord
=
=====================
*/

void HandleWord (void)
{
	char		word[WORDLIMIT];
	int			i,wordindex;
	unsigned	wwidth,wheight,newpos;


	//
	// copy the next word into [word]
	//
	word[0] = *text++;
	wordindex = 1;
	while (*text>32)
	{
		word[wordindex] = *text++;
		if (++wordindex == WORDLIMIT)
			Quit ("PageLayout: Word limit exceeded");
	}
	word[wordindex] = 0;		// stick a null at end for C

	//
	// see if it fits on this line
	//
	VW_MeasurePropString (word,&wwidth,&wheight);

	while (px+wwidth > rightmargin[rowon])
	{
		NewLine ();
		if (layoutdone)
			return;		// overflowed page
	}

	//
	// print it
	//
	newpos = px+wwidth;
	VWB_DrawPropString (word);
	px = newpos;

	//
	// suck up any extra spaces
	//
	while (*text == ' ')
	{
		px += SPACEWIDTH;
		text++;
	}
}

/*
=====================
=
= PageLayout
=
= Clears the screen, draws the pics on the page, and word wraps the text.
= Returns a pointer to the terminating command
=
=====================
*/

void PageLayout (boolean shownumber)
{
	int		i,oldfontcolor;
	char	ch;

	oldfontcolor = fontcolor;

	fontcolor = 0;

//
// clear the screen
//
	VWB_Bar (0,0,320,200,BACKCOLOR);
	VWB_DrawPic (0,0,H_TOPWINDOWPIC);
	VWB_DrawPic (0,8,H_LEFTWINDOWPIC);
	VWB_DrawPic (312,8,H_RIGHTWINDOWPIC);
	VWB_DrawPic (8,176,H_BOTTOMINFOPIC);


	for (i=0;i<TEXTROWS;i++)
	{
		leftmargin[i] = LEFTMARGIN;
		rightmargin[i] = SCREENPIXWIDTH-RIGHTMARGIN;
	}

	px = LEFTMARGIN;
	py = TOPMARGIN;
	rowon = 0;
	layoutdone = false;

//
// make sure we are starting layout text (^P first command)
//
	while (*text <= 32)
		text++;

	if (*text != '^' || toupper(*++text) != 'P')
		Quit ("PageLayout: Text not headed with ^P");

	while (*text++ != '\n')
	;


//
// process text stream
//
	do
	{
		ch = *text;

		if (ch == '^')
			HandleCommand ();
		else
		if (ch == 9)
		{
		 px = (px+8)&0xf8;
		 text++;
		}
		else if (ch <= 32)
			HandleCtrls ();
		else
			HandleWord ();

	} while (!layoutdone);

	pagenum++;

	if (shownumber)
	{
		#ifdef SPANISH
		strcpy (str,"Hoja ");
		itoa (pagenum,str2,10);
		strcat (str,str2);
		strcat (str," de ");
		py = 183;
		px = 208;
		#else
		strcpy (str,"pg ");
		itoa (pagenum,str2,10);
		strcat (str,str2);
		strcat (str," of ");
		py = 183;
		px = 213;
		#endif
		itoa (numpages,str2,10);
		strcat (str,str2);
		fontcolor = 0x4f; 			   //12^BACKCOLOR;

		VWB_DrawPropString (str);
	}

	fontcolor = oldfontcolor;
}

//===========================================================================

/*
=====================
=
= BackPage
=
= Scans for a previous ^P
=
=====================
*/

void BackPage (void)
{
	pagenum--;
	do
	{
		text--;
		if (*text == '^' && toupper(*(text+1)) == 'P')
			return;
	} while (1);
}


//===========================================================================


/*
=====================
=
= CacheLayoutGraphics
=
= Scans an entire layout file (until a ^E) marking all graphics used, and
= counting pages, then caches the graphics in
=
=====================
*/
void CacheLayoutGraphics (void)
{
	char	far *bombpoint, far *textstart;
	char	ch;

	textstart = text;
	bombpoint = text+30000;
	numpages = pagenum = 0;

	do
	{
		if (*text == '^')
		{
			ch = toupper(*++text);
			if (ch == 'P')		// start of a page
				numpages++;
			if (ch == 'E')		// end of file, so load graphics and return
			{
				CA_MarkGrChunk(H_TOPWINDOWPIC);
				CA_MarkGrChunk(H_LEFTWINDOWPIC);
				CA_MarkGrChunk(H_RIGHTWINDOWPIC);
				CA_MarkGrChunk(H_BOTTOMINFOPIC);
				CA_CacheMarks ();
				text = textstart;
				return;
			}
			if (ch == 'G')		// draw graphic command, so mark graphics
			{
				ParsePicCommand ();
				CA_MarkGrChunk (picnum);
			}
			if (ch == 'T')		// timed draw graphic command, so mark graphics
			{
				ParseTimedCommand ();
				CA_MarkGrChunk (picnum);
			}
		}
		else
			text++;

	} while (text<bombpoint);

	Quit ("CacheLayoutGraphics: No ^E to terminate file!");
}
#endif


/*
=====================
=
= ShowArticle
=
=====================
*/

#ifdef JAPAN
void ShowArticle (int which)
#else
void ShowArticle (char far *article)
#endif
{
	#ifdef JAPAN
	int		snames[10] = {	H_HELP1PIC,
							H_HELP2PIC,
							H_HELP3PIC,
							H_HELP4PIC,
							H_HELP5PIC,
							H_HELP6PIC,
							H_HELP7PIC,
							H_HELP8PIC,
							H_HELP9PIC,
							H_HELP10PIC};
	int		enames[14] = {
							0,0,
							#ifndef JAPDEMO
							C_ENDGAME1APIC,
							C_ENDGAME1BPIC,
							C_ENDGAME2APIC,
							C_ENDGAME2BPIC,
							C_ENDGAME3APIC,
							C_ENDGAME3BPIC,
							C_ENDGAME4APIC,
							C_ENDGAME4BPIC,
							C_ENDGAME5APIC,
							C_ENDGAME5BPIC,
							C_ENDGAME6APIC,
							C_ENDGAME6BPIC
							#endif
							};
	#endif
	unsigned	oldfontnumber;
	unsigned	temp;
	boolean 	newpage,firstpage;

	#ifdef JAPAN
	pagenum = 1;
	if (!which)
		numpages = 10;
	else
		numpages = 2;

	#else

	text = article;
	oldfontnumber = fontnumber;
	fontnumber = 0;
	CA_MarkGrChunk(STARTFONT);
	VWB_Bar (0,0,320,200,BACKCOLOR);
	CacheLayoutGraphics ();
	#endif

	newpage = true;
	firstpage = true;

	do
	{
		if (newpage)
		{
			newpage = false;
			#ifdef JAPAN
			if (!which)
				CA_CacheScreen(snames[pagenum - 1]);
			else
				CA_CacheScreen(enames[which*2 + pagenum - 1]);
			#else
			PageLayout (true);
			#endif
			VW_UpdateScreen ();
			if (firstpage)
			{
				VL_FadeIn(0,255,&gamepal,10);
				// VW_FadeIn ()
				firstpage = false;
			}
		}

		LastScan = 0;
		while (!LastScan)
		;

		switch (LastScan)
		{
		case sc_UpArrow:
		case sc_PgUp:
		case sc_LeftArrow:
			if (pagenum>1)
			{
				#ifndef JAPAN
				BackPage ();
				BackPage ();
				#else
				pagenum--;
				#endif
				newpage = true;
			}
			break;

		case sc_Enter:
		case sc_DownArrow:
		case sc_PgDn:
		case sc_RightArrow:		// the text allready points at next page
			if (pagenum<numpages)
			{
				newpage = true;
				#ifdef JAPAN
				pagenum++;
				#endif
			}
			break;
		}

		#ifndef SPEAR
		if (Keyboard[sc_Tab] && Keyboard[sc_P] && MS_CheckParm("goobers"))
			PicturePause();
		#endif

	} while (LastScan != sc_Escape);

	IN_ClearKeysDown ();
	fontnumber = oldfontnumber;
}


//===========================================================================

#ifndef JAPAN
#ifdef ARTSEXTERN
int 	endextern = T_ENDART1;
#ifndef SPEAR
int		helpextern = T_HELPART;
#endif
#endif
char helpfilename[13] = "HELPART.",
	 endfilename[13] = "ENDART1.";
#endif

/*
=================
=
= HelpScreens
=
=================
*/
#ifndef SPEAR
void HelpScreens (void)
{
	int			artnum;
	char far 	*text;
	memptr		layout;


	CA_UpLevel ();
	MM_SortMem ();
#ifdef JAPAN
	ShowArticle (0);
	VW_FadeOut();
	FreeMusic ();
	CA_DownLevel ();
	MM_SortMem ();
#else




#ifdef ARTSEXTERN
	artnum = helpextern;
	CA_CacheGrChunk (artnum);
	text = (char _seg *)grsegs[artnum];
	MM_SetLock (&grsegs[artnum], true);
#else
	CA_LoadFile (helpfilename,&layout);
	text = (char _seg *)layout;
	MM_SetLock (&layout, true);
#endif

	ShowArticle (text);

#ifdef ARTSEXTERN
	MM_FreePtr (&grsegs[artnum]);
#else
	MM_FreePtr (&layout);
#endif



	VW_FadeOut();

	FreeMusic ();
	CA_DownLevel ();
	MM_SortMem ();
#endif
}
#endif

//
// END ARTICLES
//
void EndText (void)
{
	int			artnum;
	char far 	*text;
	memptr		layout;


	ClearMemory ();

	CA_UpLevel ();
	MM_SortMem ();
#ifdef JAPAN
	ShowArticle(gamestate.episode + 1);

	VW_FadeOut();

	SETFONTCOLOR(0,15);
	IN_ClearKeysDown();
	if (MousePresent)
		Mouse(MDelta);	// Clear accumulated mouse movement

	FreeMusic ();
	CA_DownLevel ();
	MM_SortMem ();
#else



#ifdef ARTSEXTERN
	artnum = endextern+gamestate.episode;
	CA_CacheGrChunk (artnum);
	text = (char _seg *)grsegs[artnum];
	MM_SetLock (&grsegs[artnum], true);
#else
	endfilename[6] = '1'+gamestate.episode;
	CA_LoadFile (endfilename,&layout);
	text = (char _seg *)layout;
	MM_SetLock (&layout, true);
#endif

	ShowArticle (text);

#ifdef ARTSEXTERN
	MM_FreePtr (&grsegs[artnum]);
#else
	MM_FreePtr (&layout);
#endif


	VW_FadeOut();
	SETFONTCOLOR(0,15);
	IN_ClearKeysDown();
	if (MousePresent)
		Mouse(MDelta);	// Clear accumulated mouse movement

	FreeMusic ();
	CA_DownLevel ();
	MM_SortMem ();
#endif
}
