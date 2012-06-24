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

 v_utils.cc - Different utility classes

*/


#include "v_utils.h"
#include "v_common.h"
#include "v_gfx.h"
#include "v_save.h"


//=====================================================


CFilledRect::CFilledRect(int apx, int apy, int asx, int asy, CColor aColor)
 : CView(apx, apy, asx, asy)
{
  Col = aColor;
}


CFilledRect::~CFilledRect()
{
}


void* CFilledRect::HandleEvent(cmCommand Event, void* Data)
{
  // Output serialize imformation
  if(Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CFilledRect P%p Col=%i }\n",this,Col);
    AddSerializeInfo(Data,info);
  }
    
  return CView::HandleEvent(Event,Data);
}


void CFilledRect::Draw()
{
  if (Owner!=NULL)
  {
    Cgfx::Rectfill(Owner->GetBuffer(), px, py, sx, sy, Col);
  }

  CView::Draw();
}


void CFilledRect::SetColor(CColor aColor)
{
  Col = aColor;
  Redraw();
}


CColor CFilledRect::GetColor()
{
  return Col;
}


//=====================================================


C3dIn::C3dIn(int apx, int apy, int asx, int asy, bool doFill)
 : CView(apx, apy, asx, asy),
 Fill(doFill)
{
}


C3dIn::~C3dIn()
{
}


void* C3dIn::HandleEvent(cmCommand Event, void* Data)
{
  // Output serialize imformation
  if(Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ C3dIn P%p Fill=%i }\n",this,Fill);
    AddSerializeInfo(Data,info);
  }
    
  return CView::HandleEvent(Event,Data);
}


void C3dIn::Draw()
{
  if (Owner!=NULL)
  {
    BITMAP *Buffer = Owner->GetBuffer();

    if (Fill)
      button(Buffer, px, py, sx, sy,
             Color(colDark), Color(colLight), Color(colBack));
    else
      emptybutton(Buffer, px, py, sx, sy,
             Color(colDark), Color(colLight), Color(colBack));
  }

  CView::Draw();
}


//=====================================================


C3dOut::C3dOut(int apx, int apy, int asx, int asy, bool doFill)
 : CView(apx, apy, asx, asy),
 Fill(doFill)
{
}


C3dOut::~C3dOut()
{
}


void* C3dOut::HandleEvent(cmCommand Event, void* Data)
{
  // Output serialize imformation
  if(Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ C3dOut P%p Fill=%i }\n",this,Fill);
    AddSerializeInfo(Data,info);
  }
    
  return CView::HandleEvent(Event,Data);
}


void C3dOut::Draw()
{
  if (Owner!=NULL)
  {
    BITMAP *Buffer = Owner->GetBuffer();

    if (Fill)
      button(Buffer, px, py, sx, sy,
             Color(colLight), Color(colDark), Color(colBack));
    else
      emptybutton(Buffer, px, py, sx, sy,
             Color(colLight), Color(colDark), Color(colBack));

  }

  CView::Draw();
}



