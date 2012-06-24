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

 v_scoll.cc - Scroll bar class

*/


#include "v_scroll.h"
#include "v_button.h"
#include "v_utils.h"
#include "v_common.h"
#include "v_gfx.h"
#include "v_sysbit.h"
#include "v_save.h"

#include <allegro.h>


//=====================================================


class CDragger:public CView
{
 public:
  CDragger(int apx, int apy, int asx, int asy, int aVertical);
  virtual ~CDragger();

  virtual void HandleMouse(int mx, int my, int buttons);
  virtual void* HandleEvent(cmCommand Event, void* Data=0);

  virtual void Draw();

  void Update();
  void Update(int aPos, int aMax, int aPageSize);
  int GetNewPos();
  void ScrollTo(int p);

 protected:
  int Vertical;

  int Max;
  int Pos;
  int NewPos;
  int PageSize;
  int ScrPageSize;

  int ScreenPos();
  int Logical2Screen(int p);
  int Screen2Logical(int p);
};


CDragger::CDragger(int apx, int apy, int asx, int asy, int aVertical)
 : CView(apx, apy, asx, asy)
{
  Vertical = aVertical;

  Update(0, 10, 2);
}


CDragger::~CDragger()
{
}


void* CDragger::HandleEvent(cmCommand Event, void* Data)
{
  switch(Event)
  {
    case cmUp: ScrollTo(Pos-1); break;
    case cmDown: ScrollTo(Pos+1); break;

    case cmSerialize:
    {
      char info[1000];
      sprintf(info,"{ CDragger P%p }\n",this);
      AddSerializeInfo(Data,info);
      return CView::HandleEvent(Event, Data);
    }
                       
    default: { };
  }

  return CView::HandleEvent(Event, Data);
}


void CDragger::HandleMouse(int mx, int my, int buttons)
{
  int m;
  if (Vertical)
    m = my-py;
  else
    m = mx-px;

  if (Dragging)
  {
    if (buttons!=butLeft) Drop(); else
    if (my!=Oldmy || mx!=Oldmx)
    {
      ScrollTo(Screen2Logical(m));
    }
  } else
  {
    if (Oldbuttons==0 && buttons==butLeft)
    {
      if (m>=ScreenPos() && m<ScreenPos()+ScrPageSize)
      {
        if (Vertical)
          Drag(0, m-ScreenPos());
        else
          Drag(m-ScreenPos(), 0);
      } else
      {
        if (m>ScreenPos()) ScrollTo(Pos+PageSize); else
          ScrollTo(Pos-PageSize);
      }
    }
  }

  CView::HandleMouse(mx, my, buttons);
}


void CDragger::Draw()
{
  if (Owner!=NULL)
  {
    BITMAP *Buffer = Owner->GetBuffer();

    // Create a pattern for drawing on the scrollbar
    BITMAP *pattern = Cgfx::CreateBitmap(2, 2);
    putpixel(pattern, 0, 1, Color(colBack));
    putpixel(pattern, 1, 0, Color(colBack));
    putpixel(pattern, 0, 0, Color(colDark));
    putpixel(pattern, 1, 1, Color(colDark));

    // Draw the patteren background
	 drawing_mode(DRAW_MODE_COPY_PATTERN, pattern, 0, 0);
//    Cgfx::Rectfill(Buffer, px, py, px+sx-1, py+sy-1, 0);

    int p;
    int h = ScrPageSize;
    if (Vertical)
    {
      p = ScreenPos()+py;
      Cgfx::Rectfill(Buffer, px, py, px+sx-1, p-1, (CColor)0);
      Cgfx::Rectfill(Buffer, px, p+h, px+sx-1, py+sy-1, (CColor)0);
    } else
    {
      p = ScreenPos()+px;
      Cgfx::Rectfill(Buffer, px, py, p-1, py+sy-1, (CColor)0);
      Cgfx::Rectfill(Buffer, p+h, py, px+sx-1, py+sy-1, (CColor)0);
    }

	 solid_mode();
    Cgfx::DestroyBitmap(pattern);

    if (Vertical)
    {
      Cgfx::Line(Buffer, px     , p     , px+sx-2, p    , Color(colLight));
      Cgfx::Line(Buffer, px     , p+1   , px     , p+h-2, Color(colLight));
      Cgfx::Line(Buffer, px+sx-1, p+1   , px+sx-1, p+h-2, Color(colDark));
      Cgfx::Line(Buffer, px+1   , p+h-1 , px+sx-1, p+h-1, Color(colDark));

      Cgfx::Rectfill(Buffer, px+1, p+1, px+sx-2, p+h-2, Color(colBack));

    } else
    {
      Cgfx::Line(Buffer, p     , py     , p+h-2, py    , Color(colLight));
      Cgfx::Line(Buffer, p     , py+1   , p    , py+sy-2, Color(colLight));
      Cgfx::Line(Buffer, p+h-1,  py+1   , p+h-1, py+sy-2, Color(colDark));
      Cgfx::Line(Buffer, p+1   , py+sy-1, p+h-1, py+sy-1, Color(colDark));

      Cgfx::Rectfill(Buffer, p+1, py+1, p+h-2, py+sy-2, Color(colBack));
    }
  }
}


int CDragger::GetNewPos()
{
  return NewPos;
}


void CDragger::Update()
{
  if (Vertical)
    ScrPageSize = int((PageSize*sy)/(double)(Max+1.));
  else
    ScrPageSize = int((PageSize*sx)/(double)(Max+1.));

  if (ScrPageSize<3) ScrPageSize=3;
}


void CDragger::Update(int aPos, int aMax, int aPageSize)
{
  Max = aMax;
  if (Max<=0)
  {
    Max=0;
    PageSize = 1;
  } else
  {
    PageSize = aPageSize;
    if (PageSize>Max) PageSize=Max;
  }

  Pos = aPos;
  if (Pos<0) Pos=0;
  if (Pos>Max) Pos=Max;

  Update();
}


void CDragger::ScrollTo(int p)
{
  if (p<0) p=0;
  if (p>Max) p=Max;

  NewPos = p;

  if (Owner!=NULL) Owner->HandleEvent(cmDragger);
}


int CDragger::ScreenPos()
{
  return Logical2Screen(Pos);
}


int CDragger::Logical2Screen(int p)
{
  if (Max==0) return 0;

  if (Vertical)
    return p*(sy-ScrPageSize)/Max;
  else
    return p*(sx-ScrPageSize)/Max;
}


int CDragger::Screen2Logical(int p)
{
  if (Vertical)
  {
    if (sy-ScrPageSize==0) return 0;
    return p*Max/(sy-ScrPageSize);
  } else
  {
    if (sx-ScrPageSize==0) return 0;
    return p*Max/(sx-ScrPageSize);
  }
}


//=====================================================


CScrollbar::CScrollbar(int apx, int apy, int asx, int asy, CView *aTarget,
  int aFlags)
 : CGroup(apx, apy, asx, asy)
{
  Flags = aFlags;
  Target = aTarget;

  btnFlags ButtonFlags = btnDefault;
  
  if (Flags & scrFastScroll) ButtonFlags = ButtonFlags | btnMultiEvent;
  if (sy>sx) Vertical=1; else Vertical=0;

  if (Vertical)
  {
    CView *v = new C3dIn(0, sx, sx, sy-2*9, false);
    v->SetScaleMode(sclRight | sclBottom);
    Insert(v);

    Dragger = new CDragger(1, sx+1, sx-2, sy-2*sx-2, Vertical);
    Dragger->SetScaleMode(sclRight | sclBottom);
    Insert(Dragger);

    v = new CSystemBitmapButton(0, 0, sx, sx, bmpArrowUpOut,
              bmpArrowUpIn, cmUp, NULL, ButtonFlags);
    v->SetScaleMode(sclRight);
    Insert(v);

    v = new CSystemBitmapButton(0, sy-sx, sx, sx, bmpArrowDownOut,
              bmpArrowDownIn, cmDown, NULL, ButtonFlags);
    v->SetScaleMode(sclRight | sclTop | sclBottom);
    Insert(v);
  } else
  {
    CView *v = new C3dIn(sy, 0, sx-2*sy, sy, false);
    v->SetScaleMode(sclRight | sclBottom);
    Insert(v);

    Dragger = new CDragger(sy+1, 1, sx-2*sy-2, sy-2, Vertical);
    Dragger->SetScaleMode(sclRight | sclBottom);
    Insert(Dragger);

    v = new CSystemBitmapButton(0, 0, sy, sy, bmpArrowLeftOut,
              bmpArrowLeftIn, cmUp, NULL, ButtonFlags);
    v->SetScaleMode(sclBottom);
    Insert(v);

    v = new CSystemBitmapButton(sx-sy, 0, sy, sy, bmpArrowRightOut,
              bmpArrowRightIn, cmDown, NULL, ButtonFlags);
    v->SetScaleMode(sclLeft | sclRight | sclBottom);
    Insert(v);
  }
}


CScrollbar::~CScrollbar()
{
}


void* CScrollbar::HandleEvent(cmCommand Event, void* Data)
{
  switch (Event)
  {
    case cmDragger: if (Target!=NULL)
                      Target->HandleEvent(cmScrolled, this);
                    else
                      Owner->HandleEvent(cmScrolled, this);
                    return NULL;

    case cmSerialize:
    {
      char info[1000];
      sprintf(info,"{ CScrollbar P%p Target=%p Vertical=%i Flags=%i }\n",this,Target,Vertical,Flags);
      AddSerializeInfo(Data,info);
      break;
    }
                       
    default: { };
  }
  return CGroup::HandleEvent(Event, Data);
}


void CScrollbar::SetTarget(CView *aTarget)
{
  Target = aTarget;
}


void CScrollbar::Update(int aPos, int aMax, int aPageSize)
{
  Dragger->Update(aPos, aMax, aPageSize);
  Dragger->Redraw();
}


int CScrollbar::GetNewPos()
{
  return Dragger->GetNewPos();
}


void CScrollbar::ScaleTo(int x, int y)
{
  CGroup::ScaleTo(x, y);
  Dragger->Update();
}
