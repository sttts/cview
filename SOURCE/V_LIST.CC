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

 v_list.cc - Abstract list class and list box class

*/


#include <string.h>
#include "v_list.h"
#include "v_common.h"
#include "v_rect.h"
#include "v_gfx.h"
#include "v_system.h"
#include "v_save.h"

#include <allegro.h>


//================================================================


CListItem::CListItem(int aId, char *aText)
{
  Owner = NULL;
  Next = NULL;
  if (aText) Text = strdup(aText); else Text = strdup("");
  Id = aId;
}


CListItem::~CListItem()
{
  if (Owner!=NULL) Owner->Remove(this);
  if (Text!=NULL) free(Text);
}


int CListItem::Height()
{
  return 0;
}


int CListItem::Width()
{
  return 0;
}


void CListItem::Draw(BITMAP *Buffer, int x, int y)
{
  (void)Buffer;
  (void)x;
  (void)y;
}


//===================================================


CTextItem::CTextItem(int aId, char *aText)
 : CListItem(aId, aText)
{
  h = SystemFont.Height() + 2;
  w = SystemFont.Length(Text) + 2;
}


CTextItem::~CTextItem()
{
}


int CTextItem::Height()
{
  return h;
}


int CTextItem::Width()
{
  return w;
}


void CTextItem::Draw(BITMAP *Buffer, int x, int y)
{
  Cgfx::Text(Buffer, SystemFont, Text,
          x+1, y+1, Owner->Color(colFore));
}


//===================================================


CPicItem::CPicItem(int aId, BITMAP *aPicture, int aCopy)
 : CListItem(aId)
{
  Copy = aCopy;

  if (Copy)
  {
    Picture = Cgfx::CreateBitmap(aPicture->w, aPicture->h);
    clear(Picture);
    draw_sprite(Picture, aPicture, 0, 0);
  } else
  {
    Picture = aPicture;
  }
}


CPicItem::CPicItem(int aId, RLE_SPRITE *aPicture)
 : CListItem(aId)
{
  Copy = 1;
  Picture = Cgfx::CreateBitmap(aPicture->w, aPicture->h);
  clear(Picture);
  draw_rle_sprite(Picture, aPicture, 0, 0);
}


CPicItem::~CPicItem()
{
  if (Copy) Cgfx::DestroyBitmap(Picture);
}


int CPicItem::Height()
{
  return Picture->h + 2;
}


int CPicItem::Width()
{
  return Picture->w + 2;
}


void CPicItem::Draw(BITMAP *Buffer, int x, int y)
{
  draw_sprite(Buffer, Picture, x+1, y+1);
}


//===================================================


CList::CList(int apx, int apy, int asx, int asy, CView *aTarget)
 : CView(apx, apy, asx, asy)
{
  First = NULL;
  Last = NULL;
  Selected = NULL;
  Target = aTarget;

  ItemNum = 0;
  ActItem = 0;
  ColWidth = 0;
  LineHeight = 0;
  Lines = 0;
  Columns = 0;
}


CList::~CList()
{
  RemoveAll();
}


void CList::Insert(CListItem *Item)
{
  // Search position
  CListItem *Act=First;

  if (Act && Compare(Act, Item)>0)
  {
    Item->Next = First;
    First = Item;
  } else
  {
    if (Act)
      while (Act->Next && Compare(Act->Next, Item)<=0) Act=Act->Next;

    if (Act==NULL)
    {
      if (Last!=NULL)
      {
        Last->Next = Item;
        Last = Item;
      } else
      {
        Last = Item;
        First = Item;
      }

      Item->Next = NULL;
    } else
    {
      if (Act->Next==0) Last=Item;
      Item->Next = Act->Next;
      Act->Next = Item;
    }
  }

  Item->Owner = this;

  ItemNum++;

  int Width = Item->Width();
  if (Width>ColWidth) ColWidth = Width;

  int Height = Item->Height();
  if (Height>LineHeight) LineHeight = Height;
}


void CList::Remove(CListItem *Item)
{
  if (First==Item)
  {
    First = Item->Next;
    if (Last==Item) Last=NULL;
  } else
  {
    CListItem *Prev = First;
    while (Prev!=NULL && Prev->Next!=Item)
    {
      Prev = Prev->Next;
    }

    if (Prev!=NULL)
    {
      Prev->Next = Item->Next;
      if (Last==Item) Last = Prev;
    }
  }

  if (Selected==Item) Selected=NULL;

  Item->Next = NULL;
  Item->Owner = NULL;

  ItemNum--;
}


void CList::Remove(int aId)
{
  CListItem *Item = First;

  while (Item!=NULL)
  {
    CListItem *NextItem = Item->Next;
    if (Item->GetId()==aId) Remove(Item);
    Item = NextItem;
  }
}


void CList::RemoveAll()
{
  CListItem *Act = First;
  while (Act!=NULL)
  {
    CListItem *Next = Act->Next;
    delete Act;
    Act = Next;
  }

  First = NULL;
  Last = NULL;
  Selected = NULL;

  ItemNum = 0;
  ActItem = 0;
  ColWidth = 0;
  LineHeight = 0;
}


int CList::GetSelectedId()
{
  if (Selected!=NULL) return Selected->GetId();
    else return -1;
}


CListItem *CList::GetSelectedItem()
{
  return Selected;
}


char *CList::GetSelectedText()
{
  if (Selected!=NULL) return Selected->GetText();
    else return NULL;
}


CListItem *CList::GetNext(CListItem *Item, int aId)
{
  if (Item==NULL) Item=First;
  while (Item!=NULL && Item->GetId()!=aId) Item=Item->Next;
  return Item;
}


CListItem *CList::GetNext(CListItem *Item, char *aText)
{
  if (Item==NULL) Item=First;
  while (Item!=NULL && strcmp(Item->GetText(), aText)!=0) Item=Item->Next;
  return Item;
}


void CList::Select(int aId)
{
  CListItem *Item = GetNext(NULL, aId);
  if (Item!=NULL) Select(Item);
}


void CList::Select(CListItem *Item)
{
  Selected=Item;

  Redraw();
}


void CList::Scroll(int d)
{
  ScrollTo(ActItem + d);
}


void CList::ScrollTo(int d)
{
  int NewItem = d;

  if (NewItem<0) NewItem=0;
  if (NewItem>=ItemNum) NewItem=ItemNum-1;

  ActItem = NewItem;
}


int CList::ColumnNum()
{
  if (Columns!=0) return Columns;

  if (ColWidth>0)
  {
    int n = sx/ColWidth;
    if (n==0) n=1;
    return n;
  } else return 1;
}


int CList::LineNum()
{
  if (Lines!=0) return Lines;

  if (LineHeight>0)
    return (sy+LineHeight-1)/LineHeight;
  else
    return 1;
}


int CList::Compare(CListItem *Item1, CListItem *Item2)
{
  (void)Item1;
  (void)Item2;

  return -1;
}


int CList::GetIndex(CListItem *Item)
{
  int ret = 0;

  CListItem* ptr=First;
  while (ptr!=NULL && ptr!=Item)
  {
    ptr = ptr->Next;
    ++ret;
  }
  return ret;
}


//===================================================


CListbox::CListbox(int apx, int apy, int asx, int asy, int aFlags, CView *aTarget,
                   CScrollbar *aHScroller, CScrollbar *aVScroller,
                   int aColumns, int aLines)
 : CList(apx, apy, asx, asy, aTarget)
{
  Columns = aColumns;
  Lines = aLines;
  Flags = aFlags;

  VScroller = aVScroller;
  HScroller = aHScroller;
  if (HScroller!=NULL) HScroller->SetTarget(this);
  if (VScroller!=NULL) VScroller->SetTarget(this);

  UpdateScroller();
}


CListbox::~CListbox()
{
}


void* CListbox::HandleEvent(cmCommand Event, void* Data)
{
  switch (Event)
  {
    case cmScrolled:
    {
       CScrollbar *s = (CScrollbar *)Data;

       if (s == VScroller)
       {
         ScrollTo(s->GetNewPos()*ColumnNum());
         UpdateScroller();
         Redraw();
       }
       else if (s == HScroller)
       {
         ScrollTo(s->GetNewPos());
         UpdateScroller();
         Redraw();
       }
       break;
    }
    case cmSerialize:
    {
      char info[1000];
      sprintf(info,"{ CListbox P%p ",this);
      AddSerializeInfo(Data,info);

      CListItem *Act = First;
      while (Act!=NULL)
      {
        CListItem *Next = Act->Next;
        
        sprintf(info," ListItem=\"%s\" ",Act->GetText());
        AddSerializeInfo(Data,info);
        
        Act = Next;
      }
      AddSerializeInfo(Data,"}\n");
      
      break;
    }
    
    default: { };
  }

  return CView::HandleEvent(Event, Data);
}


void CListbox::HandleMouse(int mx, int my, int buttons)
{
  if (Oldbuttons==1 && buttons==0)
  {
    CListItem *Item = GetItemAt(mx-2-px, my-2-py);
    if (Flags & lstSelect)
    {
      if (Item!=NULL)
      {
        CListItem *Current = GetSelectedItem();

        // A new selection
        if(Current!=Item)
        {
          Select(Item);

          if (Target!=NULL)
            Target->HandleEvent(cmListSelect, this);
          else
            Owner->HandleEvent(cmListSelect, this);
        }
        // Second click on a selection
        else
        {
          if (Target!=NULL)
            Target->HandleEvent(cmListSelectDbl, this);
          else
            Owner->HandleEvent(cmListSelectDbl, this);
        }
      }
    }

    Redraw();
  }

  CView::HandleMouse(mx, my, buttons);
}


void* CListbox::HandleKey(char Char, char Scancode)
{
  return ProcessKey(Char,Scancode);
}


void* CListbox::HandleAccel(char Char, char Scancode)
{
  return ProcessKey(Char,Scancode);
}


void CListbox::Draw(int ax, int ay, int asx, int asy)
{
  if (Owner!=NULL)
  {
    BITMAP *Buffer = Owner->GetBuffer();

//    button(Buffer, px, py, sx, sy, Color(colDark), Color(colLight), Color(colNormal));
    emptybutton(Buffer, px, py, sx, sy, Color(colDark), Color(colLight), Color(colBack));

    DrawItems(ax, ay, asx, asy);
  }

  UpdateScroller();
}


void CListbox::Insert(CListItem *Item)
{
  CList::Insert(Item);
  UpdateScroller();
}


void CListbox::Remove(CListItem *Item)
{
  CList::Remove(Item);
  UpdateScroller();
}


void CListbox::UpdateScroller()
{
  if (VScroller!=NULL)
  {
//    int Cols = ColumnNum();

    // Adjust the limits of the scrollbar //DE
    VScroller->Update(ActItem, ItemNum-LineNum()+1, LineNum());
//    VScroller->Update(ActItem, ItemNum-1, LineNum());
  }

  if (HScroller!=NULL)
  {
    HScroller->Update(ActItem, ItemNum-1, LineNum()*ColumnNum());
  }
}


void CListbox::ScrollTo(int d)
{
  CList::ScrollTo(d);
}


CListItem *CListbox::GetItemAt(int ax, int ay)
{
  int x = 1;
  int y = 1;
  int Col = 0;
  int Line = 0;
  int w = sx-2;
  int h = sy-2;

  CListItem *Act = First;

  for (int z=0; z<ActItem && Act!=NULL; z++) Act=Act->Next;

  while (Act!=NULL)
  {
    // Max. lines reached
    if (Lines>0 && Line>=Lines) break;
    if (y > h) break;
    
//    if (ax>=x && ax<x+ColWidth &&
//        ay>=y && ay<y+LineHeight) return Act;
    if (ax>=x && ay>=y && ay<y+LineHeight) return Act;

    // Next item
    x += ColWidth;
    Col++;

    // Next line if needed
    if ((Columns>0 && Col>=Columns) ||
        (Columns==0 && Lines!=1 && x+ColWidth>w))
    {
      Col = 0;
      x = 1;
      y += LineHeight;

      Line++;
    }

    // Next item
    Act = Act->Next;
  }

  return NULL;
}



void CListbox::DrawItems(int ax, int ay, int asx, int asy)
{
  int x = 1;
  int y = 1;
  int Col = 0;
  int Line = 0;
  int w = sx-2;
  int h = sy-2;

  CColor backcol;

  BITMAP *Buffer = Owner->GetBuffer();
  if (Owner==NULL) return;

  // Set clipping to inner rect
  // TODO: make general function for rectangle clipping
/*  if (ax<px+1) { asx -= px+1-ax; ax = px+1; } // clip left
  if (ay<py+1) { asy -= py+1-ay; ay = py+1; } // clip top
  if (ax+asx>px+sx-2) asx += px+sx-2-ax-asx; // clip right
  if (ay+asy>py+sy-2) asy += py+sy-2-ay-asy; // clip bottom
  if (asx>sx-2) asx=sx-2;
  if (asy>sy-2) asy=sy-2;
  if (asx<=0 || asy<=0) return;
  set_clip(Buffer, ax, ay, ax+asx-1, ay+asy-1);*/

  CRect Win(px+1, py+1, sx-2, sy-2);
  CRect DrawRect(ax, ay, asx, asy);
  CRect Clipped(Win & DrawRect);
  if (!Clipped.Valid()) return;
  set_clip(Buffer, Clipped.p.x, Clipped.p.y,
                   Clipped.p.x+Clipped.s.w-1, Clipped.p.y+Clipped.s.h-1);

  CListItem *Act=First;
  CListItem *LastDrawn=NULL;

  for (int z=0; z<ActItem && Act!=NULL; z++) Act=Act->Next;

  while (Act!=NULL)
  {
    // Max. lines reached
    if (Lines>0 && Line>=Lines) break;
    if (y > h) break;

    // Show selection
    if ((Flags & lstShowSel) && Act==Selected)
      backcol = Color(colSelect);
    else
      backcol = Color(colBack);

    // Draw item
    Cgfx::Rectfill(Buffer, px+1, py+y, px+sx-2, py+y+Act->Height()-1, backcol);
    text_mode(backcol);
    Act->Draw(Buffer, px+x, py+y);
    LastDrawn = Act;

    // Show selection
//    if ((Flags & lstShowSel) && Act==Selected)
//      rect(Buffer, px+x, py+y,
//        px+x+Act->Width()-1, py+y+Act->Height()-1, Color(colBlack));

    // Next item
    x += ColWidth;
    Col++;

    // Next line if needed
    if ((Columns>0 && Col>=Columns) ||
        (Columns==0 && Lines!=1 && x+ColWidth>w))
    {
      Col = 0;
      x = 1;

      y += LineHeight;

      Line++;
    }

    // Next item
    Act = Act->Next;
  }

  // Make sure we drew the whole area
  if (y<=h)
  {
    Cgfx::Rectfill(Buffer, px+1, py+y, px+sx-2, py+sy, Color(colBack));
  }
  // Make sure that the selected object is not obscured
  else if (LastDrawn==Selected)
  {
    Scroll(1);
    DrawItems(ax,ay,asx,asy);
  }

  // Reset any changes to the system
  text_mode(-1);
  set_clip(Buffer, 0, 0, Buffer->w-1, Buffer->h-1);
}


void CListbox::MoveTo(int x, int y)
{
  CList::MoveTo(x,y);
  UpdateScroller();
}


void CListbox::ScaleTo(int x, int y)
{
  CList::ScaleTo(x,y);
  UpdateScroller();
}


void CListbox::DefaultScale()
{
  CList::ScaleTo(100,100);
  UpdateScroller();
}


void* CListbox::ProcessKey(char Char, char aScancode)
{
//  CListItem *oldSelected = Selected;

  // Check for enter or space to select the active element
  if (aScancode==KEY_ENTER || aScancode==KEY_SPACE)
  {
    if (Selected!=NULL)
    {
      // Second click on a selection
      if (Target!=NULL)
        Target->HandleEvent(cmListSelectDbl, this);
      else
        Owner->HandleEvent(cmListSelectDbl, this);
        
//      Redraw();
      return this;
    }
  }
  // Check for up and down arrows to move selection
  else if (aScancode == KEY_DOWN)
  {
    // Go to the next list item
    if (Selected!=NULL)
    {
      CListItem *Next = Selected->Next;

      if (Next!=NULL)
      {
        Select(Next);
        UpdateScroller();
        Redraw();
      }
    }
    // We will go to the first item
    else
    {
      if (First!=NULL)
      {
        Select(First);
        Redraw();
      }
    }
    return this;
  }
  else if (aScancode == KEY_UP)
  {
    if (Selected!=NULL && Selected!=First)
    {
      CListItem* Item=First;
      while (Item!=NULL)
      {
        if (Item->Next==Selected)
        {
          // Select the upwards item
          Select(Item);
          Redraw();

          // Possibly scroll upwards
          int dist = GetIndex(Item) - ActItem;
          if(dist<0)
          {
            UpdateScroller();
            Scroll(dist);
            Redraw();
          }

          // Stop looking
          Item = NULL;
          return this;
        }
        else
          Item=Item->Next;
      }
    }
  }
    
  // We did nothing with the key
  return NULL;
}




