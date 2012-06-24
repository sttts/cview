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
#include "v_gfx.h"


//================================================================


void CDialogBorderWin95::Draw(CDialog& aTarget, CBorder& aBorder)
{
  BITMAP *Buffer = aTarget.GetBuffer();

  CColor Light = aBorder.Color(colLight);
  CColor Dark = aBorder.Color(colDark);
  CColor Back = aBorder.Color(colBack);

  // Border around the entire dialog
  int px = aBorder.Posx();
  int py = aBorder.Posy();
  int pw = aBorder.Width();
  int ph = aBorder.Height();
  
  emptybutton(Buffer,px,py,pw,ph,Light,Dark,Back);
  Cgfx::HLine(Buffer,px+1,py+1,px+pw-2,Back);
  Cgfx::VLine(Buffer,px+1,py+2,py+CDialog::TitleSize+2,Back);
  Cgfx::VLine(Buffer,px+pw-2,py+2,py+CDialog::TitleSize+2,Back);

  // Line and border around the client
  CClient* Cli = aTarget.GetClient();
  
  px = Cli->Posx()-1;
  py = Cli->Posy()-1;
  pw = Cli->Width()+2;
  ph = Cli->Height()+2;
  
  emptybutton(Buffer,px,py,pw,ph,Dark,Light,Back);
  emptybutton(Buffer,px-1,py-1,pw+2,ph+2,Back,Back,Back);

  // Line around the close button
  CCloseButton* Close = aTarget.GetClose();
  if (Close!=NULL)
  {
    px = Close->Posx()-1;
    py = Close->Posy()-1;
    pw = Close->Width()+1;
    ph = Close->Height()+1;

    // Find what color it should be i.e. if the window is on top
    CTitlebar* Title = aTarget.GetTitle();

    CColor c = Title->Color(colBack);
    if (Title->GetOnTop())
      c = Title->Color(colSelect);

    // Draw the extra line
    Cgfx::Rect(Buffer,px,py,px+pw,py+ph,c);
  }
}


void CDialogBorderWin95::PositionTitle(CDialog& aTarget, CTitlebar& aTitle)
{
  int x = aTarget.Posx();
  int y = aTarget.Posy();
  int w = aTarget.Width();
  int h = aTarget.Height();

  x = 2;
  y = 2;
  w -= 4;
  
  if (aTarget.GetFlags() & dlgClose)
    w -= CDialog::TitleSize;
  
  h = CDialog::TitleSize;

  aTitle.MoveTo(x,y);
  aTitle.ScaleTo(w,h);

  aTitle.SetScaleMode(sclRight);
}


void CDialogBorderWin95::PositionClient(CDialog& aTarget, CClient& aClient)
{
  int x = aTarget.Posx();
  int y = aTarget.Posy();
  int w = aTarget.Width();
  int h = aTarget.Height();

  x = 3;
  y = CDialog::TitleSize+1+3;
  w -= 6;
  h -= CDialog::TitleSize+1+2+4;
  
  aClient.MoveTo(x,y);
  aClient.ScaleTo(w,h);
  
  aClient.SetScaleMode(sclRight | sclBottom,0);
}


void CDialogBorderWin95::PositionClose(CDialog& aTarget, CCloseButton& aClose)
{
  int x = aTarget.Posx();
  int y = aTarget.Posy();
  int w = aTarget.Width();
  int h = aTarget.Height();

  x = w-CDialog::TitleSize-1;
  y = 3;
  w = CDialog::TitleSize-2;
  h = CDialog::TitleSize-2;

  aClose.MoveTo(x,y);
  aClose.ScaleTo(w,h);
  
  aClose.SetScaleMode(sclLeft | sclRight,0);
}



