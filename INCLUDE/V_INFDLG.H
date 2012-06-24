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

 v_infdlg.h - Program info dialog class header

*/


#ifndef V_INFDLG_H_INCLUDED
#define V_INFDLG_H_INCLUDED


#include "v_dialog.h"


class CInfoDlg:public CDialog
{
 public:
  CInfoDlg(char* aText, char* aFilename);
  virtual ~CInfoDlg();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);
};

#endif
