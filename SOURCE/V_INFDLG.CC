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

 v_infdlg.cc - Program info dialog class

*/


#include "v_infdlg.h"
#include "v_button.h"
#include "v_picture.h"
#include "v_common.h"
#include "v_save.h"

#include <allegro.h>


//===================================================


CInfoDlg::CInfoDlg(char* aText, char* aFilename)
 : CDialog(0, 0, 20, 10, aText, dlgClose|
                                dlgDrag|
                                dlgEscape)
{
  RGB Pal[256];
  BITMAP *b = load_pcx(aFilename, Pal);
  View_PaletteConvert(b,Pal);
  
  Client->Insert(new CPicture(0, 0, b->w, b->h, b, pctDestroy));
  Client->Insert(new CTransButton(0, 0, b->w, b->h, cmClose));

  Enclose(0);
  Center();
}


CInfoDlg::~CInfoDlg()
{
}


void *CInfoDlg::HandleEvent(cmCommand Event, void* Data)
{
  if(Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CInfoDlg P%p }\n",this);
    AddSerializeInfo(Data,info);
  }
  
  return CDialog::HandleEvent(Event,Data);
}

