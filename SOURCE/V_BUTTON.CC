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

 v_button.cc - Different button classes

*/


//#include "view.h"
#include <ctype.h>
#include <string.h>
#include "v_button.h"
#include "v_common.h"
#include "v_system.h"
#include "v_save.h"

#include <allegro.h>


// Helper button operations
btnFlags operator|(const btnFlags one, const btnFlags two)
{
  return (btnFlags)((int)one | (int)two);
}


//=======================================================


CButton::CButton(int apx, int apy, int asx,  int asy,
                 cmCommand aCommand, CView *aTarget, btnFlags aFlags)
 : CView(apx, apy, asx, asy)
{
  Command = aCommand;
  Target = aTarget;
  Flags = aFlags;
  Clicked = 0;

  if (Flags & btnRadio) Flags|=btnSwitch;
}


CButton::~CButton()
{
}


void CButton::HandleMouse(int mx, int my, int buttons)
{
  if (!Clicked && buttons==butLeft)
  {
    Clicked = 1;
    Redraw();
  } else
  if (Clicked && Oldbuttons==butLeft && buttons!=butLeft)
  {
    if (!(Flags & btnSwitch))
    {
      Clicked = 0;
      Redraw();
    }

    if (!(Flags & btnMultiEvent))
    {
      if (Target!=NULL)
        Target->HandleEvent(Command);
      else
        Owner->HandleEvent(Command);
    }

    if (Flags & btnRadio)
    {
      if (Owner!=NULL) Owner->HandleEvent(cmButtonSelect, this);
    }
  }

  CView::HandleMouse(mx, my, buttons);
}


void* CButton::HandleEvent(cmCommand Event, void* Data)
{
  switch (Event)
  {
    case cmButtonSelect: if ((CView*)Data!=this && (Flags & btnRadio))
                         {
                           Clicked = 0;
                           Redraw();
                         }
                         break;
    // Output serialize imformation
    case cmSerialize:
    {
      char info[1000];
      sprintf(info,"{ CButton P%p Flags=%i Command=%i Target=%p }\n",this,Flags,Command,Target);
      AddSerializeInfo(Data,info);
      break;
    }
    
    default: { };
  }

  return CView::HandleEvent(Event, Data);
}


void CButton::MouseLost(int mx, int my, int buttons)
{
  (void)mx;
  (void)my;
  (void)buttons;

  if (Clicked && !(Flags & btnSwitch))
  {
    Clicked = 0;
    Redraw();
  }
}


void CButton::SetState(int aClicked)
{
  Clicked = aClicked;
  Redraw();

  if (Clicked && (Flags & btnRadio))
  {
    if (Owner!=NULL) Owner->HandleEvent(cmButtonSelect, this);
  }
}


int CButton::GetState()
{
  return Clicked;
}


void CButton::Action()
{
  if ((Flags & btnMultiEvent) && Clicked)
  {
    if (Target!=NULL)
      Target->HandleEvent(Command);
    else
      Owner->HandleEvent(Command);
  }

  CView::Action();
}


void CButton::Draw()
{
  if (Owner!=NULL)
  {
    BITMAP *Buffer = Owner->GetBuffer();

    CColor Light = Color(colLight);
    CColor Dark = Color(colDark);

    if (Clicked)
    {
      Dark = Color(colLight);
      Light = Color(colDark);
    }

    if (Focused==this)
    {
      button(Buffer, px+1, py+1, sx-2, sy-2, Light, Dark, Color(colBack));
      rect(Buffer, px, py, px+sx-1, py+sy-1, Color(colDark));
    }else
    {
      button(Buffer, px, py, sx, sy, Light, Dark, Color(colBack));
    }
  }

  CView::Draw();
}


//=======================================================


CPicButton::CPicButton(int apx, int apy, int asx,  int asy, BITMAP *aPicture,
                       cmCommand aCommand, CView *aTarget, btnFlags aFlags)
 : CButton(apx, apy, asx, asy, aCommand, aTarget, aFlags)
{
  PictureUp = aPicture;
  PictureDown = aPicture;
}


CPicButton::CPicButton(int apx, int apy, int asx,  int asy,
                       BITMAP *aPicture1, BITMAP *aPicture2,
                       cmCommand aCommand, CView *aTarget, btnFlags aFlags)
 : CButton(apx, apy, asx, asy, aCommand, aTarget, aFlags)
{
  PictureUp = aPicture1;
  PictureDown = aPicture2;
}


CPicButton::~CPicButton()
{
}


void* CPicButton::HandleEvent(cmCommand Event, void* Data)
{
  if(Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CPicButton P%p }\n",this);
    AddSerializeInfo(Data,info);
  }
  return CButton::HandleEvent(Event, Data);
}


void CPicButton::Draw()
{
  BITMAP *Bitmap;

  if (Clicked)
  {
    Bitmap = PictureDown;
  } else
    Bitmap = PictureUp;

  int w = Bitmap->w;
  int h = Bitmap->h;

  int rx = (sx-Bitmap->w)/2;
  int ry = (sy-Bitmap->h)/2;

  if (rx<0)
  {
    rx = 0;
    w = sx;
  }

  if (ry<0)
  {
    ry = 0;
    h = sy;
  }

  if (Owner!=NULL)
  {
    if (Flags & btnTransparent)
    {
      BITMAP *Buffer = Owner->GetBuffer();

      CColor Light = Color(colLight);
      CColor Dark = Color(colDark);

      if (Clicked)
      {
        Dark = Color(colLight);
        Light = Color(colDark);
      }

      if (Focused==this)
      {
        button(Buffer, px+1, py+1, sx-2, sy-2, Light, Dark, Color(colBack));
        rect(Buffer, px, py, px+sx-1, py+sy-1, Color(colDark));
      } else
      {
        button(Buffer, px, py, sx, sy, Light, Dark, Color(colBack));
      }

      draw_sprite(Buffer, Bitmap, px+rx, py+ry);
    } else
    {
      blit(Bitmap, Owner->GetBuffer(), 0, 0, px+rx, py+ry, w, h);
    }
  }

  CView::Draw();
}


void CPicButton::DefaultScale()
{
  sy = PictureUp->h;
  sx = PictureUp->w;
}


//=======================================================


CSystemBitmapButton::CSystemBitmapButton(int apx, int apy, int asx,  int asy, bmpBitmap aPicture,
                       cmCommand aCommand, CView *aTarget, btnFlags aFlags)
 : CButton(apx, apy, asx, asy, aCommand, aTarget, aFlags)
{
  PictureUp = aPicture;
  PictureDown = aPicture;
}


CSystemBitmapButton::CSystemBitmapButton(int apx, int apy, int asx,  int asy,
                       bmpBitmap aPicture1, bmpBitmap aPicture2,
                       cmCommand aCommand, CView *aTarget, btnFlags aFlags)
 : CButton(apx, apy, asx, asy, aCommand, aTarget, aFlags)
{
  PictureUp = aPicture1;
  PictureDown = aPicture2;
}


CSystemBitmapButton::~CSystemBitmapButton()
{
}


void* CSystemBitmapButton::HandleEvent(cmCommand Event, void* Data)
{
  if(Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CSystemBitmapButton P%p PictureUp=%i PictureDown=%i }\n",this,PictureUp,PictureDown);
    AddSerializeInfo(Data,info);
  }
  return CButton::HandleEvent(Event, Data);
}


void CSystemBitmapButton::Draw()
{
  bmpBitmap BitmapIndex;

  if (Clicked)
  {
    BitmapIndex = PictureDown;
  } else
    BitmapIndex = PictureUp;

  BITMAP* Bitmap = SystemBitmap[BitmapIndex];

  int w = Bitmap->w;
  int h = Bitmap->h;

  int rx = (sx-Bitmap->w)/2;
  int ry = (sy-Bitmap->h)/2;

  if (rx<0)
  {
    rx = 0;
    w = sx;
  }

  if (ry<0)
  {
    ry = 0;
    h = sy;
  }

  if (Owner!=NULL)
  {
    if (Flags & btnTransparent)
    {
      BITMAP *Buffer = Owner->GetBuffer();

      CColor Light = Color(colLight);
      CColor Dark = Color(colDark);

      if (Clicked)
      {
        Dark = Color(colLight);
        Light = Color(colDark);
      }

      if (Focused==this)
      {
        button(Buffer, px+1, py+1, sx-2, sy-2, Light, Dark, Color(colBack));
        rect(Buffer, px, py, px+sx-1, py+sy-1, Color(colDark));
      } else
      {
        button(Buffer, px, py, sx, sy, Light, Dark, Color(colBack));
      }

      draw_sprite(Buffer, Bitmap, px+rx, py+ry);
    } else
    {
      blit(Bitmap, Owner->GetBuffer(), 0, 0, px+rx, py+ry, w, h);
    }
  }

  CView::Draw();
}


void CSystemBitmapButton::DefaultScale()
{
  sy = SystemBitmap[PictureUp]->h;
  sx = SystemBitmap[PictureUp]->w;
}


//=======================================================


CTextButton::CTextButton(int apx, int apy, int asx,  int asy, char *aText,
                         cmCommand aCommand, CView *aTarget, btnFlags aFlags)
 : CButton(apx, apy, asx, asy, aCommand, aTarget, aFlags)
{
  Text = strdup(aText);
}


CTextButton::~CTextButton()
{
  free(Text);
}


void* CTextButton::HandleEvent(cmCommand Event, void* Data)
{
  if(Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CTextButton P%p Text=\"%s\" }\n",this,Text);
    AddSerializeInfo(Data,info);
  }
  return CButton::HandleEvent(Event, Data);
}


void CTextButton::Draw()
{
  if (Owner!=NULL)
  {
    BITMAP *Buffer = Owner->GetBuffer();

    CColor Light = Color(colLight);
    CColor Dark = Color(colDark);

    if (Clicked)
    {
      Dark = Color(colLight);
      Light = Color(colDark);
    }

    if (Focused==this)
    {
      button(Buffer, px+1, py+1, sx-2, sy-2, Light, Dark, Color(colBack));
      rect(Buffer, px, py, px+sx-1, py+sy-1, Color(colDark));
    } else
    {
      button(Buffer, px, py, sx, sy, Light, Dark, Color(colBack));
    }

    // Text output
    int ty = SystemFont.Height();

    int rx = (sx-ViewTextLength(SystemFont,Text))/2;
    int ry = (sy-ty)/2;

    if (Clicked)
    {
      rx++;
      ry++;
    }

    text_mode(-1); // transparent
    ViewTextout(Buffer, SystemFont, Text, px+rx, py+ry, Color(colFore));
  }

  CView::Draw();
}


void CTextButton::DefaultScale()
{
  sy = SystemFont.Height()*3/2;
  sx = ViewTextLength(SystemFont,Text)*3/2;
}


void* CTextButton::HandleAccel(char aChar, char Scancode)
{
  // Make sure it was an ALT combnation
  if (aChar!=0)
    return NULL;

  char realChar = SCANCODE_TO_KEY(Scancode);

  // Find what the hotkey is
  char hotkey = ViewAccel(Text);

  // Check if the key is the hotkey
  if (tolower(realChar) == tolower(hotkey))
  {
    if (!(Flags & btnMultiEvent))
    {
      if (Target!=NULL)
        Target->HandleEvent(Command);
      else
        Owner->HandleEvent(Command);
    }
    return this;
  }
  return NULL;
}


//=======================================================


CTransButton::CTransButton(int apx, int apy, int asx,  int asy,
                           cmCommand aCommand, CView *aTarget, btnFlags aFlags)
 : CButton(apx, apy, asx, asy, aCommand, aTarget, aFlags)
{
  SetTransparent(1);
}


CTransButton::~CTransButton()
{
}


void* CTransButton::HandleEvent(cmCommand Event, void* Data)
{
  if(Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CTransButton P%p }\n",this);
    AddSerializeInfo(Data,info);
  }
  return CButton::HandleEvent(Event, Data);
}


void CTransButton::Draw()
{
}

