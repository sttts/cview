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

 v_bord1.cc - Dialog border class with Windows 3.1 look
              by Doug Eleveld

*/


#include "v_border.h"
#include "v_common.h"
#include "v_dialog.h"
#include "v_gfx.h"


//================================================================


void CDialogBorderWin31::Draw(CDialog& aTarget, CBorder& aBorder)
{
  BITMAP *Buffer = aTarget.GetBuffer();

  CColor Dark = aBorder.Color(colDark);
  CColor Light = aBorder.Color(colLight);

  int px = aBorder.Posx();
  int py = aBorder.Posy();
  int pw = aBorder.Width()-1;
  int ph = aBorder.Height()-1;

  // Outer rectangle
  Cgfx::Rect(Buffer,px,py,px+pw,py+ph,Dark);
  Cgfx::Rect(Buffer,px+1,py+1,px+pw-1,py+ph-1,Light);
  Cgfx::Rect(Buffer,px+2,py+2,px+pw-2,py+ph-2,Light);
  Cgfx::Rect(Buffer,px+3,py+3,px+pw-3,py+ph-3,Dark);

  // Little strips in outer rectangle
  Cgfx::HLine(Buffer,1,CDialog::TitleSize+4,2,Dark);
  Cgfx::HLine(Buffer,px+pw-2,CDialog::TitleSize+4,px+pw-1,Dark);

  Cgfx::VLine(Buffer,CDialog::TitleSize+4,1,2,Dark);
  Cgfx::VLine(Buffer,px+pw-CDialog::TitleSize-4,1,2,Dark);
  
  Cgfx::HLine(Buffer,1,py+ph-CDialog::TitleSize-4,2,Dark);
  Cgfx::HLine(Buffer,px+pw-2,py+ph-CDialog::TitleSize-4,px+pw-1,Dark);

  Cgfx::VLine(Buffer,CDialog::TitleSize+4,py+ph-2,py+ph-1,Dark);
  Cgfx::VLine(Buffer,px+pw-CDialog::TitleSize-4,py+ph-2,py+ph-1,Dark);
  
  // Seperating strip between Client and titlebar
  CClient* Cli = aTarget.GetClient();
  px = Cli->Posx();
  py = Cli->Posy();
  pw = Cli->Width();
  ph = Cli->Height();

  Cgfx::HLine(Buffer,px,py-1,px+pw-1,Dark);

  // Seperating line between between Close button and titlebar
  CCloseButton* Close = aTarget.GetClose();
  if(Close!=NULL)
  {
    px = Close->Posx();
    pw = Close->Width();
    Cgfx::VLine(Buffer,px+pw,4,CDialog::TitleSize+3,Dark);
  }
}


void CDialogBorderWin31::PositionTitle(CDialog& aTarget, CTitlebar& aTitle)
{
  int x = aTarget.Posx();
  int y = aTarget.Posy();
  int w = aTarget.Width();
  int h = aTarget.Height();

  x = 4;
  y = 4;
  w -= 8;

  if (aTarget.GetFlags() & dlgClose)
  {
    x += CDialog::TitleSize+1;
    w -= CDialog::TitleSize+1;
  }

  h = CDialog::TitleSize;

  aTitle.MoveTo(x,y);
  aTitle.ScaleTo(w,h);

  aTitle.SetScaleMode(sclRight);
}


void CDialogBorderWin31::PositionClient(CDialog& aTarget, CClient& aClient)
{
  int x = aTarget.Posx();
  int y = aTarget.Posy();
  int w = aTarget.Width();
  int h = aTarget.Height();

  x = 4;
  y = CDialog::TitleSize+1+2+2;
  w -= 8;
  h -= CDialog::TitleSize+1+2+1+5;
  
  aClient.MoveTo(x,y);
  aClient.ScaleTo(w,h);
  
  aClient.SetScaleMode(sclRight | sclBottom,0);
}


void CDialogBorderWin31::PositionClose(CDialog& aTarget, CCloseButton& aClose)
{
  int x = aTarget.Posx();
  int y = aTarget.Posy();
  int w = aTarget.Width();
  int h = aTarget.Height();

  x = 4;
  y = 4;
  w = CDialog::TitleSize;
  h = CDialog::TitleSize;

  aClose.MoveTo(x,y);
  aClose.ScaleTo(w,h);
  
  aClose.SetScaleMode(sclNone);
}



