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

 v_picdlg.h - Program picture dialog class header

*/


#ifndef V_PICDLG_H_INCLUDED
#define V_PICDLG_H_INCLUDED


#include "v_dialog.h"


class CPicDialog:public CDialog
{
 public:
  CPicDialog(int apx, int apy, char *aTitle, BITMAP *b);
  virtual ~CPicDialog();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);
};


#endif
