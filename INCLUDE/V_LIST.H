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

 v_list.h - Abstract list class and list box class

*/


#ifndef V_LIST_H_INCLUDED
#define V_LIST_H_INCLUDED

#include "v_scroll.h"


const lstSelect          = 0x0001;
const lstShowSel         = 0x0002;
const lstDefault         = lstShowSel | lstSelect;


class CListItem
{
 public:
  CListItem(int aId, char *aText=NULL);
  virtual ~CListItem();

  virtual int Height();
  virtual int Width();

  virtual void Draw(BITMAP *Buffer, int x, int y);
  char *GetText() { return Text; }
  int GetId() {return Id;}

  class CList *Owner;
  CListItem *Next;

 protected:
  int Id;
  char *Text;
};


class CTextItem:public CListItem
{
 public:
  CTextItem(int aId, char *aText);
  virtual ~CTextItem();

  virtual int Height();
  virtual int Width();

  virtual void Draw(BITMAP *Buffer, int x, int y);

 protected:
  int h;
  int w;
};


class CPicItem:public CListItem
{
 public:
  CPicItem(int aId, BITMAP *aPicture, int aCopy=0);
  CPicItem(int aId, RLE_SPRITE *aPicture);
  virtual ~CPicItem();

  virtual int Height();
  virtual int Width();

  virtual void Draw(BITMAP *Buffer, int x, int y);

 protected:
  int Copy;

  BITMAP *Picture;
};


class CList:public CView
{
 public:
  CList(int apx, int apy, int asx, int asy, CView *aTarget=NULL);
  virtual ~CList();

  virtual void Insert(CListItem *Item);
  virtual void Remove(CListItem *Item);
  void Remove(int aId);
  void RemoveAll();

  int GetSelectedId();
  char *GetSelectedText();
  CListItem *GetSelectedItem();

  void Select(int aId);
  void Select(CListItem *Item);

  CListItem *GetNext(CListItem *Item, int aId);
  CListItem *GetNext(CListItem *Item, char *aText);

  virtual int Compare(CListItem *Item1, CListItem *Item2);
    // Item1<Item2  -> -1
    // Item1==Item2 -> 0
    // Item1>Item2  -> 1

 protected:
  CListItem *First;
  CListItem *Last;

  int Columns;  // 0 = undefined
  int Lines;    // 0 = undefined
  int ActItem;
  int ItemNum;
  int ColWidth;
  int LineHeight;

  CListItem *Selected;
  CView *Target;

  int GetIndex(CListItem* Item);

  int ColumnNum();
  int LineNum();
  void Scroll(int d);
  virtual void ScrollTo(int d);
};


class CListbox:public CList
{
 public:
  CListbox(int apx, int apy, int asx, int asy, int aFlags=lstDefault,
           CView *aTarget=NULL, CScrollbar *aHScroller=NULL,
           CScrollbar *aVScroller=NULL, int aColumns=1, int aLines=0);
  virtual ~CListbox();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  virtual void HandleMouse(int mx, int my, int buttons);
  virtual void* HandleKey(char Char, char Scancode);
  virtual void* HandleAccel(char Char, char Scancode);

  virtual void Draw(int ax, int ay, int asx, int asy);

  void Insert(CListItem *Item);
  void Remove(CListItem *Item);

  virtual void DefaultScale();
  virtual void MoveTo(int x, int y);
  virtual void ScaleTo(int x, int y);

 protected:
  CScrollbar *VScroller, *HScroller;

  int Flags;

  virtual void* ProcessKey(char Char, char Scancode);

  void UpdateScroller();
  virtual void ScrollTo(int d);
  void DrawItems(int ax, int ay, int asx, int asy);
  CListItem *GetItemAt(int ax, int ay);
};


#endif
