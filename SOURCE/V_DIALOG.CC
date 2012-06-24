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

 v_dialog.cc - Dialog class and helper classes
             - Border class by Doug Eleveld

*/


#include <string.h>
#include "v_dialog.h"
#include "v_common.h"
#include "v_gfx.h"
#include "v_border.h"
#include "v_sysbit.h"
#include "v_system.h"
#include "v_save.h"

#include <allegro.h>

int CDialog::TitleSize         = 11;

const int CDialog::ResizeSpace       = 3;
const int CDialog::DiagResizeSpace   = 10;

const int CDialog::drgDiag           = 3;
const int CDialog::drgHor            = 1;
const int CDialog::drgVert           = 2;


//================================================================


dlgFlags operator|(const dlgFlags one, const dlgFlags two)
{
  return (dlgFlags)((int)one | (int)two);
}


dlgFlags operator&(const dlgFlags one, const dlgFlags two)
{
  return (dlgFlags)((int)one & (int)two);
}


//================================================================


CDialog::CDialog(int apx, int apy, int asx, int asy, char *aTitle,
                 dlgFlags aFlags)
 : CWindow(apx, apy, asx, asy+TitleSize),
   Close(NULL)
{
  Flags = aFlags;
  DragMode = 0;

  Border = new CBorder(*this);
  Insert(Border);

  // Make a titlebar and ask the border to position it
  Title = new CTitlebar(0, 0, 1, 1, aTitle, Flags & dlgCenter);
  Border->Drawer.PositionTitle(*this,*Title);
  Insert(Title);
  
  // Make a close button and ask the border to position it
  if (Flags & dlgClose)
  {
    Close = new CCloseButton(0, 0, 1, 1);
    Border->Drawer.PositionClose(*this,*Close);
    Insert(Close);
  }

  // Make a client button and ask the border to position it
  Client = new CClient(0,0,1,1);
  Border->Drawer.PositionClient(*this,*Client);
  Insert(Client);

  UpdateLook();
}


CDialog::~CDialog()
{
}


void CDialog::Draw(int ax, int ay, int asx, int asy)
{
  CWindow::Draw(ax,ay,asx,asy);
}


void* CDialog::HandleEvent(cmCommand Event, void* Data)
{
  switch(Event)
  {
    case cmVideoMode: if (px>=Cgfx::Width())
                      {
                        MoveTo(Cgfx::Width()-sx, py);
                      }
                      if (py>=Cgfx::Height())
                      {
                        MoveTo(px, Cgfx::Height()-sy);
                      }

                      UpdateLook();
                      Redraw();
                      break;
    case cmOk:
    case cmYes:
    case cmNo:
    case cmIgnore:
    case cmRetry:
               if (CanClose()) Stop(Event);
               break;
    case cmCancel: Stop(cmCancel);
                   break;
    case cmDrag: if (!(Flags & dlgDrag)) return NULL;
                   break;

    case cmDialogStyle:
    {
      // Get the border out of the data
      CDialogBorderStyle::rep* newStyle = (CDialogBorderStyle::rep*)Data;

      // Set the new border
      if (newStyle!=NULL)
        SetStyle(*newStyle);

      break;
    }
    case cmSerialize:
    {
      char info[1000];
      sprintf(info,"{ CDialog P%p TitleText=\"%s\" TitleCentered=%i Close=%p Flags=%i }\n",this,Title->GetText(),Title->GetCentered(),Close,Flags);
    
      AddSerializeInfo(Data,info);
    }
                   
    default: { };
  }
  return CWindow::HandleEvent(Event, Data);
}


void CDialog::HandleMouse(int mx, int my, int buttons)
{
  if (Dragging)
  {
    if (buttons==1)
    {
      int x = sx;
      int y = sy;

      if (DragMode & drgHor) x = mx-px;
      if (DragMode & drgVert) y = my-py;

      ScaleTo(x, y);
    } else
    {
      SetMouse(NULL);
      Drop();
    }
  } else
  {
    if (Oldbuttons==0 && buttons==1) ToFront();

    if (Flags & dlgScale)
    {
      // Find drag mode
      if (my>=py+sy || mx>=px+sx) DragMode = 0; else
      if ((mx>=px+sx-DiagResizeSpace && my>=py+sy-ResizeSpace) ||
          (mx>=px+sx-ResizeSpace && my>=py+sy-DiagResizeSpace))
        DragMode = drgDiag;
      else if (mx>=px+sx-ResizeSpace) DragMode = drgHor;
      else if (my>=py+sy-ResizeSpace) DragMode = drgVert;
      else DragMode = 0;

      // Select new mouse bitmap
      BITMAP *m = NULL;
      switch (DragMode)
      {
        case drgDiag: m = SystemBitmap[bmpResizeDiag]; break;
        case drgHor:  m = SystemBitmap[bmpResizeHor];  break;
        case drgVert: m = SystemBitmap[bmpResizeVert]; break;
      }
      if (m!=NULL) SetMouse(m, m->w/2, m->h/2); else SetMouse(NULL);

      // Start scaling
      if (DragMode!=0 && Oldbuttons==0 && buttons==1)
      {
        Drag(mx-px-sx, my-py-sy);
        return;
      }
    }

    CWindow::HandleMouse(mx, my, buttons);
  }
}


void* CDialog::HandleKey(char Char, char Scancode)
{
  // Check if we are supposed to stop from an escape key
  if (Scancode==KEY_ESC && (Flags&dlgEscape))
  {
    Stop(cmEscape);
    return this;
  }
  else
    return CWindow::HandleKey(Char,Scancode);
}


void CDialog::UpdateLook()
{
/*  if (Cgfx::Width()>320) TitleSize = 11;
    else TitleSize = 9;

  ScaleTo(sx, Client->Height()+TitleSize);

  if (Flags & dlgClose)
  {
    Close->ScaleTo(TitleSize, TitleSize);
    Close->MoveTo(sx-TitleSize, 0);
    Title->ScaleTo(sx-TitleSize, TitleSize);
  } else
  {
    Title->ScaleTo(sx, TitleSize);
  }

  Client->MoveTo(0, TitleSize);*/
}


void CDialog::ScaleTo(int x, int y)
{
  CorrectSize(x, y);
  CWindow::ScaleTo(x, y);
}


void CDialog::Enclose(int border)
{
  // Get the current difference between the client and
  // the border, we have to preserve this border area
  int ex = sx - Client->Posx() - Client->Width();
  int ey = sy - Client->Posy() - Client->Height();

  CGroup::Enclose(0);

  // Rescale the dialog with the extra border
  Scale(ex+border,ey+border);
}


void CDialog::CorrectSize(int &x, int &y)
{
  if (x<20) x=20;
  if (y<Title->Height()+5) y=Title->Height()+5;
}


void CDialog::SetBackground(BITMAP *aTitle, BITMAP *aClient)
{
  Title->SetBackground(aTitle);
  Client->SetBackground(aClient);
}

void CDialog::Center()
{
  int x;
  int y;

  if (Owner!=NULL)
  {
    x = (Owner->Width()-Width())/2;
    y = (Owner->Height()-Height())/2;
  } else
  {
    x = (Cgfx::Width()-Width())/2;
    y = (Cgfx::Height()-Height())/2;
  }

  MoveTo(x, y);
}


void CDialog::SetStyle(CDialogBorderStyle::rep& newStyle)
{
  Border->Drawer = newStyle;

  // Re-position the sub objects
  Border->Drawer.PositionTitle(*this,*Title);
  if(Close!=NULL)
    Border->Drawer.PositionClose(*this,*Close);
  Border->Drawer.PositionClient(*this,*Client);

  Redraw();
}


//=====================================================


CClient::CClient(int apx, int apy, int asx, int asy)
 : CView(apx, apy, asx, asy)
{
  Back = NULL;
  ScaleMode = sclRight | sclBottom;
}


CClient::~CClient()
{
}


void CClient::Insert(CView *View)
{
  View->Move(px,py);
  if (Owner!=NULL)
     ((CGroup*)Owner)->Insert(View);
}


void CClient::Remove(CView *View)
{
  if (Owner!=NULL)
     ((CGroup*)Owner)->Remove(View);
}


void* CClient::HandleEvent(cmCommand Event, void* Data)
{
  if(Event==cmSerialize)
    return NULL;
    
  return CView::HandleEvent(Event,Data);
}


void CClient::Draw()
{
  if (Owner==NULL) return;
  BITMAP *Buffer = Owner->GetBuffer();

  // Background
  if (Back==NULL)
    Cgfx::Rectfill(Buffer, px, py, px+sx-1, py+sy-1, Color(colBack));
  else
    if (Back!=NULL)
    {
      for (int y=px; y<py+sy-1; y+=Back->h)
        for (int x=py; x<px+sx-1; x+=Back->w)
        {
          blit(Back, Buffer, 0, 0, x, y, Back->w, Back->h);
        }
    }

  CView::Draw();
}


void CClient::SetBackground(BITMAP *aBack)
{
  Back = aBack;
  RedrawOwner();
}


//=====================================================


CBorder::CBorder(CDialog& aTarget)
 :CView(0, 0, 1, 1),
 Drawer(CDialogBorderStyle::Default),
 Target(aTarget)
{
  SetTransparent(true);
  ScaleTo(Target.Width(), Target.Height());
  ScaleMode = sclRight | sclBottom;
}


CBorder::~CBorder()
{
}


void* CBorder::HandleEvent(cmCommand Event, void* Data)
{
  if(Event==cmSerialize)
    return NULL;

  return CView::HandleEvent(Event,Data);
}


//=====================================================


CCloseButton::CCloseButton(int apx, int apy, int asx, int asy)
 : CButton(apx, apy, asx, asy, cmClose)
{
  ScaleMode = sclLeft | sclRight;
}


CCloseButton::~CCloseButton()
{
}


void* CCloseButton::HandleEvent(cmCommand Event, void* Data)
{
  if(Event==cmSerialize)
    return NULL;

  return CView::HandleEvent(Event,Data);
}


void CCloseButton::Draw()
{
  CButton::Draw();

  if (Owner!=NULL)
  {
    BITMAP *Buffer = Owner->GetBuffer();

    if (!Clicked)
    {
      line(Buffer, px+2, py+2, px+sx-3, py+sy-3, Color(colFore));
      line(Buffer, px+sx-3, py+2, px+2, py+sy-3, Color(colFore));
    } else
    {
      line(Buffer, px+2, py+2, px+sx-3, py+sy-3, Color(colLight));
      line(Buffer, px+sx-3, py+2, px+2, py+sy-3, Color(colLight));
    } 
  }
}


//=====================================================


CTitlebar::CTitlebar(int apx, int apy, int asx,  int asy, char *aText,
                     int aCentered)
 : CView(apx, apy, asx, asy)
{
  ScaleMode = sclRight;
  Text = strdup(aText);
  Back = NULL;
  Centered = aCentered;
  OnTop = 0;
}


CTitlebar::~CTitlebar()
{
  if (Text!=NULL) free(Text);
}


void CTitlebar::HandleMouse(int mx, int my, int buttons)
{
  if (!Dragging)
  {
    if (Oldbuttons==0 && buttons==1)
    {
      if (Owner!=NULL) Drag(mx-px, my-py);
    }
  } else
  {
    if (buttons==1)
    {
      if (Owner!=NULL) Owner->Move(mx-px, my-py);
    } else Drop();
  }

  CView::HandleMouse(mx, my, buttons);
}


void* CTitlebar::HandleEvent(cmCommand Event, void* Data)
{
  switch(Event)
  {
  case cmTopWindow: OnTop = 1;
                    RedrawOwner();
                    break;
  case cmNotTopWindow: OnTop = 0;
                       RedrawOwner();
                       break;

  case cmSerialize:
    return NULL;
                       
  default: { };
  }

  return CView::HandleEvent(Event, Data);
}


void CTitlebar::Draw()
{
  if (Owner!=NULL)
  {
    BITMAP *Buffer = Owner->GetBuffer();

    // Background
    if (Back==NULL)
    {

      if (OnTop)
      {
        Cgfx::Rectfill(Buffer, px, py, px+sx-1, py+sy-1, Color(colSelect));
      }
      else
      {
        Cgfx::Rectfill(Buffer, px, py, px+sx-1, py+sy-1, Color(colBack));
      }
    }
    else
      if (Back!=NULL)
      {
        for (int y=0; y<sy; y+=Back->h)
          for (int x=0; x<sx; x+=Back->w)
          {
            int w=Back->w;
            int h=Back->h;
            if (px+x+w>sx) w=sx-x-px;
            if (py+y+h>sy) h=sy-y-py;

            blit(Back, Buffer, 0, 0, px+x, py+y, w, h);
          }
      }
    // Title text
    text_mode(-1); // transparent

    int ty = SystemFont.Height();
    
    int rx;
    if (Centered)
    {
      int tx = SystemFont.Length(Text);
      rx = (sx-tx)/2;
    } else rx=2;

    int ry = (sy-ty)/2;

    Cgfx::Text(Buffer, SystemFont, Text, px+rx, py+ry, Color(colFore));
  }

  CView::Draw();
}


void CTitlebar::SetBackground(BITMAP *aBack)
{
  Back = aBack;
}

