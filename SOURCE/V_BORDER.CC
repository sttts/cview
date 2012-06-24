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

 v_bord1.cc - Default Dialog border class
              by Doug Eleveld

*/


#include "v_border.h"
#include "v_common.h"
#include "v_dialog.h"


//================================================================


// The default border style
CDialogBorderStyle CDialogBorderStyle::Default;

// The default border drawer
CDialogBorderStyle::CDefault CDialogBorderStyle::DefaultDrawer;


void CDialogBorderStyle::CDefault::Draw(CDialog& aTarget, CBorder& aBorder)
{
  BITMAP *Buffer = aTarget.GetBuffer();

  // Border around the client
  CClient* Cli = aTarget.GetClient();
  
  int px = Cli->Posx()-1;
  int py = Cli->Posy()-1;
  int pw = Cli->Width()+2;
  int ph = Cli->Height()+2;
  
  emptybutton(Buffer,px,py,pw,ph,aBorder.Color(colLight),aBorder.Color(colDark),aBorder.Color(colBack));

  // Border around the titlebar
  CTitlebar* Tit = aTarget.GetTitle();
  
  px = Tit->Posx()-1;
  py = Tit->Posy()-1;
  pw = Tit->Width()+2;
  ph = Tit->Height()+2;
  
  emptybutton(Buffer,px,py,pw,ph,aBorder.Color(colLight),aBorder.Color(colDark),aBorder.Color(colBack));
}


void CDialogBorderStyle::CDefault::PositionTitle(CDialog& aTarget, CTitlebar& aTitle)
{
  int x = aTarget.Posx();
  int y = aTarget.Posy();
  int w = aTarget.Width();
  int h = aTarget.Height();

  x = 1;
  y = 1;
  w -= 2;
  
  if (aTarget.GetFlags() & dlgClose)
    w -= CDialog::TitleSize+2;
  
  h = CDialog::TitleSize;

  aTitle.MoveTo(x,y);
  aTitle.ScaleTo(w,h);
  
  aTitle.SetScaleMode(sclRight);
}


void CDialogBorderStyle::CDefault::PositionClient(CDialog& aTarget, CClient& aClient)
{
  int w = aTarget.Width();
  int h = aTarget.Height();

  int x = 1;
  int y = CDialog::TitleSize+1+2;
  w -= 2;
  h -= CDialog::TitleSize+1+2+1;
  
  aClient.MoveTo(x,y);
  aClient.ScaleTo(w,h);

  aClient.SetScaleMode(sclRight | sclBottom,0);
}


void CDialogBorderStyle::CDefault::PositionClose(CDialog& aTarget, CCloseButton& aClose)
{
  int w = aTarget.Width();

  int x = w-CDialog::TitleSize-2;
  int y = 0;
  w = CDialog::TitleSize+2;
  int h = CDialog::TitleSize+2;
  
  aClose.MoveTo(x,y);
  aClose.ScaleTo(w,h);

  aClose.SetScaleMode(sclLeft | sclRight,0);
}



