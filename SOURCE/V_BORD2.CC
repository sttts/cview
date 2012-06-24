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

 v_bord1.cc - Dialog border class with flat look
              by Doug Eleveld

*/


#include "v_border.h"
#include "v_common.h"
#include "v_dialog.h"


//================================================================


void CDialogBorderFlat::Draw(CDialog& aTarget, CBorder& aBorder)
{
  BITMAP *Buffer = aTarget.GetBuffer();

  // Border around the entire dialog
  int px = aBorder.Posx();
  int py = aBorder.Posy();
  int pw = aBorder.Width();
  int ph = aBorder.Height();
  
  emptybutton(Buffer,px,py,pw,ph,aBorder.Color(colLight),aBorder.Color(colDark),aBorder.Color(colBack));
  emptybutton(Buffer,px+1,py+1,pw-2,ph-2,aBorder.Color(colDark),aBorder.Color(colLight),aBorder.Color(colBack));
  
  // Border around the titlebar
  CTitlebar* Tit = aTarget.GetTitle();
  
  px = Tit->Posx()-1;
  py = Tit->Posy()-1;
  pw = Tit->Width()+2;
  ph = Tit->Height()+2;
  
  emptybutton(Buffer,px,py,pw,ph,aBorder.Color(colLight),aBorder.Color(colDark),aBorder.Color(colBack));
}


void CDialogBorderFlat::PositionTitle(CDialog& aTarget, CTitlebar& aTitle)
{
  int x = aTarget.Posx();
  int y = aTarget.Posy();
  int w = aTarget.Width();
  int h = aTarget.Height();

  x = 3;
  y = 3;
  w -= 6;
  
  if (aTarget.GetFlags() & dlgClose)
    w -= CDialog::TitleSize+2;
  
  h = CDialog::TitleSize;

  aTitle.MoveTo(x,y);
  aTitle.ScaleTo(w,h);
  
  aTitle.SetScaleMode(sclRight);
}


void CDialogBorderFlat::PositionClient(CDialog& aTarget, CClient& aClient)
{
  int x = aTarget.Posx();
  int y = aTarget.Posy();
  int w = aTarget.Width();
  int h = aTarget.Height();

  x = 2;
  y = CDialog::TitleSize+1+3;
  w -= 4;
  h -= CDialog::TitleSize+1+2+3;
  
  aClient.MoveTo(x,y);
  aClient.ScaleTo(w,h);
  
  aClient.SetScaleMode(sclRight | sclBottom,0);
}


void CDialogBorderFlat::PositionClose(CDialog& aTarget, CCloseButton& aClose)
{
  int x = aTarget.Posx();
  int y = aTarget.Posy();
  int w = aTarget.Width();
  int h = aTarget.Height();

  x = w-CDialog::TitleSize-4;
  y = 2;
  w = CDialog::TitleSize+2;
  h = CDialog::TitleSize+2;

  aClose.MoveTo(x,y);
  aClose.ScaleTo(w,h);

  aClose.SetScaleMode(sclLeft | sclRight,0);
}



