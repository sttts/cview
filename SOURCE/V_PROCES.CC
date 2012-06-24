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

 v_process.cc - Process bar

*/


//#include <string.h>
#include "v_proces.h"
#include "v_common.h"
#include "v_save.h"
//#include "v_fonts.h"
//#include "allegttf.h"


//=====================================================


CProcessBar::CProcessBar(int apx, int apy, int asx, int asy, int aStatus)
 : CView(apx, apy, asx, asy)
{
  Status = aStatus;
  if (Status<0) Status=0;
  if (Status>100) Status=100;
}


CProcessBar::~CProcessBar()
{
}


void* CProcessBar::HandleEvent(cmCommand Event, void* Data)
{
  if(Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CProcessbar P%p Status=%i }\n",this,Status);
    AddSerializeInfo(Data,info);
  }
  return CView::HandleEvent(Event, Data);
}


void CProcessBar::Draw()
{
  if (Owner!=NULL)
  {
    BITMAP *Buffer = Owner->GetBuffer();

    button(Buffer, px, py, sx, sy,
           Color(colDark), Color(colLight), Color(colBack));

    button(Buffer, px+1, py+1, (sx-2)*Status/100, sy-2,
           Color(colLight), Color(colDark), Color(colBack));
  }

  CView::Draw();
}


void CProcessBar::SetStatus(int aStatus)
{
  Status = aStatus;
  if (Status<0) Status=0;
  if (Status>100) Status=100;
  Redraw();
}

