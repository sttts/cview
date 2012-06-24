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

 v_msg.cc - Message and input dialog class

*/


#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "v_msg.h"
#include "v_common.h"
#include "v_pictur.h"
#include "v_string.h"
#include "v_text.h"
#include "v_sysbit.h"

#include <allegro.h>


const Buttonh = 15;
const Buttonw = 60;
const ButtonSpacing = 5;
const WindowSpacing = 5;
const Inputw = 150;


struct CStdButton
{
  int StringNum;
  cmCommand Command;
  msgButtons BitMask;
};

CStdButton StdButtons[] = {
                           {strOk,     cmOk,      msgOk        },
                           {strCancel, cmCancel,  msgCancel    },
                           {strYes,    cmYes,     msgYes       },
                           {strNo,     cmNo,      msgNo        },
                           {strRetry,  cmRetry,   msgRetry     },
                           {strIgnore, cmIgnore,  msgIgnore    },
                           {0,         cmNone ,   (msgButtons)0}
                          };

struct CStdIcon
{
  msgIcon Id;
  bmpBitmap BitmapNum;
};

CStdIcon StdIcons[] = {
                       {msgIconQuestion,       bmpQuestion     },
                       {msgIconExclamation,    bmpExclamation  },
                       {msgIconInformation,    bmpInformation  },
                       {(msgIcon)0,            (bmpBitmap)0}
                      };


// Helper button operations
msgButtons operator|(const msgButtons one, const msgButtons two)
{
  return (msgButtons)((int)one | (int)two);
}


//=======================================================


int MessageBox(int aButtons, int aIcons, char *aTitle, char *aMsg, ...)
{
  va_list arg;
  va_start(arg, aMsg);
  CMessageBox *Msg = new CMessageBox(aButtons, aIcons, aTitle, aMsg, arg);
  int r=ExecuteWindow(Msg);
  delete Msg;
  va_end(arg);
  return r;
}


int InputBox(char *aTitle, char *aLabel, char *aInput, char *Buffer, int nMaxLen)
{
  CInputBox *Box = new CInputBox(aTitle, aLabel, nMaxLen, aInput);
  int r=ExecuteWindow(Box);
  if (r==cmOk) strncpy(Buffer, Box->GetInput(), nMaxLen); else strcpy(Buffer, "");
  delete Box;
  return r;
}


//====================================================


CMessageBox::CMessageBox(int aButtons, int aIcons, int Width, int Height,
                         char *aTitle, char *aMsg, ...)

 : CDialog(0, 0, 100, 10, aTitle, dlgDrag | dlgCenter)
{
  va_list arg;
  va_start(arg, aMsg);
  SetupWindow(aButtons, aIcons, Width, Height, aTitle, aMsg, arg);
  va_end(arg);
}


CMessageBox::CMessageBox(int aButtons, int aIcons, char *aTitle,
  char *aMsg, ...)
  : CDialog(0, 0, 100, 10, aTitle, dlgDrag | dlgCenter)
{
  va_list arg;
  va_start(arg, aMsg);
  SetupWindow(aButtons, aIcons, 200, 0, aTitle, aMsg, arg);
  va_end(arg);
}


CMessageBox::CMessageBox(int aButtons, int aIcons, int Width, int Height,
                         char *aTitle, char *aMsg, va_list arg)

 : CDialog(0, 0, 100, 10, aTitle, dlgDrag | dlgCenter)
{
  SetupWindow(aButtons, aIcons, Width, Height, aTitle, aMsg, arg);
}


CMessageBox::CMessageBox(int aButtons, int aIcons, char *aTitle,
  char *aMsg, va_list arg)
  : CDialog(0, 0, 100, 10, aTitle, dlgDrag | dlgCenter)
{
  SetupWindow(aButtons, aIcons, 200, 0, aTitle, aMsg, arg);
}


CMessageBox::~CMessageBox()
{
}


void CMessageBox::SetupWindow(int aButtons, int aIcons, int Width, int Height,
    char *aTitle, char *aMsg, va_list arg)
{
  (void)aTitle;

  // size of window
  int h = Height;
  int w = Width;
  int tx = 0; // Text start

  // Insert icons
  if (aIcons>0)
  {
    int n=0;

    while (StdIcons[n].Id!=0)
    {
      if (StdIcons[n].Id==aIcons) break;
      n++;
    }

    if (StdIcons[n].Id!=0)
    {
      BITMAP *Icon = SystemBitmap[StdIcons[n].BitmapNum];

      Client->Insert(new CPicture(WindowSpacing, WindowSpacing,
                                  Icon->w, Icon->h, Icon, pctTrans));

      tx = Icon->w + WindowSpacing;
    }
  }

  // Insert text
  CView *Text = NULL;

  char *Message = new char[1024];
  vsprintf(Message, aMsg, arg);

  if (aIcons==0)
  {
    Text = new CText(tx+WindowSpacing, WindowSpacing,
                     w-tx-2*WindowSpacing, Message,
                     txtDefault | txtCentered);
  } else
  {
    Text = new CText(tx+WindowSpacing, WindowSpacing,
                     w-tx-2*WindowSpacing, Message);
  }

  delete Message;

  Client->Insert(Text);

  // Calculate height
  int nh = Title->Height() + 2*WindowSpacing +
           Text->Height() + 2*ButtonSpacing + Buttonh;

  if (nh>h) h=nh;

  // Count buttons
  int z=0;
  int n;

  for (n=0; StdButtons[n].BitMask!=0; n++)
    if (aButtons & StdButtons[n].BitMask) z++;

  if (z==0)
  {
    aButtons = msgOk;
    z = 1;
  }

  // Insert Buttons
  int x = (w - z*Buttonw - (z-1)*ButtonSpacing) / 2;
  int y = h - WindowSpacing - Buttonh - Title->Height();

  for (n=0; StdButtons[n].BitMask!=0; n++)
  {
    if (aButtons & StdButtons[n].BitMask)
    {
      Client->Insert(new CTextButton(x, y, Buttonw, Buttonh,
        View_GetString(StdButtons[n].StringNum),
        StdButtons[n].Command, this));

      x += Buttonw + ButtonSpacing;
    }
  }

  // Adjust window
  Enclose();
  Center();
}


//====================================================


CInputBox::CInputBox(int asx, char *aTitle, char *aLabel,
                     int aMaxLen, char *aInput, char *aKeys)
 : CDialog(0, 0, asx, 10, aTitle, dlgDrag | dlgCenter)
{
  int x = WindowSpacing;
  int y = WindowSpacing;

  // Label
  if (aLabel)
  {
    CText *Label=new CText(x, y, aLabel);
    x += Label->Width()+WindowSpacing;
    Client->Insert(Label);
  }

  // Input line
  Input = new CInputLine(x, y, sx-x-WindowSpacing, 12, aMaxLen, aInput, aKeys);
  Client->Insert(Input);

  // Ok and Cancel button
  y+=12+WindowSpacing;
  x=(sx-2*Buttonw-ButtonSpacing)/2;

  Client->Insert(new CTextButton(x, y, Buttonw, Buttonh,
        View_GetString(strOk), cmOk));

  x+=ButtonSpacing+Buttonw;

  Client->Insert(new CTextButton(x, y, Buttonw, Buttonh,
        View_GetString(strCancel), cmCancel));
  y+=Buttonh+WindowSpacing;

  // Scale to new size
  Enclose();
  Center();
}


CInputBox::CInputBox(char *aTitle, char *aLabel,
                     int aMaxLen, char *aInput, char *aKeys)
 : CDialog(0, 0, 100, 10, aTitle, dlgDrag | dlgCenter)
{
  int x = WindowSpacing;
  int y = WindowSpacing;

  // Label
  if (aLabel)
  {
    CText *Label=new CText(x, y, aLabel);
    x += Label->Width()+WindowSpacing;
    Client->Insert(Label);
  }

  // Input line
  Input = new CInputLine(x, y, Inputw, 12, aMaxLen, aInput, aKeys);
  Client->Insert(Input);
  x += Inputw+WindowSpacing;
  ScaleTo(x, sy);

  // Ok and Cancel button
  y+=12+WindowSpacing;
  x=(sx-2*Buttonw-ButtonSpacing)/2;

  Client->Insert(new CTextButton(x, y, Buttonw, Buttonh,
        View_GetString(strOk), cmOk));

  x+=ButtonSpacing+Buttonw;

  Client->Insert(new CTextButton(x, y, Buttonw, Buttonh,
        View_GetString(strCancel), cmCancel));
  y+=Buttonh+WindowSpacing;

  // Scale to new size
//  ScaleTo(sx, y+Title->Height());
  Enclose();
  Center();
}


CInputBox::~CInputBox()
{
}


char *CInputBox::GetInput()
{
  return Input->GetInput();
}


void CInputBox::SetInput(char *aInput)
{
  Input->SetInput(aInput);
}

