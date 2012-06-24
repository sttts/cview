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

 v_mouse.h - Mouse cursor management

*/


#ifndef V_MOUSE_H_INCLUDED
#define V_MOUSE_H_INCLUDED


#include "v_gfx.h"
#include "v_const.h"
#include "v_sysbit.h"


extern BITMAP *DefaultMouse;


extern void View_SetMouse(bmpBitmap aNum, int rx=0, int ry=0);
extern void View_SetMouse(BITMAP *Bitmap, int rx=0, int ry=0);
extern void View_ShowMouse(BITMAP *Buffer);
extern void View_RestoreMouse(BITMAP *Buffer);
extern void View_BlitMouse(BITMAP *Buffer);
extern void View_UpdateMouse(int mx = -1, int my = -1);
extern void View_PollMouse(int &x, int &y, int &sx, int &sy);
extern void View_GetMouseRect(int &x, int &y, int &sx, int &sy);
extern int View_GetDblClickTime();
extern void View_SetDblClickTime(int aTime);
extern int View_GetDblClickDist();
extern void View_SetDblClickDist(int aDist);


#endif


