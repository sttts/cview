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

 v_sysbit.cc - System bitmap handling by Doug Eleveld

*/


#include "v_sysbit.h"
#include "v_mouse.h"
#include "v_common.h"

#include <allegro.h>


//================================================================


CSystemBitmaps::CSystemBitmaps()
{
  // Indicate that it is empty
  theBitmaps[0] = NULL;
}


CSystemBitmaps::~CSystemBitmaps()
{
  Invalidate();
}


BITMAP* CSystemBitmaps::operator[](bmpBitmap aNum)
{
  return Get(aNum);
}


BITMAP* CSystemBitmaps::Get(bmpBitmap aNum)
{
  // If color depth has changed, we must reload the bitmaps
  static int depth = -1;
  if (Cgfx::Depth()!=depth)
  {
    Invalidate();
    depth = Cgfx::Depth();
  }

  // Possibly setup the system bitmaps
  if (theBitmaps[0]==NULL)
  {
    // Load in the system datafile, keeping everything 8 bit
    set_color_conversion(COLORCONV_NONE);
    DATAFILE* DialogDat = load_datafile("CVIEW.DAT");
    if (!DialogDat) return NULL;
    set_color_conversion(COLORCONV_TOTAL);

    // We must convert palettes if we load into an 8 bit mode
    PALETTE* pal = (PALETTE *)(DialogDat+0)->dat;
    
    // Clear the old bitmaps
    for(unsigned i=1;i<bmpUser;i++)
    {
       // Get the system bitmap
       BITMAP* ret = (BITMAP *)(DialogDat+i)->dat;

       // Convert the bitmap to the current depth using the
       // known palette, preserving any transparency
       theBitmaps[i] = View_FixupBitmap(ret,*pal);
    }
    unload_datafile(DialogDat);

    // Make sure the mouse is setup properly
    DefaultMouse = theBitmaps[bmpMouse];
    View_SetMouse(DefaultMouse);

    // Indicate that we have been setup properly
    theBitmaps[0] = (BITMAP*)1;
  }
  return theBitmaps[aNum];
}


void CSystemBitmaps::Invalidate()
{
  if (theBitmaps[0]!=NULL)
  {
    for(unsigned i=1;i<bmpUser;i++)
      Cgfx::DestroyBitmap(theBitmaps[i]);

    theBitmaps[0] = NULL;
  }
}


void CSystemBitmaps::Reload()
{
  Invalidate();
  Get(bmpMouse);
}


//================================================================


// The global interface to the system bitmaps
CSystemBitmaps CSystemBitmaps::Singleton;
CSystemBitmaps& SystemBitmap(CSystemBitmaps::Singleton);

