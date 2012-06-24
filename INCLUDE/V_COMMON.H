/*

    _______   _    __ __
   /\  ___ \/\ \  /| |\_\
   \ \ \_/\_\ \ \ || |/_/
    \ \ \\/_/\ \ \|| |  __     __  __        __
     \ \ \   _\ \ \| | /\ \  /'__'\\ \  /\  / /
      \ \ \__\ \ \   | \ \ \/\  __/ \ \/  \/ /
       \ \______\ \__|  \ \_\ \____\ \__/\__/
        \/______/\/__/   \/_/\/____/\/__\/_/

           A window library for Allegro

 (c) By Stefan Schimanski & Janko Heilgeist '1997/1998


                  1Stein@gmx.de
              Janko.Heilgeist@gmx.de

            http://1Stein.home.pages.de

 -----------------------------------------------------

 v_common.h - Common helper function

*/

#ifndef V_COMMON_H_INCLUDED
#define V_COMMON_H_INCLUDED

//#include "allegro.h"
#include "v_comman.h"
#include "v_gfx.h"
#include "v_const.h"

#ifndef min
  #define min(a,b) ((a<b)?a:b)
#endif

#ifndef max
  #define max(a,b) ((a<b)?b:a)
#endif


extern class CView *Focused;
extern class CGroup *Bill; // the ruling object
extern class CView *Dragged;
extern class CProgram *Program;
extern int Dragx;
extern int Dragy;
//extern int *DefaultPal;
extern int DebugMode;

extern bool View_Init(int sx, int sy, int depth);
extern void View_Done();

extern bool View_SetVideoMode(int sx, int sy, int depth);
//extern int View_Screenx;
//extern int View_Screeny;
//extern void View_SetPalette(int *Pal);
extern void View_SetDebugMode(int aMode=1);

extern cmCommand ExecuteWindow(class CWindow *Win, int aForeground=0);
extern void InsertWindow(class CWindow *Win);
extern void RemoveWindow(class CWindow *Win);

// Helper drawing functions
void button(BITMAP *Buffer, int px, int py, int sx, int sy,
            CColor lu, CColor rd, CColor in);
void emptybutton(BITMAP *Buffer, int px, int py, int sx, int sy,
            CColor lu, CColor rd, CColor in);
void panel(BITMAP *Buffer, int px, int py, int sx, int sy,
            CColor lu, CColor rd, CColor in);
void emptypanel(BITMAP *Buffer, int px, int py, int sx, int sy,
            CColor lu, CColor rd);

// Function to convert a 8 bit image with a palette
// to use the current palette
void View_PaletteConvert(BITMAP* bit, RGB* in, bool preserve_trans = true);
BITMAP* View_FixupBitmap(BITMAP* bmp, RGB* pal);

// Scaling operations
sclScaling operator|(const sclScaling one, const sclScaling two);

// return the hotkey from a string
char ViewAccel (char *string);

// Give a pointer to a Accel in a string, else NULL
char* ViewAccelMarker (char *string);

// Gui text output funbction properly handles the & char for Accels
int ViewTextout (BITMAP *bmp, Cgfx::Font& font, char *string, int x, int y, CColor color, bool underline = true);

// Find the length in pixels of text string, properly handling Accels
int ViewTextLength (Cgfx::Font& font, char *string);


// Some helper functions for debugging
#include <stdio.h>
#include <iostream.h>
#include <string.h>

#define LOG(x)   clog<<#x" = "<<x<<"\n"
#define COVERAGE clog<<__FILE__<<" ("<<__LINE__<<") "<<__PRETTY_FUNCTION__<<"\n"
#define FLOG     clog<<__PRETTY_FUNCTION__<<"\n"
#define LLOG     clog<<"line "<<__LINE__<<"\n"
#define TLOG(x)  clog<<#x" = "<<x<<" in "<<__FILE__<<" ("<<__LINE__<<") "<<__PRETTY_FUNCTION__<<"\n"
#define VLLOG(x) clog<<#x" = "<<x<<" at line "<<__LINE__<<"\n"
#define VFLOG(x) clog<<#x" = "<<x<<" in "<<__PRETTY_FUNCTION__<<" line "<<__LINE__<<"\n"

#endif
