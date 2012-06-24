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

 v_pictur.h - Simple picture class

*/


#ifndef V_PICTURE_H_INCLUDED
#define V_PICTURE_H_INCLUDED


#include "v_base.h"


enum pctFlags { pctNone      = 0,
                pctCopy      = 1,
                pctDestroy   = 1<<1,
                pctTrans     = 1<<2,
                pctTiled     = 1<<3,
                pctCentered  = 1<<4,
                pctScale     = 1<<5,
                pctDefault   = pctNone,

                pctKeep      = 0xffff // Only for internal usage
              };


pctFlags operator|(const pctFlags one, const pctFlags two);


class CPicture:public CView
{
 public:
  CPicture(int apx, int apy, int asx,  int asy, BITMAP *aPicture,
    pctFlags aFlags=pctDefault);
  CPicture(int apx, int apy, int asx,  int asy, RLE_SPRITE *aPicture,
    pctFlags aFlags=pctDefault);
  virtual ~CPicture();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  virtual void Draw();

  void SetPicture(BITMAP *aPicture, pctFlags aFlags=pctKeep);
  void SetPicture(RLE_SPRITE *aPicture, pctFlags aFlags=pctKeep);

  virtual void DefaultScale();

 protected:
  BITMAP *Picture;
  int Flags;
};


#endif
