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

 v_text.cc - Simple static text

*/


#include <string.h>
#include "v_text.h"
#include "v_common.h"
#include "v_gfx.h"
#include "v_system.h"
#include "v_save.h"


const LineSpacing=2;


txtFlags operator|(const txtFlags one, const txtFlags two)
{
  return (txtFlags)((int)one | (int)two);
}


//===================================================


CText::CText(int apx, int apy, char *aText, txtFlags aFlags)
 : CView(apx, apy, 1, 1)
{
  Text = strdup(aText);
  dx = 0;
  dy = 0;
  Lines = NULL;
  Text = NULL;
  LineNum = 0;
  Flags = aFlags;
  AutoWidth = 1;
  AutoHeight = 1;

  if (Flags & txtTransparent) SetTransparent(1);

  SetText(aText);
}


CText::CText(int apx, int apy, int asx, int asy, char *aText, txtFlags aFlags)
 : CView(apx, apy, asx, asy)
{
  Text = strdup(aText);
  dx = 0;
  dy = 0;
  Lines = NULL;
  Text = NULL;
  LineNum = 0;
  Flags = aFlags;
  AutoWidth = 0;
  AutoHeight = 0;

  if (Flags & txtTransparent) SetTransparent(1);

  SetText(aText);
}


CText::CText(int apx, int apy, int asx, char *aText, txtFlags aFlags)
 : CView(apx, apy, asx, 100)
{
  Text = strdup(aText);
  dx = 0;
  dy = 0;
  Lines = NULL;
  Text = NULL;
  LineNum = 0;
  Flags = aFlags;
  AutoWidth = 0;
  AutoHeight = 1;

  if (Flags & txtTransparent) SetTransparent(1);

  SetText(aText);
}

CText::~CText()
{
  if (Text!=NULL) free(Text);
  if (Lines!=NULL) free(Lines);
}


void* CText::HandleEvent(cmCommand Event, void* Data)
{
  // Output serialize imformation
  if(Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CText P%p Text=\"%s\" Flags=%i }\n",this,Text,Flags);
    AddSerializeInfo(Data,info);
  }
    
  return CView::HandleEvent(Event,Data);
}


void CText::Draw()
{
  if (Owner!=NULL)
  {
    BITMAP *Buffer = Owner->GetBuffer();

    if ((Flags & txtTransparent)==0)
      Cgfx::Rectfill(Buffer, px, py, px+sx-1, py+sy-1, Color(colBack));

    int y = py;
    for (int n=0; n<LineNum && y<=py+sy; n++)
    {
        if (Flags & txtCentered)
        {
          Cgfx::TextCenter(Buffer, SystemFont, Lines[n], px+sx/2, y, Color(colFore));
        } else
          Cgfx::Text(Buffer, SystemFont, Lines[n], px, y, Color(colFore));

        y+=SystemFont.Height() + LineSpacing;
    }
  }

  CView::Draw();
}


void CText::SetText(char *aText)
{
  if (Text!=NULL) free(Text);
  if (Lines!=NULL) free(Lines);

  Text = strdup(aText);

  if (AutoWidth)
  {
    // count lines
    LineNum=1;
    char *s=Text;
    while (*s)
    {
      if (*s=='\n') LineNum++;
      if (*s=='\\' && *(s+1) == '0')
      {
        *s = 0;
        break;
      }
      s++;
    }

    // create lines
    Lines = (char**)malloc(sizeof(char*) * LineNum);

    Lines[0] = Text;
    int n=1;
    s=Text;

    while (*s)
    {
      if (*s=='\n')
      {
        *s = '\0';
        Lines[n] = s+1;
        n++;
      }

      s++;
    }

    // search longest line
    int Maxw=0;
    for (n=0; n<LineNum; n++)
    {
      int w = SystemFont.Length(Lines[n]);
      if (w>Maxw) Maxw=w;
    }

    if (AutoHeight) 
      ScaleTo(Maxw, SystemFont.Height()*LineNum + LineSpacing*(LineNum-1));

    int nx = px-dx;
    int ny = py-dy;

    if (Flags & txtCentered)
    {
      dx = -sx/2;
      dy = 0;
    } else
    {
      dx = 0;
      dy = 0;
    }

    MoveTo(nx+dx, ny+dy);
  } else
  {
    // count lines
    LineNum=1;
    char *s=Text;
    char *Line=Text;
    while (*s)
    {
      if (*s=='\n')
      {
        Line=s+1;
        LineNum++;
      } else
      if (*s=='\\' && *(s+1)=='0')
      {
        *s=0;
        break;
      } else
      {
        char Old = *s;
        *s = '\0';
        int w = SystemFont.Length(Line);
        *s = Old;

        if (w>sx)
        {
          while (s>=Line && *s!=' ') s--;

          if (s<Line) break;

          *s = '\n';
          Line= s+1;
          LineNum++;
        }
      }
      s++;
    }

    // create lines
    Lines = (char**)malloc(sizeof(char*) * LineNum);

    Lines[0] = Text;
    int n=1;
    s=Text;

    while (*s)
    {
      if (*s=='\n')
      {
        *s = '\0';
        Lines[n] = s+1;
        n++;
      }

      s++;
    }

    if (AutoHeight)
      ScaleTo(sx, SystemFont.Height()*LineNum + LineSpacing*(LineNum-1));
  }

  Redraw();
}


void CText::DefaultScale()
{
  sy = SystemFont.Height();
  sx = ViewTextLength(SystemFont,Text);
}



