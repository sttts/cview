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

 v_combo.cc - Combo box class

*/


#include "v_combo.h"
#include "v_base.h"
#include "v_utils.h"
#include "v_button.h"
#include "v_common.h"
#include "v_prog.h"
#include "v_sysbit.h"
#include "v_system.h"
#include "v_save.h"

#include <allegro.h>


//const HEIGHT = 12;
const BORDER = 3;


//=====================================================


class CPopupWindow:public CWindow
{
 public:
  CPopupWindow(int apx, int apy, int asx, int asy);
  virtual ~CPopupWindow();

  virtual void HandleMouse(int mx, int my, int buttons);
  virtual void* HandleKey(char Char, char Scancode);

 protected:
};


CPopupWindow::CPopupWindow(int apx, int apy, int asx, int asy)
 : CWindow(apx, apy, asx, asy)
{
  Insert(new C3dOut(0, 0, sx, sy));
}


CPopupWindow::~CPopupWindow()
{
}


void CPopupWindow::HandleMouse(int mx, int my, int buttons)
{
  if (Oldbuttons!=0 && buttons==0)
  {
    if (!IsIn(mx, my)) Stop(cmCancel);
  }

  CWindow::HandleMouse(mx, my, buttons);
}


void* CPopupWindow::HandleKey(char Char, char Scancode)
{
  if (Char==27)
  {
    Stop(cmCancel);
    return this;
  }
  return CWindow::HandleKey(Char, Scancode);
}


//=====================================================


CCombobox::CCombobox(int apx, int apy, int asx, int asy,
                     char *aContent, int aFlags)
 : CGroup(apx, apy, asx, asy)
{
  Flags = aFlags;

  int HEIGHT = SystemFont.Height()+2;

  lx = asx;
  ly = asy-HEIGHT-1;

  Popup = NULL;
  Selected = -1;

  int left = SystemBitmap[bmpComboOut]->w;

  if (Flags & comboPopup)
  {
    CView *v=new CFilledRect(0, 0, sx, sy, Color(colBack));
    v->SetScaleMode(sclRight | sclBottom);
    CGroup::Insert(v);

    Scroller = new CScrollbar(lx-9-BORDER, BORDER, 9, ly-2*BORDER);
    Scroller->SetScaleMode(sclRight | sclLeft | sclBottom);

    List = new CListbox(BORDER, BORDER, lx-9-3*BORDER, ly-2*BORDER,
      lstDefault, this, NULL, Scroller);
    List->SetScaleMode(sclRight | sclBottom);

    Input = new CInputLine(0, 0, sx-left-2, HEIGHT, 256, "",
              NULL, (Flags & comboEditable) ? inpEditable : 0);
    Input->SetScaleMode(sclRight);
//    Input->DefaultScale();
    CGroup::Insert(Input);

    v = new CSystemBitmapButton(sx-left-2, 0, 1, 1,
      bmpComboOut, bmpComboIn, cmComboPopup);
    v->DefaultScale();
    v->SetScaleMode(sclRight | sclLeft);
    CGroup::Insert(v);

//    ScaleTo(sx, 12);
    ScaleTo(sx, SystemFont.Height()+3);

    // If the button isn't big enough vertically, then
    // we need to draw the background to avoid empty spots
//    if (Height()>SystemBitmap[bmpComboOut]->h)
//      SetTransparent(true);

  } else
  {
    CView *v = new CFilledRect(0, 0, sx, sy, Color(colBack));
    v->SetScaleMode(sclRight | sclBottom);
    CGroup::Insert(v);

    Scroller = new CScrollbar(lx-9, HEIGHT+1, 9, ly);
    Scroller->SetScaleMode(sclRight | sclLeft | sclBottom);

    List = new CListbox(0, HEIGHT+1, lx-9-BORDER, ly,
      lstDefault, this, NULL, Scroller);
    List->SetScaleMode(sclRight | sclBottom);

    Input = new CInputLine(0, 0, sx, 12, 256, "",
              NULL, (Flags & comboEditable) ? inpEditable : 0);
              Input->SetScaleMode(sclRight);
    Input->DefaultScale();
    CGroup::Insert(Input);

    CGroup::Insert(Scroller);
    CGroup::Insert(List);
  }
  
  Select(aContent);
}


CCombobox::~CCombobox()
{
  if (Scroller) delete Scroller;
  if (List) delete List;
}


void* CCombobox::HandleEvent(cmCommand Event, void* Data)
{
  switch(Event)
  {
  case cmComboPopup:
    ShowList();
    break;

  case cmListSelect:
    if ((CListbox *)Data==List)
    {
      CListItem *Act = List->GetSelectedItem();
      if (Act!=NULL)
      {
        Input->SetInput(Act->GetText());
        if (Popup) Popup->Stop();
        if (Owner) Owner->HandleEvent(cmComboChanged, this);
        Selected = List->GetSelectedId();
      } else Selected = -1;
    }
    break;

  case cmInputChanged:
    if ((CInputLine*)Data==Input)
    {
      Select(Input->GetInput());
      if (Owner) Owner->HandleEvent(cmComboChanged, this);
    }
    break;

  case cmSerialize:
  {
    char info[1000];
    sprintf(info,"{ CCombobox P%p Flags%i Selected=%i }",this,Flags,Selected);
    AddSerializeInfo(Data,info);
    break;
  }
  default: { };
  }

  return CGroup::HandleEvent(Event, Data);
}


void CCombobox::Insert(CListItem *Item)
{
  List->Insert(Item);
}


void CCombobox::Insert(char *aText, int aId)
{
  Insert(new CTextItem(aId, aText));
}


void CCombobox::RemoveItems()
{
  List->RemoveAll();
}


char *CCombobox::GetInput()
{
  return Input->GetInput();
}


int CCombobox::GetSelectedId()
{
  return Selected;
}


void CCombobox::Select(int aId)
{
  CListItem *i = List->GetNext(NULL, aId);

  if (i)
  {
    List->Select(i);
    Selected = i->GetId();
    Input->SetInput(i->GetText());
  } else
  {
    List->Select(NULL);
    Selected=-1;
    Input->SetInput("");
  }
}


void CCombobox::Select(char *aText)
{
  CListItem *i = List->GetNext(NULL, aText);

  if (i)
  {
    List->Select(i);
    Selected = i->GetId();
    Input->SetInput(i->GetText());
  } else
  {
    List->Select(NULL);
    Selected=-1;
    Input->SetInput(aText);
  }
}


void CCombobox::ShowList()
{
 if (Popup==NULL)
 {
   int x=px;

   int HEIGHT = SystemFont.Height()+2;
   
   int y=py+HEIGHT+1;
   
   MakeGlobal(x, y, Program);

   Popup = new CPopupWindow(x, y, lx, ly);

   Popup->Insert(List);
   Popup->Insert(Scroller);

   ExecuteWindow(Popup);

   Popup->Remove(Scroller);
   Popup->Remove(List);

   delete Popup;

   Popup = NULL;
 }
}

void CCombobox::DefaultScale()
{
  sy = SystemFont.Height()+3;
  sx = SystemFont.Length("A")*10;
}

