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

 v_check.h - Checkbox object

*/


#ifndef V_CHECK_H_INCLUDED
#define V_CHECK_H_INCLUDED


#include "v_base.h"


class CCheckbox:public CView
{
 public:
  CCheckbox(int apx, int apy, char* aText = NULL, int aStatus = 0);
  virtual ~CCheckbox();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  virtual void HandleMouse(int mx, int my, int buttons);
  virtual void* HandleAccel(char aChar, char Scancode);
  virtual void Draw();

  void DefaultScale();

  int GetStatus() { return Status; }
  void SetStatus(int aStatus);

 protected:
  int Status;
  char* Text;
};


#endif
