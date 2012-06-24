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

 v_prog.cc - Application main class

*/


#include <stdio.h>
#include "dpmi.h"
#include "v_prog.h"
#include "v_common.h"
#include "v_mouse.h"
#include "v_gfx.h"
#include "v_system.h"
#include "v_save.h"

#include <allegro.h>


#define DEFAULTMAXDIRT 16

#define RECURSIVEDRAW
#define DIRTYRECTANGLE
//#define SHOWRECTS


//===============================================


volatile int FramesCounter=0;
volatile int Fps=0;
volatile int FpsCount=0;
volatile int Timer1=0;
volatile int Timer2=0;
volatile int Timer3=0;
volatile int ClickCounter=0;

volatile int Counter=0;
volatile int SyncSpeed=35; // 1000 Hz

int ShowRects=0;

void TimerProc(...)
{
  Timer1++;
  Timer2++;
  Timer3++;

  if (Timer1>=1000/SyncSpeed)
  {
    Counter++;
    ClickCounter++;
    Timer1-=1000/SyncSpeed;
  }

  FpsCount++;
  if (FpsCount>=1000)
  {
    Fps=FramesCounter;
    FramesCounter=0;
    FpsCount=0;
  }
}

END_OF_FUNCTION(TimerProc)


int Timed=0;


void StartTimer()
{
  Timer2=0;
}


int StopTimer()
{
  Timed=Timer2;
  return Timed;
}


void View_SwitchRects(int aOn)
{
  ShowRects = aOn;
}


void View_SetSyncSpeed(int aH)
{
  SyncSpeed = aH;
}


//=================================================================


const bool CProgram::double_buffer;


struct CDirtRect
{
  int ax, ay, bx, by;
};


CProgram::CProgram()
 : CWindow(0, 0, Cgfx::Width(), Cgfx::Height())
{
  Program = this;

  LOCK_FUNCTION(TimerProc);
  LOCK_VARIABLE(Fps);
  LOCK_VARIABLE(Counter);
  LOCK_VARIABLE(FramesCounter);
  LOCK_VARIABLE(FpsCount);
  LOCK_VARIABLE(Timer1);
  LOCK_VARIABLE(Timer2);
  LOCK_VARIABLE(Timer3);
  LOCK_VARIABLE(SyncSpeed);

  Counter = 0;
  FpsCount = 0;
  FramesCounter = 0;

  #ifdef DIRTYRECTANGLE
  Dirt = NULL;
  SetMaxDirtyRects(DEFAULTMAXDIRT);
  Dirty = 0;
  DirtAllowed = 1;
  #endif

  install_int(TimerProc, 1);

  if(double_buffer)
    SetBuffer(BufferFull);
  else
    SetBuffer(screen);
}


CProgram::~CProgram()
{
  remove_int(TimerProc);

  #ifdef DIRTYRECTANGLE
  delete[] Dirt;
  #endif

  if (Program==this) Program = NULL;
}


void CProgram::SetMaxDirtyRects(int aNum)
{
  if (Dirt) delete[] Dirt;

  MaxDirtyRects = aNum;
  Dirt = new CDirtRect[MaxDirtyRects];
  RedrawNextTime=1;
}


void* CProgram::HandleKey(char Char, char Scancode)
{
  // for debugging
  if (DebugMode)
  {
    // Alt-ESC -> quit the program
    if (Char==0 && Scancode==KEY_ESC)
    {
      Stop();
      return this;
    }
  }
  return NULL;
}


void* CProgram::HandleEvent(cmCommand Event, void* Data=0)
{
  switch (Event)
  {
   case cmQuit: Stop();
                return this;

   case cmVideoMode:
   {
     if(double_buffer)
       SetBuffer(BufferFull);
     else
       SetBuffer(screen);
   
     ScaleTo(Cgfx::Width(), Cgfx::Height());
     break;
   }
   case cmSerialize:
   {
     char info[1000];
     sprintf(info,"{ CProgram P%p }\n",this);
     AddSerializeInfo(Data,info);
     break;
   }
                
   default: { };
  }
  return CWindow::HandleEvent(Event, Data);
}


void CProgram::Insert(CView *View)
{
  CView *Old = Last;
  CWindow::Insert(View);
  if (Old!=Last)
  {
    if (Old) Old->HandleEvent(cmNotTopWindow);
    if (Last) Last->HandleEvent(cmTopWindow);
  }
}


void CProgram::Remove(CView *View)
{
  CView *Old = Last;
  CWindow::Remove(View);
  if (Old!=Last)
  {
    if (Old) Old->HandleEvent(cmNotTopWindow);
    if (Last) Last->HandleEvent(cmTopWindow);
  }
}


void CProgram::Front(CView *View)
{
  CView *Old = Last;
  CWindow::Front(View);
  if (Old!=Last)
  {
    if (Old) Old->HandleEvent(cmNotTopWindow);
    if (Last) Last->HandleEvent(cmTopWindow);
  }
}


long FreeMem()
{
  __dpmi_free_mem_info info;
  __dpmi_get_free_memory_information(&info);
  return info.total_number_of_free_pages;
}


void CProgram::Idle()
{
}


void CProgram::CreateBuffer()
{
  if(double_buffer)
    CBufView::CreateBuffer();
  else
    SetBuffer(screen);
}


void CProgram::Draw()
{
  View_UpdateMouse();
  
  #ifdef DIRTYRECTANGLE
  View_BlitMouse(GetBuffer());

  int Temp = Dirty;

  if (RedrawNextTime)
  {
  #endif

    #ifdef RECURSIVEDRAW
    DrawRect(px, py, sx, sy);
    #else
    View_RestoreMouse(GetBuffer());
    // TODO: call draw for every window
    View_ShowMouse(GetBuffer());
    #endif

  #ifdef DIRTYRECTANGLE
  } else
  {
    if (Dirty>0) DrawDirt();
  }

  Dirty = 0;
  RedrawNextTime = 0;

  #endif


  FramesCounter++;
  if (DebugMode)
  {
    char s[40];
    sprintf(s, "Fps:%u Dirty:%u Mem:%li T:%u", Fps, Temp, FreeMem(), Timed);
    Cgfx::Rectfill(GetBuffer(), sx-250, sy-SystemFont.Height(), sx, sy, Color(colBack));
    Cgfx::Text(GetBuffer(), SystemFont, s, sx-250, sy-SystemFont.Height(), Color(colFore));
  }

  #ifndef DIRTYRECTANGLE
  #ifndef RECURSIVEDRAW
  View_ShowMouse(GetBuffer());
  #endif
  #endif

  if(double_buffer)
    blit(GetBuffer(), screen, px, py, 0, 0, sx, sy);

  #ifndef DIRTYRECTANGLE
  #ifndef RECURSIVEDRAW
  View_RestoreMouse(GetBuffer());
  #endif
  #endif
}


void CProgram::DrawDirt()
{
  DirtAllowed = 0;

  for (int n=0; n<Dirty; n++)
  {
    DrawRect(Dirt[n].ax, Dirt[n].ay, Dirt[n].bx, Dirt[n].by);
  }

  DirtAllowed = 1;
  Dirty = 0;
}


void CProgram::DrawRect(int ax, int ay, int bx, int by)
{
  int mx, my, msx, msy;
  View_GetMouseRect(mx, my, msx, msy);

  int vax=mx;
  int vay=my;
  int vbx=mx+msx;
  int vby=my+msy;

  if (vax<bx && vay<by && vbx>ax && vby>ay)
  {
    if (vax<ax) vax=ax;
    if (vay<ax) vay=ay;
    if (vbx>bx) vbx=bx;
    if (vby>by) vby=by;

    if (vay>ay) DrawRecursive(ax,  ay,  bx,  vay, Last); // top
    if (vax>ax) DrawRecursive(ax,  vay, vax, vby, Last); // left

    set_clip(GetBuffer(), px, py, sx-1, sy-1);
    View_RestoreMouse(GetBuffer());

    DrawRecursive(vax, vay, vbx, vby, Last);

    set_clip(GetBuffer(), px, py, sx-1, sy-1);
    View_ShowMouse(GetBuffer());

    if (vbx<bx) DrawRecursive(vbx, vay, bx,  vby, Last); // right
    if (vby<by) DrawRecursive(ax,  vby, bx,  by,  Last); // bottom
  } else
  {
    DrawRecursive(ax, ay, bx, by, Last);
  }

  set_clip(GetBuffer(), 0, 0, sx-1, sy-1);
}


void CProgram::Update(CView *aClient, int ax, int ay, int asx, int asy)
{
  #ifdef DIRTYRECTANGLE
  (void)aClient;

  if (ax>sx || ay>sy || ax+asx<0 || ay+asy<0 || RedrawNextTime) return;

  if (!DirtAllowed) return;
  if (Dirty<MaxDirtyRects)
  {
    Dirt[Dirty].ax = ax;
    Dirt[Dirty].ay = ay;
    Dirt[Dirty].bx = ax+asx;
    Dirt[Dirty].by = ay+asy;
    Dirty++;
  } else
  {
    RedrawNextTime = 1;
  }
  #endif
}
