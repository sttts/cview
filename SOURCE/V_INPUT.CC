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

 v_input.cc - Input line class

*/


#include <string.h>
#include "v_input.h"
#include "v_common.h"
#include "v_gfx.h"
#include "v_system.h"
#include "v_save.h"

#include <allegro.h>


//======================================================


CInputLine::CInputLine(int apx, int apy, int asx, int asy, int aMaxLen,
  char* aInput, char* aKeys, int aFlags)
 : CView(apx, apy, asx, asy)
{
  MaxLen=aMaxLen;
  Input=(char*)malloc(MaxLen+1);
  Flags=aFlags;

  if (aInput!=NULL)
  {
    strncpy(Input, aInput, MaxLen);
    *(Input+MaxLen)=0;
  } else strcpy(Input,"");

  OldInput=strdup(Input);

  if (aKeys!=NULL) Keys=strdup(aKeys); else Keys=NULL;

  Cur=0;
  Pos=0;
  VisChars=0;
}


CInputLine::~CInputLine()
{
  if (Input!=NULL) free(Input);
  if (OldInput!=NULL) free(OldInput);
  if (Keys!=NULL) free(Keys);
}


void* CInputLine::HandleEvent(cmCommand Event, void* Data)
{
  // Output serialize imformation
  if(Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CInputLine P%p Input=\"%s\" MaxLen=%i Flags=%i }\n",this,Input,MaxLen,Flags);
    AddSerializeInfo(Data,info);
  }
    
  return CView::HandleEvent(Event,Data);
}


void CInputLine::HandleMouse(int mx, int my, int buttons)
{
  if (Focused==this)
  {
  } else
  {
    if (Oldbuttons==0 && buttons==1)
    {
      if (Flags & inpEditable) Focus();
    }
  }

  CView::HandleMouse(mx, my, buttons);
}


void* CInputLine::HandleKey(char Char, char Scancode)
{
  switch(Scancode)
  {
    case KEY_LEFT:
      MoveCursor(Cur-1);
      Redraw();
      return this;
    case KEY_RIGHT:
      MoveCursor(Cur+1);
      Redraw();
      return this;
    case KEY_DEL:
      Delete();
      Redraw();
      return this;
    case KEY_HOME:
      MoveCursor(0);
      Redraw();
      return this;
    case KEY_END:
      MoveCursor(strlen(Input));
      Redraw();
      return this;
    case KEY_ENTER:
      strcpy(OldInput, Input); DeFocus();
      if (Owner!=NULL)
        Owner->HandleEvent(cmInputChanged, this);
      return this;
    case KEY_BACKSPACE:
      BackSpace();
      Redraw();
      return this;
    case KEY_ESC:
      strcpy(Input, OldInput);
      DeFocus();
      return this;
  }

  if (Char>31)
  {
    Insert(Char);
    Redraw();
    return this;
  }

  return CView::HandleKey(Char, Scancode);
}


void CInputLine::Draw()
{
  if (Owner!=NULL)
  {
    BITMAP *Buffer = Owner->GetBuffer();

    button(Buffer, px, py, sx, sy, Color(colDark), Color(colLight), Color(colBack));

    // Text output
//    FONT *Font = View_GetFont(small7);
//    int h = text_height(Font);

    char Char[2]=" ";
    int x=0;
    int p=Pos;
    VisChars=0;

    for (;p<MaxLen;p++)
    {
      char c = *(Input+p);
      if (c==0) break;
      Char[0] = c;

      int w = SystemFont.Length(Char);
      if (x+w>=sx-4) break;

      if (Focused==this)
      {
        if (Cur==p) Cgfx::Rectfill(Buffer, px+2+x, py+2, px+2+x+w-1, py+sy-3,
                             Color(colLight));
      }

      Cgfx::Text(Buffer, SystemFont, Char, px+2+x, py+1, Color(colFore));

      VisChars++;
      x += w;
    }

    VisChars += (sx-4-x)/5;

    if (Focused==this)
      if (p<=Cur && x+5<sx-4)
      {
        Cgfx::Rectfill(Buffer, px+2+x+(Cur-p)*5, py+2, px+2+x+(Cur-p)*5+4, py+sy-3,
                 Color(colLight));
      }
  }

  CView::Draw();
}


int CInputLine::Focus()
{
  int Result=CView::Focus();
  Redraw();
  return Result;
}


int CInputLine::DeFocus()
{
  if (strcmp(OldInput, Input)!=0)
  {
    strcpy(OldInput, Input);
    if (Owner!=NULL) Owner->HandleEvent(cmInputChanged, this);
  }

  int Result=CView::DeFocus();
  Redraw();
  return Result;
}


int CInputLine::CanClose()
{
  if (Focused==this) return DeFocus(); else return CView::CanClose();
}


char *CInputLine::GetInput()
{
  return Input;
}


void CInputLine::SetInput(char *aInput)
{
  Pos = 0;
  Cur = 0;

  strncpy(Input, aInput, MaxLen);
  *(Input+MaxLen)=0;

  Redraw();
}


void CInputLine::MoveCursor(unsigned int c)
{
//  if (c<0) c=0;
  if (c>strlen(Input)) c = strlen(Input);
  if (c>=MaxLen) c = MaxLen-1;

  Cur = c;

  if (Cur<Pos) Pos = Cur;
  if (Cur>=Pos+VisChars) Pos = Cur-VisChars+1;
}


void CInputLine::Insert(char c)
{
  if (Keys!=NULL)
  {
    if (strchr(Keys, c)==NULL) return;
  }

  int Len=strlen(Input);
  if (Len<MaxLen)
  {
    if (Cur>=Len)
    {
      for (; Len<Cur; Len++) *(Input+Len)=' ';
      *(Input+Cur+1)=0;
      *(Input+Cur)=c;
    } else
    {
      for (char* t=Input+Len; t>Input+Cur; t--) *t=*(t-1);
      *(Input+Cur)=c;
    }

    MoveCursor(Cur+1);
  }
}


void CInputLine::BackSpace()
{
  if (Cur>0)
  {
    if (Cur-1<strlen(Input)) strcpy(Input+Cur-1, Input+Cur);
    MoveCursor(Cur-1);
  }
}


void CInputLine::Delete()
{
  if (Cur<strlen(Input))
  {
    if (Cur==strlen(Input)-1) *(Input+Cur)=0; else
      strcpy(Input+Cur, Input+Cur+1);
  }
}

void CInputLine::DefaultScale()
{
  sy = SystemFont.Height()+4;
  sx = SystemFont.Length("A")*MaxLen;
}




