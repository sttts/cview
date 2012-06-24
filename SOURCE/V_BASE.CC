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

 v_base.cc - Base classes

*/


#include <stdio.h>
#include <stdlib.h>
#include "v_common.h"
#include "v_base.h"
#include "v_prog.h"
#include "v_mouse.h"
#include "v_color.h"
#include "v_rect.h"
#include "v_macro.h"
#include "v_save.h"

#include <allegro.h>


//#define SYNC
//#define VSYNC
//#define DEFAULTBUFFEROBJECTS
//#define BUFFEREDWINDOWS
#define RECURSIVEWINDOWS


//============================================================


int CView::DefaultSpacing = 5;


CView::CView(int apx, int apy, int asx, int asy)
  :Next(NULL),
  Prev(NULL),
  Owner(NULL),
  px(apx),
  py(apy),
  sx(asx),
  sy(asy),
  Value(cmNone),
  ScaleMode(sclNone),
  ScaleModeOffset(0),
  Oldmx(0),
  Oldmy(0),
  Oldbuttons(0),
  Dragging(0),
  Mouse(NULL),
  Mouserx(0),
  Mousery(0),
  SentencedToDeath(0),
  Transparent(0)
{
}


CView::~CView()
{
  if (Owner!=NULL)
  {
    Owner->Update(px, py, sx, sy);
    Owner->Remove(this);
  }
  if (Focused==this) Focused=NULL;
  if (Dragging) Drop();
}


void CView::HandleMouse(int mx, int my, int buttons)
{
  Oldbuttons = buttons;
  Oldmx = mx;
  Oldmy = my;
}


void CView::MouseLost(int mx, int my, int buttons)
{
(void)mx;
(void)my;
(void)buttons;
}


void CView::MouseGot(int mx, int my, int buttons)
{
  Oldbuttons = buttons;
  Oldmx = mx;
  Oldmy = my;
}


void* CView::HandleEvent(cmCommand Event, void* Data)
{
  // Check for a change in color scheme
  if(Event==cmColorScheme)
  {
    CColorInterface::rep* newcolor = (CColorInterface::rep*)Data;
    if (newcolor!=NULL)
    {
      ColorInterface = *newcolor;
      ColorInterface.Reset();
      Redraw();
    }
  }
  // Reset the color scheme on mode change
  else if(Event==cmVideoMode)
    ColorInterface.Reset();

  // Output serialize imformation
  else if(Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CView P%p px=%i py=%i sx=%i sy=%i Value=%i ScaleMode=%i ScaleModeOffset=%i Transparent=%i }\n",this,px,py,sx,sy,Value,ScaleMode,ScaleModeOffset,Transparent);
    
    AddSerializeInfo(Data,info);
  }
    
  return NULL;
}


void* CView::HandleKey(char Char, char Scancode)
{
(void)Char;
(void)Scancode;
  return NULL;
}


void* CView::HandleAccel(char Char, char Scancode)
{
(void)Char;
(void)Scancode;
  return NULL;
}


int CView::IsIn(int x, int y)
{
  return x>=px && x<px+sx && y>=py && y<py+sy;
}


int CView::IsIn(int ax, int ay, int bx, int by)
{
  return px<bx && px+sx>ax &&
         py<by && py+sy>ay;
}


void CView::Draw()
{
}


void CView::Redraw()
{
  if (Owner!=NULL) Owner->Update(this, px, py, sx, sy);
}


void CView::RedrawOwner()
{
  if (Owner!=NULL) Owner->Redraw();
}


void CView::Action()
{
}


int CView::Focus()
{
  if (Focused==this) return 1;
  if (Focused!=NULL) Focused->DeFocus();
  if (Focused==NULL)
  {
    Focused = this;
    return 1;
  } else return 0;
}


int CView::DeFocus()
{
  if (Focused==this)
    Focused = NULL;
  return 1;
}


int CView::CanClose()
{
  return 1;
}


CColor CView::Color(colSystemColor Num)
{
  return ColorInterface.Color(Num);
}


void CView::Move(int x,int y)
{
  MoveTo(px+x, py+y);
}


void CView::Scale(int x, int y)
{
  ScaleTo(sx+x, sy+y);
}


void CView::MoveTo(int x, int y)
{
  px = x;
  py = y;
}


void CView::ScaleTo(int x, int y)
{
  sx = x;
  sy = y;
}


void CView::DefaultScale()
{
  sx = 10;
  sy = 10;
}


void CView::Drag(int mx, int my)
{
  if (Dragged!=NULL) Dragged->Drop();

  Dragged = this;
  Dragging = 1;
  Dragx = mx;
  Dragy = my;
}


void CView::Drop()
{
  if (Dragging)
  {
    Dragged = NULL;
    Dragging = 0;
  }
}


void CView::MakeLocal(int &x, int &y, CView *Origin)
{
  if (Origin!=this && Owner!=NULL)
  {
    Owner->MakeLocal(x, y, Origin);

    x -= Owner->px;
    y -= Owner->py;
  }
}


void CView::MakeGlobal(int &x, int &y, CView *Target)
{
  if (Target!=this && Owner!=NULL)
  {
    Owner->MakeGlobal(x, y, Target);

    x += Owner->px;
    y += Owner->py;
  }
}

// Helper function to find the previous sclRef object to that
// objects can make themselves relative to another object in the group
CView* CView::FindPrevsclRef ()
{
  CView *Act = Prev;

  while (Act!=NULL)
  {
    if (Act->ScaleMode & sclRef)
      return Act;

    Act = Act->Prev;
  }

  return NULL;
}


void CView::Adjust(int dx, int dy)
{
  int npx = px;
  int npy = py;
  int nsx = sx;
  int nsy = sy;

  // Relative to last object positioning flags
  if ((Prev!=NULL) && (!(ScaleMode & sclRefRelative)))
  {
    if (ScaleMode & sclSameWidth)
    {
      nsx = Prev->sx;
    }

    if (ScaleMode & sclSameHeight)
    {
      nsy = Prev->sy;
    }
    
    if (ScaleMode & sclXOnLeft)
    {
      npx = Prev->px - ScaleModeOffset - nsx;
    }

    if (ScaleMode & sclXOnRight)
    {
      npx = Prev->px + Prev->sx + ScaleModeOffset;
    }

    if (ScaleMode & sclYOnTop)
    {
      npy = Prev->py - ScaleModeOffset - nsy;
    }

    if (ScaleMode & sclYOnBottom)
    {
      npy = Prev->py + Prev->sy + ScaleModeOffset;
    }

    // Relative to last object sizing/positioning flags
    if (ScaleMode & sclSameX)
    {
      npx = Prev->px;
    }

    if (ScaleMode & sclSameY)
    {
      npy = Prev->py;
    }
  }

  // Relative to last sclRef object positioning flags
  if (ScaleMode & sclRefRelative)
  {
    // Make sure that there is a previous reference to make us relative to
    CView* Ref = FindPrevsclRef();
    if (Ref != NULL)
    {
      if (ScaleMode & sclSameWidth)
      {
        nsx = Ref->sx;
      }

      if (ScaleMode & sclSameHeight)
      {
        nsy = Ref->sy;
      }
      
      if (ScaleMode & sclXOnLeft)
      {
        npx = Ref->px - ScaleModeOffset - nsx;
      }

      if (ScaleMode & sclXOnRight)
      {
        npx = Ref->px + Ref->sx + ScaleModeOffset;
      }

      if (ScaleMode & sclYOnTop)
      {
        npy = Ref->py - ScaleModeOffset - nsy;
      }

      if (ScaleMode & sclYOnBottom)
      {
        npy = Ref->py + Ref->sy + ScaleModeOffset;
      }

      // Relative to last sclRef object sizing/positioning flags
      if (ScaleMode & sclSameX)
      {
        npx = Ref->px;
      }

      if (ScaleMode & sclSameY)
      {
        npy = Ref->py;
      }
    }
  }

  // Normal resizing flags
  if (ScaleMode & sclLeft)
  {
    nsx -= dx;
    npx += dx;
  }

  if (ScaleMode & sclRight)
  {
    nsx += dx;
  }

  if (ScaleMode & sclTop)
  {
    nsy -= dy;
    npy += dy;
  }

  if (ScaleMode & sclBottom)
  {
    nsy += dy;
  }

  // Actually do the resizing
  MoveTo(npx, npy);
  ScaleTo(nsx, nsy);
}


void CView::PrepareDraw()
{
}


void CView::SetScaleMode(sclScaling aScaleMode, int aOffset)
{
  ScaleMode = aScaleMode;
  ScaleModeOffset = aOffset;

  // Make sure that the object gets it's correct size/pos
  Adjust(0,0);
}


BITMAP *CView::GetMouse(int &rx, int &ry)
{
  rx = Mouserx;
  ry = Mousery;
  return Mouse;
}


void CView::SetMouse(BITMAP *aMouse, int rx, int ry)
{
  Mouse = aMouse;
  Mouserx = rx;
  Mousery = ry;
}


void CView::Kill()
{
  SentencedToDeath=1;
}


void CView::CreateBuffer()
{
}


void CView::DestroyBuffer()
{
}


//======================================================


CBufView::CBufView(int apx, int apy, int asx, int asy)
 :CView(apx, apy, asx, asy),
 RedrawNextTime(1),
 Buffer(NULL),
#ifdef DEFAULTBUFFEROBJECTS
 BufferType(BufferFull)
#else
 BufferType(BufferSub)
#endif
{
}


CBufView::~CBufView()
{
  DestroyBuffer();
}


void CBufView::Draw(int ax, int ay, int asx, int asy)
{
  if (RedrawNextTime)
  {
    RedrawNextTime = 0;
  }

  if (Owner!=NULL)
  {
    if (BufferType==BufferFull)
    {
      blit(GetBuffer(), Owner->GetBuffer(), ax-px, ay-py, ax, ay, asx, asy);
    }
  }
}


void CBufView::Redraw()
{
  RedrawNextTime = 1;
  if (Owner!=NULL) Owner->Update(this, px, py, sx, sy);
}


void CBufView::MoveTo(int x, int y)
{
  if (BufferType==BufferSub) // Sub bitmaps with negative x and y are not allowed
  {
    if (x<0) x=0;
    if (y<0) y=0;
  }

  CView::MoveTo(x, y);

  if (BufferType==BufferSub)
  {
    DestroyBuffer();
    RedrawNextTime = 1;
  }
}

void CBufView::ScaleTo(int x, int y)
{
  CView::ScaleTo(x, y);

  DestroyBuffer();
  RedrawNextTime = 1;
}


void* CBufView::HandleEvent(cmCommand Event, void* Data)
{
  // Update the internal bitmap
  if(Event==cmVideoMode)
    DestroyBuffer();

  // Output serialize imformation
  else if(Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CBufView P%p BufferType=%i }\n",this,BufferType);
    AddSerializeInfo(Data,info);
  }
    
  return CView::HandleEvent(Event,Data);
}


void CBufView::CreateBuffer()
{
  if (BufferType==BufferExternal)
    return;

  DestroyBuffer();

  if (sx>0 && sy>0)
  {
    if ((Owner) && (BufferType==BufferSub))
    {
      Buffer = Cgfx::CreateBitmap(Owner->GetBuffer(), px, py, sx, sy);

      if (!Buffer)
      {
        Buffer = Cgfx::CreateBitmap(sx, sy);
        clear(Buffer);
      }
    }
    else
    {
      Buffer = Cgfx::CreateBitmap(sx, sy);
      clear(Buffer);
    }
  }
  else
  {
    Buffer = Cgfx::CreateBitmap(1, 1);
    clear(Buffer);
  }

  RedrawNextTime = 1;
}

void CBufView::DestroyBuffer()
{
  if (BufferType!=BufferExternal)
  {
    Cgfx::DestroyBitmap(Buffer);
    Buffer = NULL;
  }
}


void CBufView::SetBuffer(BITMAP *Buf)
{
  Cgfx::DestroyBitmap(Buffer);
  Buffer=Buf;

  BufferType = BufferExternal;
}


void CBufView::SetBuffer(BUFFER_T B)
{
  DestroyBuffer();
  BufferType=B;
  CreateBuffer();
}


//======================================================


CGroup::CGroup(int apx, int apy, int asx, int asy)
 :CBufView(apx, apy, asx, asy),
  First(NULL),
  Last(NULL),
  OldMouseIn(NULL),
  Adjusting(1),
  Running(0),
  DrawingType(DrawingPainter)
{
}


CGroup::~CGroup()
{
  CView *Act = First;
  while (Act!=NULL)
  {
    CView *Temp = Act->Next;
    delete Act;
    Act = Temp;
  }
}


void CGroup::HandleMouse(int mx, int my, int buttons)
{
  CView *Act = First;
  CView *Highest = NULL;
  while (Act!=NULL)
  {
    CView *Temp = Act->Next;
    if (Act->IsIn(mx-px, my-py)) Highest = Act;
    Act = Temp;
  }

  if (OldMouseIn!=NULL && Highest!=OldMouseIn)
    OldMouseIn->MouseLost(mx-px, my-py, buttons);

  if (Highest!=NULL)
  {
    if (OldMouseIn!=Highest) Highest->MouseGot(mx-px, my-py, buttons);
    Highest->HandleMouse(mx-px, my-py, buttons);
  }

  OldMouseIn = Highest;

  CView::HandleMouse(mx, my, buttons);
}


void CGroup::MouseLost(int mx, int my, int buttons)
{
  if (OldMouseIn!=NULL)
  {
    OldMouseIn->MouseLost(mx-px, my-py, buttons);
    OldMouseIn = NULL;
  }
}


void CGroup::MouseGot(int mx, int my, int buttons)
{
  CView *Act = First;
  CView *Highest = NULL;
  while (Act!=NULL)
  {
    CView *Temp = Act->Next;
    if (Act->IsIn(mx-px, my-py)) Highest = Act;
    Act = Temp;
  }

  if (Highest!=NULL) Highest->MouseGot(mx-px, my-py, buttons);

  OldMouseIn = Highest;
}


void* CGroup::HandleEvent(cmCommand Event, void* Data)
{
  // Output serialize imformation
  if(Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CGroup P%p DrawingType=%i \n",this,DrawingType);
    AddSerializeInfo(Data,info);
  }
    
  // Handle it ourselves first
  void* ret = CBufView::HandleEvent(Event, Data);

  // Otherwise pass it to our sub objects
  if(ret==NULL)
    {
    CView *Act = First;
    while (Act!=NULL)
      {
      CView *Temp = Act->Next;
      ret = Act->HandleEvent(Event, Data);
      if (ret!=NULL)
        return ret;
      Act = Temp;
      }
    }

  if(Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CGroupEnd P%p } }\n",this);
    AddSerializeInfo(Data,info);
  }
    
  return ret;
}


void* CGroup::HandleAccel(char Char, char Scancode)
{
  void *ret;

  // First try to use it ourselves
  ret = CBufView::HandleAccel(Char, Scancode);
  if (ret!=NULL)
    return ret;

  // Otherwise pass it down to our children
  CView *Act = First;
  while (Act!=NULL)
  {
    CView *Temp = Act->Next;

    // Does the child use the Accelerator key?
    ret = Act->HandleAccel(Char, Scancode);

    if (ret!=NULL)
       return ret;

    // Go to the next child
    Act = Temp;
  }
  // No child used the Accelerator key
  return NULL;
}


void CGroup::PrepareDraw()
{
  CView *Act = First;
  while (Act!=NULL)
  {
    CView *Temp = Act->Next;
    Act->PrepareDraw();
    Act = Temp;
  }

  CBufView::PrepareDraw();
}


void CGroup::Draw(int ax, int ay, int asx, int asy)
{
  if (Owner!=NULL)
  {
    if (GetBufferType()==BufferSub)
    {
      if (GetDrawingType()==DrawingPainter)
      {
        CRect ThisRect(0, 0, sx, sy);
        CRect DrawRect(ax-px, ay-py, asx, asy);

        DrawRect = DrawRect & ThisRect;
        if (!DrawRect.Valid()) return;

        CView *Act = First;
        while (Act!=NULL)
        {
          CView *Temp = Act->Next;

          CRect ActRect(Act->px, Act->py, Act->sx, Act->sy);
          CRect Clipped(ActRect & DrawRect);

          if (Clipped.Valid())
          {
            set_clip(GetBuffer(), Clipped.p.x, Clipped.p.y,
              Clipped.p.x+Clipped.s.w-1, Clipped.p.y+Clipped.s.h-1);

            Act->Draw(Clipped.p.x, Clipped.p.y, Clipped.s.w, Clipped.s.h);
          }

          Act = Temp;
        }
      } else
      {
        DrawRect(ax-px, ay-py, ax-px+asx, ay-py+asy);
      }
    } else
    {
      if (RedrawNextTime)
      {
        CView *Act = First;
        while (Act!=NULL)
        {
          CView *Temp = Act->Next;
          Act->Draw(Act->px, Act->py, Act->sx, Act->sy);
          Act = Temp;
        }

        RedrawNextTime = 0;
      }

      blit(GetBuffer(), Owner->GetBuffer(), ax-px, ay-py, ax, ay, asx, asy);
    }
  }
}


void CGroup::DrawRect(int ax, int ay, int bx, int by)
{
  DrawRecursive(ax, ay, bx, by, Last);

  set_clip(GetBuffer(), 0, 0, sx-1, sy-1);
}


extern int ShowRects;


void CGroup::DrawRecursive(int ax, int ay, int bx, int by, CView* v)
{
  if (ax>=bx || ay>=by) return;

  while (v!=NULL)
  {
    if (v->IsIn(ax, ay, bx, by))
    {
      int vax=v->Posx();
      int vay=v->Posy();
      int vbx=vax+v->Width();
      int vby=vay+v->Height();

      if (vax<ax) vax=ax;
      if (vay<ay) vay=ay;
      if (vbx>bx) vbx=bx;
      if (vby>by) vby=by;

	  set_clip(GetBuffer(), vax, vay, vbx-1, vby-1); // CView's don't clip until now
      
      if (v->IsTransparent())
        DrawRecursive(vax, vay, vbx, vby, v->Prev);

	  set_clip(GetBuffer(), vax, vay, vbx-1, vby-1); // CView's don't clip until now	 
      v->Draw(vax, vay, vbx-vax, vby-vay);

      if (ShowRects) rect(GetBuffer(), ax, ay, bx-1, by-1, ax ^ ay ^ bx ^ by);

      if (vay>ay) DrawRecursive(ax, ay,  bx, vay, v->Prev); /* top    */
      if (vax>ax) DrawRecursive(ax, vay, vax,vby, v->Prev); /* left   */
      if (vbx<bx) DrawRecursive(vbx,vay, bx, vby, v->Prev); /* right  */
      if (vby<by) DrawRecursive(ax, vby, bx, by,  v->Prev); /* bottom */

      return;
    }

    v = v->Prev;
  }
}


void CGroup::Update(CView *aClient, int ax, int ay, int asx, int asy)
{
  if (aClient!=NULL)
  {
    if (GetBufferType()==BufferFull)
      aClient->Draw(ax, ay, asx, asy);
    if (Owner!=NULL)
      Owner->Update(this, px+ax, py+ay, asx, asy);
  } else
  {
    Redraw();
  }

/*  Draw(ax+px, ay+py, asx, asy);
  if (Owner!=NULL) Owner->Update(this, px+ax, py+ay, asx, asy);*/
}


void CGroup::Enclose(int border)
{
  int mx = 0;
  int my = 0;

  int x, y;

  CView *Act = First;
  while (Act!=NULL)
  {
    // Adjust the object's size and find it's extent
    Act->Adjust(0,0);

    x = Act->px + Act->sx;
    y = Act->py + Act->sy;

    if(x>mx) mx = x;
    if(y>my) my = y;

    // Do the nect object
    Act = Act->Next;
  }

  // Rescale ourselves with a border
  ScaleTo(mx+border,my+border);
}


void CGroup::Action()
{
  CView *Act = First;
  while (Act!=NULL)
  {
    CView *Temp = Act->Next;
    if (Act->SentencedToDeath) delete Act;
      else Act->Action();
    Act = Temp;
  }

  CView::Action();
}


void CGroup::Insert(CView *View)
{
  if (Last!=NULL)
  {
    Last->Next = View;
    View->Prev = Last;
    Last = View;
  } else
  {
    View->Prev = NULL;
    Last = View;
    First = View;
  }

  View->Next = NULL;
  View->Owner = this;

  RedrawNextTime = 1;

  // Make sure that the object gets it's correct size/pos
  View->Adjust(0,0);
}


void CGroup::Remove(CView *View)
{
  if (View->Prev!=NULL)
  {
    View->Prev->Next = View->Next;
    if (Last==View) Last = View->Prev;
  } else
  {
    First = View->Next;
    if (Last==View) Last=NULL;
  }

  if (View->Next!=NULL)
  {
    View->Next->Prev = View->Prev;
  }

  if (OldMouseIn==View) OldMouseIn=NULL;

  View->Next = NULL;
  View->Prev = NULL;
  View->Owner = NULL;

  RedrawNextTime = 1;

  // Make sure that the object gets it's correct size/pos
//  View->Adjust(0,0);
}


void CGroup::Front(CView *View)
{
  if (View!=NULL)
  {
    Remove(View);
    Insert(View);
    View->Redraw();

    RedrawNextTime = 0;
  }
}


void CGroup::Back(CView *View)
{
  if (View!=NULL && View->Owner==this)
  {
    // First remove it from wherever it was
    Remove(View);

    // Now place it at the background
    View->Next = First;
    View->Prev = NULL;

    if(First!=NULL)
      First->Prev = View;

    First = View;

    if(Last==NULL)
      Last = View;
      
    RedrawNextTime = 0;
  }
}


void CGroup::ScaleTo(int x, int y)
{
  int osx = sx;
  int osy = sy;

  CBufView::ScaleTo(x,y);

  if (Adjusting)
  {
    int dx = x-osx;
    int dy = y-osy;

    CView *Act = First;
    while (Act!=NULL)
    {
      CView *Temp = Act->Next;
      Act->Adjust(dx, dy);
      Act = Temp;
    }
  }
}


BITMAP *CGroup::GetMouse(int &rx, int &ry)
{
  if (OldMouseIn!=NULL)
  {
    BITMAP *m = OldMouseIn->GetMouse(rx, ry);
    if (m!=NULL) return m;
      else return CView::GetMouse(rx, ry);
  } else return CView::GetMouse(rx, ry);
}


int CGroup::CanClose()
{
  int r=CBufView::CanClose();

  CView *Act = First;
  while (Act!=NULL)
  {
    CView *Temp = Act->Next;
    r &= Act->CanClose();
    Act = Temp;
  }

  return r;
}


void CGroup::CreateBuffer()
{
  DestroyBuffer();

  CBufView::CreateBuffer();

  CView *Act = First;
  while (Act!=NULL)
  {
    CView *Temp = Act->Next;
    Act->CreateBuffer();
    Act = Temp;
  }
}


void CGroup::DestroyBuffer()
{
  CView *Act = First;
  while (Act!=NULL)
  {
    CView *Temp = Act->Next;
    Act->DestroyBuffer();
    Act = Temp;
  }
  CBufView::DestroyBuffer();
}


//===========================================================


CWindow::CWindow(int apx, int apy, int asx, int asy)
 : CGroup(apx, apy, asx, asy)
{
#ifdef BUFFEREDWINDOWS
  SetBuffer(BufferFull);
#endif

#ifdef RECURSIVEWINDOWS
  SetDrawing(DrawingRecursive);
#endif
}


CWindow::~CWindow()
{
}


void* CWindow::HandleEvent(cmCommand Event, void* Data)
{
//  int interm = (int)Data;
  cmCommand command = cmCommand((int)Data);
  int D = (int)Data;
                
  switch (Event)
  {
   case cmWindowBufferMode:
                SetBuffer(*((CBufView::BUFFER_T*)Data));
                break;
   case cmStop:
                Stop(command);
                break;
   case cmDrag:
                Drag(D & 0xffff, D >> 16);
                break;
   case cmEscape:
   case cmClose: if (Running)
                 {
                   Stop(cmCancel);
                 } else
                 {
                   if (CanClose())
                   {
                     if (Owner!=NULL)
                     {
                       Kill();
                     }
                   }
                 }
                 break;
   case cmSerialize:
   {
     char info[1000];
     sprintf(info,"{ CWindow P%p }\n",this);
     AddSerializeInfo(Data,info);
     break;
   }
    
  default: { };
  }

  return CGroup::HandleEvent(Event, Data);
}


void* CWindow::HandleKey(char Char, char Scancode)
{
//  CView* oldFocused = Focused;

  // Check for a TAB to switch the focused object
  if (Scancode==KEY_TAB)
  {
    if (Focused!=NULL)
      Focused = Focused->Next;
      
    if (Focused==NULL)
      Focused = First;

    return this;
  }
  return NULL;
}


void CWindow::RedrawOwner()
{
}


extern int Counter;
CView *ActionView=NULL;

extern int ClickCounter;
int Clickx, Clicky;
int LastClick;

// Macro related variables
CMacro Macro;
bool MacroRecording = false;
bool MacroPlayback = false;

cmCommand CWindow::Run(int aForeground)
{
  Running = 1;
  Value = cmNone;
  Draw(px, py, sx, sy);
  ClickCounter=0;
  LastClick=0;

  if (Focused!=NULL && Focused!=this)
    Focused->DeFocus();

  CView *ActActionView  = ActionView;

  if (aForeground)
  {
    ActActionView = this;
  } else
  {
    if (ActActionView==NULL) ActActionView=Program;
  }

  while (Running)
  {
    Bill = this;

    Program->Idle();

    // Drawing
    if (MacroPlayback==false)
    {
      Program->PrepareDraw();
      #ifdef VSYNC
      vsync();
      #endif
      Program->Draw();
    }

    #ifdef SYNC
    if (DebugMode!=0) Counter=1;

    while (Running && Counter>0)
    {
    #endif
      Bill = this;

      // Get the state of the keys and mouse
      // Make it centralized so that we can handle user
      // input macros in a clean manner
      int Ascii, Scancode, mx, my, but;

      // Get the input from a macro
      if (MacroPlayback==true)
      {
        bool more = Macro.Playback(Ascii,Scancode,mx,my,but);
        View_UpdateMouse(mx,my);

        if (more==false)
        {
          MacroPlayback = false;
        }
      }
      // Get the input from the user
      if (MacroPlayback==false)
         GetUserInput(Ascii,Scancode,mx,my,but);

      // Possibly record the user input into the macro
      if (MacroRecording==true)
      {
        Macro.Append(Ascii,Scancode,mx,my,but);
      }

      // Keyboard handling
      if (Ascii || Scancode)
      {
        // Check to see if we start reading in a macro
        if (Scancode==KEY_F1 && MacroPlayback==false)
        {
          // If we are recording, then stop recording
          // otherwise start recording
          if (MacroRecording==true)
          {
            MacroRecording = false;
          }
          else
          {
            Macro.Reset();
            MacroRecording = true;
          }
        }

        // Check to see if we start reading in a macro
        if (Scancode==KEY_F2 && MacroRecording==false && MacroPlayback==false)
        {
          MacroPlayback = true;
        }

        // Try to use the key first ourselves
        void* keyuser = HandleKey(Ascii, Scancode);
 
        // Possibly send the key to the focused object
        if (keyuser==NULL && Focused!=NULL)
          keyuser = Focused->HandleKey(Ascii, Scancode);

        // Try to send it as an accelerator key
        if (keyuser==NULL)
          keyuser = HandleAccel(Ascii, Scancode);
      }

      // Mouse handling
      int rx, ry;
      BITMAP *m = GetMouse(rx, ry);
      if (m!=NULL)
        View_SetMouse(m, rx, ry);
      else
        if (MacroRecording==true)
          View_SetMouse(bmpMouseMacroRecord);
        else
          View_SetMouse(DefaultMouse);

      MakeLocal(mx, my, Program);

      if (mx!=Oldmx || my!=Oldmy || but!=Oldbuttons)
      {
        // Double click
        if (ClickCounter>=View_GetDblClickTime())
        {
          LastClick = 0;
          ClickCounter = 0;
        }

        if (Oldbuttons!=0 && but==0)
        {
          if (LastClick!=Oldbuttons) LastClick = 0;

          if (LastClick==0)
          {
            LastClick = Oldbuttons;
            ClickCounter = 0;
            Clickx = mx;
            Clicky = my;
          } else
          {
            if (abs(mx-Clickx)<=View_GetDblClickDist() &&
                abs(my-Clicky)<=View_GetDblClickDist())
            {
              but = Oldbuttons | butDoubleClick;
            }
            LastClick = 0;
          }
        }

        // Single click
        if (Dragged!=NULL)
        {
          Dragged->MakeLocal(mx, my, this);
          Dragged->HandleMouse(mx-Dragx, my-Dragy, but);
        } else HandleMouse(mx, my, but);
      }

      // Action
      Program->Idle();
      ActionView = ActActionView;
      ActionView->Action();

      // update Counter
      Counter--;

    #ifdef SYNC
      if (DebugMode!=0) break;
    }
    #endif

  }

  Bill = NULL;
  ActionView = NULL;

  // Return the mouse pointer if we have to
  if (MacroRecording==true)
    View_SetMouse(bmpMouse);

  return Value;
}


int CWindow::Stop(cmCommand aValue)
{
  if (Running)
  {
    Value = aValue;
    Running = 0;
  }
  return 1;
}


void CWindow::MoveTo(int x, int y)
{
  int ox = px;
  int oy = py;

  CGroup::MoveTo(x, y);

  if (Owner!=NULL && (ox!=px || oy!=py))
  {
    if (px<ox+sx && px+sx>ox &&
        py<oy+sy && py+sy>oy)
    {
      Owner->Update(min(px, ox), min(py, oy), (px<ox)?ox+sx-px:px+sx-ox, (py<oy)?oy+sy-py:py+sy-oy);
    } else
    {
      Owner->Update(px, py, sx, sy);
      Owner->Update(ox, oy, sx, sy);
    }
  }
}


void CWindow::ScaleTo(int x, int y)
{
  int ox = sx;
  int oy = sy;

  CGroup::ScaleTo(x, y);

  if (Owner!=NULL && (ox!=sx || oy!=sy))
  {
    Owner->Update(px, py, max(sx, ox), max(sy, oy));
  }
}


void CWindow::ToFront()
{
  if (Owner!=NULL)
  {
    CGroup *MyOwner = Owner;
    MyOwner->Front(this);
  }
}


// Get the user input
void CWindow::GetUserInput(int& Ascii, int& Scancode, int& mx, int& my, int& but)
{
  if(keypressed())
  {
    int Key = readkey();
    Ascii = Key & 0xff;
    Scancode = Key >> 8;
  }
  else
  {
    Ascii = 0;
    Scancode = 0;
  }

  mx = mouse_x;
  my = mouse_y;
  but = mouse_b;
}


