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

 v_menu.cc - Menu classes

*/


#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "v_menu.h"
#include "v_common.h"
#include "v_prog.h"
#include "v_system.h"
#include "v_save.h"

#include <allegro.h>


//==================================================


CMenuItem::CMenuItem(int asx, int asy, cmCommand aCommand, int aAccelKey)
  :AccelKey(aAccelKey),
  Command(aCommand),
  sx(asx),
  sy(asy)
{
}


CMenuItem::~CMenuItem()
{
}


void* CMenuItem::HandleEvent(cmCommand Event, void* Data)
{
  if (Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CMenuItem P%p Command=%i }\n",this,Command);
    AddSerializeInfo(Data,info);
  }
  return NULL;
}

  
void CMenuItem::HandleDown()
{
}


void CMenuItem::HandleUp()
{
  HandleCommand();
}


void CMenuItem::HandleLost()
{
}


void CMenuItem::HandleCommand()
{
  if (Command!=0) Owner->HandleCommand(Command);
}


void CMenuItem::DefaultScale()
{
  sx = 10;
  sy = 10;
}


//==================================================


CMenu::CMenu(int apx, int apy, int asx, int asy, int aVertical, CView *aTarget)
 : CWindow(apx, apy, asx, asy),
   Base(NULL),
   Active(NULL),
   Style(CMenuStyle::Default),
   Head(NULL),
   Vertical(aVertical),
   Target(aTarget)
{
}


CMenu::~CMenu()
{
  CMenuItem *Act = Base;
  while (Act!=NULL)
  {
    CMenuItem *Temp = Act->Next;
    delete Act;
    Act = Temp;
  }
  
}


void CMenu::Draw(int ax, int ay, int asx, int asy)
{
  ax -= px;
  ay -= py;
  set_clip(GetBuffer(), ax, ay, ax+asx-1, ay+asy-1);

  // Draw the background in the correct style
  Style.Back(*this);

  int x=0;
  int y=0;

  CMenuItem *Act=Base;
  while (Act!=NULL)
  {
    Highlight(Oldbuttons, Act, x, y);

    Next(Act, x, y);
    Act=Act->Next;
  }

  if (GetBufferType()==BufferFull)
  {
    blit(GetBuffer(), Owner->GetBuffer(), ax, ay, ax+px, ay+py, asx, asy);
  }
}


void* CMenu::HandleEvent(cmCommand Event, void* Data)
{
  // Possibly change our menu style
  if (Event==cmMenuStyle)
  {
    // Get the border out of the data
    CMenuStyle::rep* newStyle = (CMenuStyle::rep*)Data;

    // Set the new border
    if (newStyle!=NULL)
      SetStyle(*newStyle);
  }
  else if (Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CMenu P%p Vertical=%i }\n",this,Vertical);
    AddSerializeInfo(Data,info);
  }

  // Pass it down to any menu items
  CMenuItem *Act=Base;
  int x = 0, y = 0;
  while (Act!=NULL)
  {
    void* ret = Act->HandleEvent(Event,Data);
    if(ret!=NULL)
       return ret;

    Next(Act, x, y);
    Act=Act->Next;
  }

  // Use the event just like a normal window
  return CWindow::HandleEvent(Event,Data);
}

  
void CMenu::HandleMouse(int mx, int my, int buttons)
{
  CMenuItem *New = ItemAt(mx-px, my-py);
  CMenuItem *Old = Active;

  if (Active!=New || buttons!=Oldbuttons)
  {
    Active = New;

    // Redraw only the objects New and Old
    UpdateItem(Old);
    UpdateItem(New);

    if (Old!=NULL && New!=Old) Old->HandleLost();
    if (New!=NULL && New!=Old && buttons==1) New->HandleDown();
    if (New!=NULL && Oldbuttons==0 && buttons==1) New->HandleDown();
    if (New!=NULL && Oldbuttons==1 && buttons==0) New->HandleUp();
  }

  CWindow::HandleMouse(mx, my, buttons);
}


void* CMenu::HandleKey(char aChar, char aScancode)
{
  CMenuItem *oldActive=Active;

  // Check for enter or space to select the active element
  if (aScancode==KEY_ENTER || aScancode==KEY_SPACE)
  {
    if (Active!=NULL)
    {
      Active->HandleDown();
      Active->HandleUp();

      UpdateItem(Active);
      
      return this;
    }
  }

  // Check for escape to leave a menu
  else if (aScancode==KEY_ESC)
  {
    // an escape key while looking through menus
    Stop(cmCancel);
    Active = NULL;

    if (Active!=oldActive)
    {
      UpdateItem(oldActive);
      UpdateItem(Active);
    }

    return this;
  }
  return NULL;
}


void* CMenu::HandleAccel(char aChar, char aScancode)
{
  // Make sure it was an ALT combnation
  if (aChar!=0)
    return NULL;
  
  CMenuItem *oldActive=Active;

  // Check the active element first
  char realChar = SCANCODE_TO_KEY(aScancode);
  
  if (Active!=NULL && tolower(realChar) == tolower(Active->AccelKey) && Active->AccelKey)
  {
    Active->HandleDown();
    Active->HandleUp();

    UpdateItem(Active);
  }

  // Check forward of the active menu element
  CMenuItem *Act=Active;
  while (Act!=NULL)
  {
    // Find the next menu element
    Act=Act->Next;
    
    // Check for matching accel keys
    if (Act!=NULL && tolower(realChar) == tolower(Act->AccelKey) && Act->AccelKey)
    {
      Active = Act;
      Active->HandleDown();
      Active->HandleUp();

      if (Active!=oldActive)
      {
        UpdateItem(oldActive);
        UpdateItem(Active);
      }

      return this;
    }
  }

  // Check forward didn't work, so look from start
  Act=Base;
  while (Act!=Active && Act!=NULL)
  {
    // Check for matching accel keys
    if (tolower(realChar) == tolower(Act->AccelKey) && Act->AccelKey)
    {
      Active = Act;
      Active->HandleDown();
      Active->HandleUp();

      if (Active!=oldActive)
      {
        UpdateItem(oldActive);
        UpdateItem(Active);
      }
        
      return this;
    }
    
    // Find the next menu element
    Act=Act->Next;
  }
  return NULL;
}


void CMenu::MouseLost(int mx, int my, int buttons)
{
  if (Active!=NULL) Active->HandleLost();

  Active = NULL;
  Redraw();

  CWindow::MouseLost(mx, my, buttons);
}


void CMenu::Insert(CMenuItem *Item)
{
  if (Head!=NULL)
  {
    Head->Next = Item;
    Head = Item;
  } else
  {
    Head = Item;
    Base = Item;
  }

  Item->Next = NULL;
  Item->Owner = this;

  AdjustSize();
}


void CMenu::Remove(CMenuItem *Item)
{
  if (Base==Item)
  {
    Base = Item->Next;
    if (Head==Item) Head=NULL;
  } else
  {
    CMenuItem *Prev = Base;
    while (Prev!=NULL && Prev->Next!=Item)
    {
      Prev = Prev->Next;
    }

    if (Prev!=NULL)
    {
      Prev->Next = Item->Next;
      if (Head==Item) Head = Prev;
    }
  }

  Item->Next = NULL;
  Item->Owner = NULL;

  AdjustSize();
}


CMenuItem *CMenu::ItemAt(int ax, int ay)
{
  int x=0;
  int y=0;

  CMenuItem *Act=Base;
  while (Act!=NULL)
  {
    /*if (Vertical && ay>=y && ay<y+Act->Height()) return Act;
    else if (!Vertical && ax>=x && ax<x+Act->Width()) return Act;*/
    if (ay>=y && ay<y+Act->Height() &&
        ax>=x && ax<x+Act->Width()) return Act;

    Next(Act, x, y);
    Act=Act->Next;
  }

  return NULL;
}


void CMenu::GetItemPos(CMenuItem *Item, int &x, int &y)
{
  x=px;
  y=py;

  MakeGlobal(x, y, Program);

  CMenuItem *Act=Base;
  while (Act!=NULL && Act!=Item)
  {
    Next(Act, x, y);
    Act=Act->Next;
  }
}


void CMenu::Next(CMenuItem *Last, int &x, int &y)
{
  if (Last==0) return;

  if (Vertical)
    y += Last->Height();
  else
    x += Last->Width();
}


void CMenu::AdjustSize()
{
  int x=0;
  int y=0;

  int w=sx;
  int h=sy;

  int maxx = 10;

  // Make the menu elements to their desired width
  CMenuItem *Act=Base;
  while (Act!=NULL)
  {
    Act->DefaultScale();

    // Find the widest menu element for vertical menus
    if (Act->sx>maxx)
       maxx = Act->sx;

    Act=Act->Next;
  }

  // For vertical menus, adjust each elements width
  // to the same width as the widest
  if( Vertical )
  {
    Act=Base;
    while (Act!=NULL)
    {
      Act->sx = maxx;
      Act=Act->Next;
    }
  }

  Act=Base;
  while (Act!=NULL)
  {
    if (x+Act->Width()>w)
      w = x+Act->Width();
    if (y+Act->Height()>h)
      h = y+Act->Height();

    Next(Act,x,y);
    Act=Act->Next;
  }

  // Adjust our menu to fit the elements
  ScaleTo(w, h);
}


void CMenu::HandleCommand(cmCommand aCommand)
{
  if (Running)
  {
    if (aCommand!=cmCancel) Stop(aCommand);
  } else
  {
    if (Target!=NULL)
      Target->HandleEvent(aCommand);
    else
      Owner->HandleEvent(aCommand);
  }
}


void CMenu::SetStyle(CMenuStyle::rep& newStyle)
{
  Style = newStyle;
  AdjustSize();
  Redraw();
}


CMenuStyle& CMenu::GetStyle()
{
  return Style;
}


void CMenu::UpdateItem(CMenuItem* Item)
{
  if(Item==NULL)
    return;
    
  // Find the dimensions of the item area
  int x=0;
  int y=0;
  
  CMenuItem *Act=Base;
  while (Act!=NULL)
  {
    // Found the object to draw
    if (Act==Item)
    {
      // Possibly draw it to our internal buffer
      if (GetBufferType()==BufferFull)
        Highlight(Oldbuttons, Act, x, y);

      // Inform the owner that we have changed
      RedrawNextTime = 1;
      Owner->Update(this, px + x, py + y, Item->Width(), Item->Height());
      
      return;
    }
    // Search the next object to draw
    Next(Act, x, y);
    Act=Act->Next;
  }
}


//==================================================


CMenuBar::CMenuBar(int apx, int apy, int asx, int asy, CView *aTarget)
 : CMenu(apx, apy, asx, asy, 0, aTarget)
{
  ScaleMode = sclRight;
}


CMenuBar::~CMenuBar()
{
}


void* CMenuBar::HandleEvent(cmCommand Event, void* Data)
{
  if (Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CMenuBar P%p }\n",this);
    AddSerializeInfo(Data,info);
  }
  return CMenu::HandleEvent(Event,Data);
}

  
void CMenuBar::HandleMouse(int mx, int my, int buttons)
{
  //if (Oldbuttons==0 && buttons==1) Front();

  CMenu::HandleMouse(mx, my, buttons);
}


void* CMenuBar::HandleKey(char aChar, char aScancode)
{
  CMenuItem *Act=Base;

  CMenuItem *oldActive=Active;

  // Check for up and down arrows to move selection
  if (aScancode == KEY_RIGHT)
  {
    if (Active != NULL)
    {
      if (Active->Next != NULL)
        Active = Active->Next;
    }
    else
      Active = Base;

    if (Active!=oldActive)
    {
      UpdateItem(oldActive);
      UpdateItem(Active);
    }
      
    return this;
  }
  else if (aScancode == KEY_LEFT)
  {
    // Look through the list till the next object
    // is the one that is active
    if (Active != NULL)
    {
      Act=Base;
      while (Act!=NULL && Act->Next != Active)
      {
        Act = Act->Next;
      }

      if (Act!=NULL)
        Active = Act;
    }
    else
      Active = Base;

    if (Active!=oldActive)
    {
      UpdateItem(oldActive);
      UpdateItem(Active);
    }
      
    return this;
  }
  // Ignore up and down keys for a menu bar
  else if (aScancode == KEY_LEFT || aScancode == KEY_DOWN)
  {
     return NULL;
  }
  // Otherwise let the menu try and handle it
  else
     return CMenu::HandleKey(aChar,aScancode);
}


void CMenuBar::Highlight(int buttons, CMenuItem *Item, int x, int y)
{
  Style.HighlightBar(*this,buttons,Item,x,y,Active);
}


//==================================================


CPopupMenu::CPopupMenu(int apx, int apy, CMenuNode *aNode, CView *aTarget)
 : CMenu(apx, apy, 1, 1, 1, aTarget)
{
  Shown = 0;
  Node = aNode;
}


CPopupMenu::~CPopupMenu()
{
}


void* CPopupMenu::HandleEvent(cmCommand Event, void* Data)
{
  if (Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CPopupMenu P%p }\n",this);
    AddSerializeInfo(Data,info);
  }
  return CMenu::HandleEvent(Event,Data);
}

  
void CPopupMenu::HandleMouse(int mx, int my, int buttons)
{
  if (Running)
  {
    if (!IsIn(mx, my))
    {
      if (my<py)
      {
        if (!Node->IsIn(mx, my)) Stop(cmCancel);
      } else
      {
        if (Oldbuttons==0 && buttons!=0) Stop(cmCancel); else
        if (Oldbuttons!=0 && buttons==0) Stop(cmCancel);
      }
    }
  }

  CMenu::HandleMouse(mx, my, buttons);
}


void* CPopupMenu::HandleKey(char aChar, char aScancode)
{
  CMenuItem *Act=Base;

  CMenuItem *oldActive=Active;

  // Check for up and down arrows to move selection
  if (aScancode == KEY_DOWN)
  {
    if (Active != NULL)
    {
      if (Active->Next != NULL)
        Active = Active->Next;
    }
    else
      Active = Base;

    if (Active!=oldActive)
    {
      UpdateItem(oldActive);
      UpdateItem(Active);
    }
    return this;
  }
  else if (aScancode == KEY_UP)
  {
    // Look through the list till the next object
    // is the one that is active
    if (Active != NULL)
    {
      Act=Base;
      while (Act!=NULL && Act->Next != Active)
      {
        Act = Act->Next;
      }

      if (Act!=NULL)
        Active = Act;
    }
    else
      Active = Base;

    // !!! Here we should only redraw menu items
    // Active and oldActive
    if (Active!=oldActive)
      Redraw();
      
    return this;
  }
  // Ignore up and down keys for a menu bar
  else if (aScancode == KEY_LEFT || aScancode == KEY_DOWN)
  {
     return NULL;
  }
  
  // Otherwise let the menu try and handle it
  return CMenu::HandleKey(aChar,aScancode);
}


void CPopupMenu::Highlight(int buttons, CMenuItem *Item, int x, int y)
{
  Style.HighlightPopup(*this,buttons,Item,x,y,Active);
}


//==================================================


CMenuText::CMenuText(char *aText, cmCommand aCommand)
 : CMenuItem(0, 0, aCommand, ViewAccel(aText))
{
  Text = strdup(aText);
  sy = SystemFont.Height() + 2;
  sx = SystemFont.Length(Text) + 2;
}


CMenuText::~CMenuText()
{
  if (Text!=NULL) free(Text);
}


void* CMenuText::HandleEvent(cmCommand Event, void* Data)
{
  if (Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CMenuText P%p Text=\"%s\" }\n",this,Text);
    AddSerializeInfo(Data,info);
  }
  return CMenuItem::HandleEvent(Event,Data);
}

  
void CMenuText::Draw(BITMAP *Buffer, int x, int y, CColor col)
{
  int SpaceLeft = Owner->GetStyle().SpaceLeft();
  int SpaceTop  = Owner->GetStyle().SpaceTop();

  text_mode(-1);
  ViewTextout(Buffer, SystemFont, Text, x+1+SpaceLeft, y+1+SpaceTop, col);
}


void CMenuText::DefaultScale()
{
  int SpaceLeft  = Owner->GetStyle().SpaceLeft();
  int SpaceRight = Owner->GetStyle().SpaceRight();
  int SpaceTop   = Owner->GetStyle().SpaceTop();
  int SpaceBot   = Owner->GetStyle().SpaceBot();

  sy = SystemFont.Height() + SpaceTop + SpaceBot + 1;
  sx = ViewTextLength(SystemFont, Text) + SpaceLeft + SpaceRight + 1;
}


//=======================================================


CMenuNode::CMenuNode(char *aText)
 : CMenuText(aText, cmNone)
{
  Menu = new CPopupMenu(100, 100, this);
}


CMenuNode::~CMenuNode()
{
  if (Menu!=NULL) delete Menu;
}


void CMenuNode::Insert(CMenuItem *Item)
{
  Menu->Insert(Item);
}


void* CMenuNode::HandleEvent(cmCommand Event, void* Data)
{
  return Menu->HandleEvent(Event,Data);
}

  
void CMenuNode::HandleDown()
{
  int x, y;
  Owner->GetItemPos(this, x, y);
  Menu->MoveTo(x, y+Height());

  Owner->HandleCommand(ExecuteWindow(Menu));
}


void CMenuNode::HandleUp()
{
}


void CMenuNode::HandleLost()
{
}


int CMenuNode::IsIn(int x, int y)
{
  int px, py;
  Owner->GetItemPos(this, px, py);

  return (x>=px && x<px+Width() && y>=py && y<py+Height());
}


//=======================================================


CMenuBar *NewMenuBar(int apx, int apy, int asx, int asy, CView *aTarget, ...)
{
  CMenuBar *Menu = new CMenuBar(apx, apy, asx, asy, aTarget);

  va_list Items;
  va_start(Items, aTarget);

  while (1)
  {
    CMenuItem *Item = va_arg(Items, CMenuItem *);
    if (Item==NULL) break;
    Menu->Insert(Item);
  }

  va_end(Items);

  return Menu;
}


CMenuBar *NewMenuBar(int apx, int apy, int asx, int asy, ...)
{
  CMenuBar *Menu = new CMenuBar(apx, apy, asx, asy);

  va_list Items;
  va_start(Items, asy);

  while (1)
  {
    CMenuItem *Item = va_arg(Items, CMenuItem *);
    if (Item==NULL) break;
    Menu->Insert(Item);
  }

  va_end(Items);

  return Menu;
}


CMenuItem *NewMenuText(char *aText, cmCommand aId)
{
  return new CMenuText(aText, aId);
}


CMenuItem *NewMenu(char *aText, ...)
{
  CMenuNode *Menu = new CMenuNode(aText);

  va_list Items;
  va_start(Items, aText);

  while (1)
  {
    CMenuItem *Item = va_arg(Items, CMenuItem *);
    if (Item==NULL) break;
    Menu->Insert(Item);
  }

  va_end(Items);

  return Menu;
}

