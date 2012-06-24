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

 v_check.cc - Checkbox object

*/


#include <string.h>
#include <ctype.h>
#include "v_check.h"
#include "v_common.h"
#include "v_sysbit.h"
#include "v_system.h"
#include "v_save.h"

#include <allegro.h>


//=====================================================


CCheckbox::CCheckbox(int apx, int apy, char *aText, int aStatus)
 : CView(apx, apy, 10, 10),
 Status(aStatus),
 Text(strdup(aText))
{
  if (Text!=NULL) SetTransparent(1);
}


CCheckbox::~CCheckbox()
{
  free(Text);
}


void* CCheckbox::HandleEvent(cmCommand Event, void* Data)
{
  if(Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CCheckbox P%p Status=%i }\n",this,Status);
    AddSerializeInfo(Data,info);
  }
  return CView::HandleEvent(Event, Data);
}


void CCheckbox::HandleMouse(int mx, int my, int buttons)
{
  if (Oldbuttons==1 && buttons==0) SetStatus(!Status);

  CView::HandleMouse(mx, my, buttons);
}


void* CCheckbox::HandleAccel(char aChar, char Scancode)
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
    SetStatus(!Status);
    return this;
  }
  return NULL;
}


void CCheckbox::SetStatus(int aStatus)
{
  Status = aStatus;

  if (Owner!=NULL) Owner->HandleEvent(cmCheckSelect, this);

  Redraw();
}


void CCheckbox::Draw()
{
//  FONT *Font = View_GetFont(small7);
  int width = SystemBitmap[bmpCheckOn]->w;

  if (Status)
  {
    if (Owner!=NULL)
    {
      draw_sprite(Owner->GetBuffer(),SystemBitmap[bmpCheckOn], px, py);
      if (Text!=NULL)
        ViewTextout(Owner->GetBuffer(), SystemFont, Text, px + width + 2, py, Color(colFore));
    }
  } else
  {
    if (Owner!=NULL)
    {
      draw_sprite(Owner->GetBuffer(),SystemBitmap[bmpCheckOff], px, py);
      if (Text!=NULL)
        ViewTextout(Owner->GetBuffer(), SystemFont, Text, px + width + 2, py, Color(colFore));
    }
  }

  CView::Draw();
}


void CCheckbox::DefaultScale()
{
  int width = SystemBitmap[bmpCheckOn]->w;

  sy = SystemFont.Height()+2;
  sx = ViewTextLength(SystemFont,Text)+width+2;
}




