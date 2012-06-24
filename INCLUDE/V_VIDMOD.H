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

 v_vidmod.h - Video mode selector dialog class
              by Doug Eleveld

*/


#ifndef V_VIDMOD_H_INCLUDED
#define V_VIDMOD_H_INCLUDED


#include "v_dialog.h"


class CText;
class CTextButton;
class CCombobox;


class CVideoModeDlg:public CDialog
{
 public:
  CVideoModeDlg();
  virtual ~CVideoModeDlg();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);

 protected:
  CText* Size;
  CText* Depth;

  CTextButton* OkButton;
  CTextButton* CancelButton;

  CCombobox* SizeList;
  CCombobox* DepthList;
};



#endif
