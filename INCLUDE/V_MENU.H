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

 v_menu.h - Menu classes

*/


#ifndef V_MENU_H_INCLUDED
#define V_MENU_H_INCLUDED


#include "v_const.h"
#include "v_base.h"
#include "v_menust.h"


class CMenuItem
{
 public:
  CMenuItem(int asx, int asy, cmCommand aCommand, int aAccelKey = 0);
  virtual ~CMenuItem();

  virtual void Draw(BITMAP *Buffer, int x, int y, CColor col) = 0;

  virtual void* HandleEvent(cmCommand Event, void* Data);
  virtual void HandleDown();
  virtual void HandleUp();
  virtual void HandleLost();
  virtual void HandleCommand();

  int Width() { return sx; }
  int Height() { return sy; }

  // Functions to help sizing
  virtual void DefaultScale();

 protected:
  friend class CMenu;
  friend class CMenuBar;
  friend class CPopupMenu;

  int AccelKey;
  cmCommand Command;
  int sx;
  int sy;
  CMenuItem *Next;
  class CMenu *Owner;
};


class CMenu:public CWindow
{
 public:
  CMenu(int apx, int apy, int asx, int asy, int aVertical, CView *aTarget=NULL);
  virtual ~CMenu();

  virtual void Draw(int ax, int ay, int asx, int asy);

  virtual void* HandleEvent(cmCommand Event, void* Data);
  virtual void HandleMouse(int mx, int my, int buttons);
  virtual void MouseLost(int mx, int my, int buttons);
  virtual void* HandleKey(char aChar, char aScancode);
  virtual void* HandleAccel(char aChar, char aScancode);

  void Insert(CMenuItem *Item);
  void Remove(CMenuItem *Item);

  void GetItemPos(CMenuItem *Item, int &x, int &y);
  virtual void HandleCommand(cmCommand aCommand);

  void SetStyle(CMenuStyle::rep& newStyle);
  CMenuStyle& GetStyle();

 protected:
  CMenuItem *ItemAt(int x, int y);
  void Next(CMenuItem *Last, int &x, int &y);
  virtual void Highlight(int buttons, CMenuItem *Item, int x, int y) = 0;

  void UpdateItem(CMenuItem* Item);

  void AdjustSize();

  CMenuItem *Base;
  CMenuItem *Active;
  
  CMenuStyle Style;
  
 private:
  CMenuItem *Head;
  int Vertical;
  CView *Target;
};


class CMenuBar:public CMenu
{
 public:
  CMenuBar(int apx, int apy, int asx, int asy, CView *aTarget=NULL);
  virtual ~CMenuBar();

  virtual void* HandleEvent(cmCommand Event, void* Data);
  virtual void HandleMouse(int mx, int my, int buttons);
  virtual void* HandleKey(char aChar, char aScancode);

 protected:
  virtual void Highlight(int buttons, CMenuItem *Item, int x, int y);
};


class CPopupMenu:public CMenu
{
 public:
  CPopupMenu(int apx, int apy, class CMenuNode *aNode=NULL, CView *aTarget=NULL);
  virtual ~CPopupMenu();

  virtual void* HandleEvent(cmCommand Event, void* Data);
  virtual void HandleMouse(int mx, int my, int buttons);
  virtual void* HandleKey(char aChar, char aScancode);

 protected:
  virtual void Highlight(int buttons, CMenuItem *Item, int x, int y);

  class CMenuNode *Node;
  int Shown;
};


class CMenuText:public CMenuItem
{
 public:
  CMenuText(char *aText, cmCommand aCommand);
  virtual ~CMenuText();

  virtual void* HandleEvent(cmCommand Event, void* Data);
  virtual void Draw(BITMAP *Buffer, int x, int y, CColor col);

  // Functions to help sizing
  virtual void DefaultScale();

 protected:
  char *Text;
};


class CMenuNode:public CMenuText
{
 public:
  CMenuNode(char *aText);
  virtual ~CMenuNode();

  void Insert(CMenuItem *Item);

 protected:
  virtual void* HandleEvent(cmCommand Event, void* Data);
  virtual void HandleDown();
  virtual void HandleUp();
  virtual void HandleLost();

  friend CPopupMenu;
  int IsIn(int x, int y);

  CPopupMenu *Menu;
};


CMenuBar *NewMenuBar(int apx, int apy, int asx, int asy, CView *aTarget, ...);
CMenuBar *NewMenuBar(int apx, int apy, int asx, int asy, ...);
CMenuItem *NewMenuText(char *aText, cmCommand aId);
CMenuItem *NewMenu(char *aText, ...);


#endif
