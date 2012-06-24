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

 v_text.h - Simple static text

*/


#ifndef V_TEXT_H_INCLUDED
#define V_TEXT_H_INCLUDED


#include "v_base.h"


enum txtFlags { txtNone        = 0,
                txtCentered    = 1,
                txtTransparent = 1<<1,

                txtDefault     = txtTransparent
              };


txtFlags operator|(const txtFlags one, const txtFlags two);
              

class CText:public CView
{
 public:
  CText(int apx, int apy, char *aText, txtFlags aFlags=txtDefault);
  CText(int apx, int apy, int asx, int asy, char *aText, txtFlags aFlags=txtDefault);
  CText(int apx, int apy, int asx, char *aText, txtFlags aFlags=txtDefault);
  virtual ~CText();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  virtual void Draw();

  virtual void DefaultScale();
  
  void SetText(char *aText);

 protected:
  char *Text;
  char **Lines;
  int LineNum;
  txtFlags Flags;
  int dx, dy;
  int AutoWidth;
  int AutoHeight;
};


#endif
