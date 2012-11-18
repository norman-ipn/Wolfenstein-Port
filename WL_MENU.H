//
// WL_MENU.H
//
//

#ifndef __WL_MENU_H__
#define __WL_MENU_H__

#include "ID_IN.H" // for ControlInfo structure 


#ifdef SPEAR

const int BORDCOLOR = 0x99
const int BORD2COLOR = 0x93
const int DEACTIVE = 0x9b
const int BKGDCOLOR = 0x9d
//#define STRIPE		0x9c

#define MenuFadeOut()	VL_FadeOut(0,255,0,0,51,10)

#else

const int BORDCOLOR = 0x29
const int BORD2COLOR = 0x23
const int DEACTIVE = 0x2b
const int BKGDCOLOR = 0x2d
const int STRIPE = 0x2c

#define MenuFadeOut()	VL_FadeOut(0,255,43,0,0,10)

#endif

const int READCOLOR = 0x4a
const int READHCOLOR = 0x47
const int VIEWCOLOR = 0x7f
const int TEXTCOLOR = 0x17
const int HIGHLIGHT = 0x13
#define MenuFadeIn()	VL_FadeIn(0,255,&gamepal,10)


#define MENUSONG	WONDERIN_MUS

#ifndef SPEAR
#define INTROSONG	NAZI_NOR_MUS
#else
#define INTROSONG	XTOWER2_MUS
#endif

const int SENSITIVE	60
const int CENTER		SENSITIVE*2

const int MENU_X = 76
const int MENU_Y = 55
const int MENU_W = 178
#ifndef SPEAR
const int MENU_H = 13*10+6
#else
const int MENU_H = 13*9+6
#endif

const int SM_X = 48
const int SM_W = 250

const int SM_Y1	= 20
const int SM_H1	= 4*13-7
const int SM_Y2	= SM_Y1+5*13
const int SM_H2	= 4*13-7
const int SM_Y3	= SM_Y2+5*13
const int SM_H3	= 3*13-7

const int CTL_X	= 24
const int CTL_Y	= 70
const int CTL_W	= 284
const int CTL_H	= 13*7-7

const int LSM_X	= 85
const int LSM_Y	= 55
const int LSM_W	= 175
const int LSM_H	= 10*13+10

const int NM_X	= 50
const int NM_Y	= 100
const int NM_W	= 225
const int NM_H	= 13*4+15

const int NE_X	= 10
const int NE_Y	= 23
const int NE_W	= 320-NE_X*2
const int NE_H	= 200-NE_Y*2

const int CST_X = 20
const int CST_Y = 48
const int CST_START = 60
const int CST_SPC = 60


//
// TYPEDEFS
//
typedef struct {
		int x,y,amount,curpos,indent;
		} CP_iteminfo;

typedef struct {
		int active;
		char string[36];
		void (* routine)(int temp1);
		} CP_itemtype;

typedef struct {
		int allowed[4];
		} CustomCtrls;

extern CP_itemtype /*far*/ MainMenu[],/*far*/ NewEMenu[];
extern CP_iteminfo MainItems;

//
// FUNCTION PROTOTYPES
//
void SetupControlPanel(void);
void CleanupControlPanel(void);

void DrawMenu(CP_iteminfo *item_i,CP_itemtype /*far*/ *items);
int  HandleMenu(CP_iteminfo *item_i,
		CP_itemtype /*far*/ *items,
		void (*routine)(int w));
void ClearMScreen(void);
void DrawWindow(int x,int y,int w,int h,int wcolor);
void DrawOutline(int x,int y,int w,int h,int color1,int color2);
void WaitKeyUp(void);
void ReadAnyControl(ControlInfo *ci);
void TicDelay(int count);
void CacheLump(int lumpstart,int lumpend);
void UnCacheLump(int lumpstart,int lumpend);
void StartCPMusic(int song);
int  Confirm(char /*far*/ *string);
void Message(char /*far*/ *string);
void CheckPause(void);
void ShootSnd(void);
void CheckSecretMissions(void);
void BossKey(void);

void DrawGun(CP_iteminfo *item_i,CP_itemtype /*far*/ *items,int x,int *y,int which,int basey,void (*routine)(int w));
void DrawHalfStep(int x,int y);
void EraseGun(CP_iteminfo *item_i,CP_itemtype /*far*/ *items,int x,int y,int which);
void SetTextColor(CP_itemtype /*far*/ *items,int hlight);
void DrawMenuGun(CP_iteminfo *iteminfo);
void DrawStripes(int y);

void DefineMouseBtns(void);
void DefineJoyBtns(void);
void DefineKeyBtns(void);
void DefineKeyMove(void);
void EnterCtrlData(int index,CustomCtrls *cust,void (*DrawRtn)(int),void (*PrintRtn)(int),int type);

void DrawMainMenu(void);
void DrawSoundMenu(void);
void DrawLoadSaveScreen(int loadsave);
void DrawNewEpisode(void);
void DrawNewGame(void);
void DrawChangeView(int view);
void DrawMouseSens(void);
void DrawCtlScreen(void);
void DrawCustomScreen(void);
void DrawLSAction(int which);
void DrawCustMouse(int hilight);
void DrawCustJoy(int hilight);
void DrawCustKeybd(int hilight);
void DrawCustKeys(int hilight);
void PrintCustMouse(int i);
void PrintCustJoy(int i);
void PrintCustKeybd(int i);
void PrintCustKeys(int i);

void PrintLSEntry(int w,int color);
void TrackWhichGame(int w);
void DrawNewGameDiff(int w);
void FixupCustom(int w);

void CP_NewGame(void);
void CP_Sound(void);
int  CP_LoadGame(int quick);
int  CP_SaveGame(int quick);
void CP_Control(void);
void CP_ChangeView(void);
void CP_ExitOptions(void);
void CP_Quit(void);
void CP_ViewScores(void);
int  CP_EndGame(void);
int  CP_CheckQuick(unsigned scancode);
void CustomControls(void);
void MouseSensitivity(void);

void CheckForEpisodes(void);

//
// VARIABLES
//
extern int SaveGamesAvail[10],StartGame,SoundStatus;
extern char SaveGameNames[10][32],SaveName[13];

enum {MOUSE,JOYSTICK,KEYBOARDBTNS,KEYBOARDMOVE};	// FOR INPUT TYPES

enum
{
	newgame,
	soundmenu,
	control,
	loadgame,
	savegame,
	changeview,

#ifndef GOODTIMES
#ifndef SPEAR
	readthis,
#endif
#endif

	viewscores,
	backtodemo,
	quit
} menuitems;

//
// WL_INTER
//
typedef struct {
		int kill,secret,treasure;
		long time;
		} LRstruct;

extern LRstruct LevelRatios[];

void Write (int x,int y,char *string);
void NonShareware(void);
int GetYorN(int x,int y,int pic);
#endif//__WL_MENU_H__
