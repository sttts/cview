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

 v_combo.h - Combo box class

*/


#ifndef V_COMBO_H_INCLUDED
#define V_COMBO_H_INCLUDED

//#include "view.h"
#include "v_base.h"
#include "v_list.h"
#include "v_input.h"
//#include "v_common.h"



const comboEditable       = 0x0001;
const comboPopup          = 0x0002;
const comboDefault        = comboEditable;


class CCombobox:public CGroup
{
 public:
  CCombobox(int apx, int apy, int asx, int asy,
            char *aContent, int aFlags=comboDefault);
  ~CCombobox();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);

  void Insert(CListItem *Item);
  void Insert(char *aText, int aId=0);
  void RemoveItems();

  char *GetInput();
  int GetSelectedId();

  void Select(int aId);
  void Select(char *aText);

  virtual void DefaultScale();

 protected:
  int Flags;
  int Selected;
  CListbox *List;
  CScrollbar *Scroller;
  CInputLine *Input;
  int lx, ly; // List size
  class CPopupWindow *Popup;

  void ShowList();
};


#endif

