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

 v_menust.h - Various menu drawing style classes
              by Doug Eleveld

*/


#include "v_menust.h"
#include "v_menu.h"
#include "v_common.h"


//================================================================


// The default border style
CMenuStyle CMenuStyle::Default;

// The default border drawer
CMenuStyle::CDefault CMenuStyle::DefaultDrawer;


void CMenuStyle::CDefault::Back(CMenu& aTarget)
{
  button(aTarget.GetBuffer(), 0, 0, aTarget.Width(), aTarget.Height(),
         aTarget.Color(colLight), aTarget.Color(colDark), aTarget.Color(colBack));
}


void CMenuStyle::CDefault::HighlightBar(CMenu& aTarget, int buttons, CMenuItem *Item, int x, int y, CMenuItem *Active)
{
  if(Item==Active)
  {
    if (buttons)
      button(aTarget.GetBuffer(), x, y, Item->Width(), Item->Height(),
             aTarget.Color(colDark), aTarget.Color(colLight), aTarget.Color(colSelect));
    else
      button(aTarget.GetBuffer(), x, y, Item->Width(), Item->Height(),
             aTarget.Color(colLight), aTarget.Color(colDark), aTarget.Color(colSelect));
  }
  // Draw the actual menu item
  Item->Draw(aTarget.GetBuffer(), x, y, aTarget.Color(colFore));
}


void CMenuStyle::CDefault::HighlightPopup(CMenu& aTarget, int buttons, CMenuItem *Item, int x, int y, CMenuItem *Active)
{
  if(Item==Active)
  {
    if (buttons)
      button(aTarget.GetBuffer(), x, y, Item->Width(), Item->Height(),
             aTarget.Color(colLight), aTarget.Color(colDark), aTarget.Color(colSelect));
    else
    {
      emptybutton(aTarget.GetBuffer(), x+1, y, Item->Width()-2, Item->Height(),
                  aTarget.Color(colDark), aTarget.Color(colLight), aTarget.Color(colBack));
      button(aTarget.GetBuffer(), x+2, y+1, Item->Width()-4, Item->Height()-2,
             aTarget.Color(colLight), aTarget.Color(colDark), aTarget.Color(colBack));
    }
  }
  // Draw the actual menu item
  Item->Draw(aTarget.GetBuffer(), x, y, aTarget.Color(colFore));
}



