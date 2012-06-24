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

 v_menust.h - Flat-box style menu drawing style class
              by Doug Eleveld

*/


#include "v_menust.h"
#include "v_menu.h"
#include "v_common.h"


//================================================================


void CMenuStyleBox::Back(CMenu& aTarget)
{
  panel(aTarget.GetBuffer(), 0, 0, aTarget.Width(), aTarget.Height(),
        aTarget.Color(colDark),aTarget.Color(colDark),aTarget.Color(colBack));
}


void CMenuStyleBox::HighlightBar(CMenu& aTarget, int buttons, CMenuItem *Item, int x, int y, CMenuItem *Active)
{
  (void)buttons;
  
  if(Item==Active)
  {
    panel(aTarget.GetBuffer(), x+1, y+1, Item->Width()-2, Item->Height()-2,
          aTarget.Color(colSelect),aTarget.Color(colSelect),aTarget.Color(colSelect));
          
    Item->Draw(aTarget.GetBuffer(), x, y, aTarget.Color(colLight));
    return;
  }
  // Draw the actual menu item
  Item->Draw(aTarget.GetBuffer(), x, y, aTarget.Color(colFore));
}


void CMenuStyleBox::HighlightPopup(CMenu& aTarget, int buttons, CMenuItem *Item, int x, int y, CMenuItem *Active)
{
  if(Item==Active)
  {
    if (buttons)
      panel(aTarget.GetBuffer(), x, y, Item->Width(), Item->Height(),
            aTarget.Color(colLight),aTarget.Color(colLight),aTarget.Color(colSelect));
    else
      panel(aTarget.GetBuffer(), x+1, y+1, Item->Width()-2, Item->Height()-2,
            aTarget.Color(colSelect),aTarget.Color(colSelect),aTarget.Color(colSelect));

    Item->Draw(aTarget.GetBuffer(), x, y, aTarget.Color(colLight));
    return;
  }
  
  // Draw the actual menu item
  Item->Draw(aTarget.GetBuffer(), x, y, aTarget.Color(colFore));
}



