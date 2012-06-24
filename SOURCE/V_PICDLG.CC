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

 v_picdlg.cc - Program picture dialog class

*/

#include <stdio.h>
#include "v_picdlg.h"
#include "v_picture.h"
#include "v_save.h"

#include <allegro.h>


//===================================================


CPicDialog::CPicDialog(int apx, int apy, char *aTitle, BITMAP *b)
 : CDialog(apx, apy, 10, 10, aTitle, dlgDefault)
{
  CView *v = new CPicture(0, 0, b->w, b->h, b, pctDestroy);
  Client->Insert(v);
  Enclose(0);
}


CPicDialog::~CPicDialog()
{
}


void *CPicDialog::HandleEvent(cmCommand Event, void* Data)
{
  if(Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CPicDialog P%p }\n",this);
    AddSerializeInfo(Data,info);
  }
  
  return CDialog::HandleEvent(Event,Data);
}

