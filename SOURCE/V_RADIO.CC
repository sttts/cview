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

 v_radio.cc - Radio button

*/


#include <string.h>
#include <ctype.h>
#include "v_radio.h"
#include "v_common.h"
#include "allegttf.h"
#include "v_sysbit.h"
#include "v_system.h"
#include "v_save.h"


//=====================================================


CRadioButton::CRadioButton(int apx, int apy, char * aText, int aStatus)
 : CView(apx, apy, 10, 10),
 Status(aStatus),
 Text(strdup(aText))
{
  if (Text!=NULL) SetTransparent(1);
}


CRadioButton::~CRadioButton()
{
  free(Text);
}


void CRadioButton::HandleMouse(int mx, int my, int buttons)
{
  if (Oldbuttons==1 && buttons==0) SetStatus(1);

  CView::HandleMouse(mx, my, buttons);
}


void* CRadioButton::HandleAccel(char aChar, char Scancode)
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
    SetStatus(1);
    return this;
  }
  return NULL;
}


void CRadioButton::SetStatus(int aStatus)
{
  Status = aStatus;
  if (Status && Owner!=NULL) Owner->HandleEvent(cmRadioSelect, this);
  Redraw();
}


void* CRadioButton::HandleEvent(cmCommand Event, void* Data)
{
  switch (Event)
  {
   case cmRadioSelect: if ((CView*)Data!=this) SetStatus(0);
                       break;

   case cmSerialize:
   {
     char info[1000];
     sprintf(info,"{ CRadioButton P%p Text=\"%s\" Status=%i }\n",this,Text,Status);
     AddSerializeInfo(Data,info);
     break;
   }
                       
   default: { };
  }

  return CView::HandleEvent(Event, Data);
}


void CRadioButton::Draw()
{
  int width = SystemBitmap[bmpRadioOn]->w;

  if (Status)
  {
    if (Owner!=NULL)
    {
      draw_sprite(Owner->GetBuffer(),SystemBitmap[bmpRadioOn], px, py);
      if (Text!=NULL)
        ViewTextout(Owner->GetBuffer(), SystemFont, Text, px + width + 2, py, Color(colFore));
    }
  } else
  {
    if (Owner!=NULL)
    {
      draw_sprite(Owner->GetBuffer(),SystemBitmap[bmpRadioOff], px, py);
      if (Text!=NULL)
        ViewTextout(Owner->GetBuffer(), SystemFont, Text, px + width + 2, py, Color(colFore));
    }
  }

  CView::Draw();
}


void CRadioButton::DefaultScale()
{
  int width = SystemBitmap[bmpRadioOn]->w;

  sy = SystemFont.Height()+2;
  sx = ViewTextLength(SystemFont,Text)+width+2;
}



