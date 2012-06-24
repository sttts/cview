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

 v_scoll.h - Scroll bar class

*/


#ifndef V_SCROLL_H_INCLUDED
#define V_SCROLL_H_INCLUDED


#include "v_base.h"


const scrFastScroll     = 0x0001;
const scrDefault        = scrFastScroll;


class CScrollbar:public CGroup
{
 public:
  CScrollbar(int apx, int apy, int asx, int asy, CView *aTarget=NULL,
    int aFlags=scrDefault);
  virtual ~CScrollbar();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  void SetTarget(CView *aTarget);
  virtual void ScaleTo(int x, int y);

  void Update(int aPos, int aMax, int aPageSize);
  int GetNewPos();

 protected:
  CView *Target;
  class CDragger *Dragger;
  int Vertical;
  int Flags;
};


#endif
