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

 v_bord1.cc - Dialog border class with minimalistic look
              by Doug Eleveld

*/


#include "v_border.h"
#include "v_common.h"
#include "v_dialog.h"


//================================================================


void CDialogBorderNone::Draw(CDialog& aTarget, CBorder& aBorder)
{
  (void)aTarget;
  (void)aBorder;
}


void CDialogBorderNone::PositionTitle(CDialog& aTarget, CTitlebar& aTitle)
{
  int x = aTarget.Posx();
  int y = aTarget.Posy();
  int w = aTarget.Width();
  int h = aTarget.Height();

  x = 0;
  y = 0;
//  w -= 4;
  
  if (aTarget.GetFlags() & dlgClose)
    w -= CDialog::TitleSize;
  
  h = CDialog::TitleSize;

  aTitle.MoveTo(x,y);
  aTitle.ScaleTo(w,h);

  aTitle.SetScaleMode(sclRight);
}


void CDialogBorderNone::PositionClient(CDialog& aTarget, CClient& aClient)
{
  int x = aTarget.Posx();
  int y = aTarget.Posy();
  int w = aTarget.Width();
  int h = aTarget.Height();

  x = 0;
  y = CDialog::TitleSize;
//  w -= 4;
  h -= CDialog::TitleSize;
  
  aClient.MoveTo(x,y);
  aClient.ScaleTo(w,h);

  aClient.SetScaleMode(sclRight | sclBottom,0);
}


void CDialogBorderNone::PositionClose(CDialog& aTarget, CCloseButton& aClose)
{
  int x = aTarget.Posx();
  int y = aTarget.Posy();
  int w = aTarget.Width();
  int h = aTarget.Height();

  x = w-CDialog::TitleSize;
  y = 0;
  w = CDialog::TitleSize;
  h = CDialog::TitleSize;

  aClose.MoveTo(x,y);
  aClose.ScaleTo(w,h);
  
  aClose.SetScaleMode(sclLeft | sclRight,0);
}



