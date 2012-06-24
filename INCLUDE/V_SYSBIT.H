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


#ifndef V_SYSBIT_H_INCLUDED
#define V_SYSBIT_H_INCLUDED


#include "v_gfx.h"
#include "v_const.h"


//================================================================


// System Bitmap constants
enum bmpBitmap { bmpMouse = 1,  // This MUST begin a 1 and not 0
                 bmpCrossOut,
                 bmpCrossIn,
                 bmpArrowUpOut,
                 bmpArrowUpIn,
                 bmpArrowRightOut,
                 bmpArrowRightIn,
                 bmpArrowDownOut,
                 bmpArrowDownIn,
                 bmpArrowLeftOut,
                 bmpArrowLeftIn,
                 bmpQuestion,
                 bmpExclamation,
                 bmpInformation,
                 bmpRadioOff,
                 bmpRadioOn,
                 bmpCheckOff,
                 bmpCheckOn,
                 bmpFolder,
                 bmpTrashcan,
                 bmpDocument,
                 bmpDisk,
                 bmpHarddisk,
                 bmpDirUp,
                 bmpDelFile,
                 bmpResizeVert,
                 bmpResizeHor,
                 bmpResizeDiag,
                 bmpComboOut,
                 bmpComboIn,
                 bmpInValid,
                 bmpMouseMacroRecord,
                 bmpUser,
                 bmpLast  = 0xffffffff
               };


class CSystemBitmaps
{
 public:
  // User access to the individual bitmaps
  BITMAP* operator[](bmpBitmap index);
  BITMAP* Get(bmpBitmap index);

  // Bitmap loading control
  void Invalidate();
  void Reload();

  static CSystemBitmaps Singleton;

  // This is a bug (DJGPP not up to standard) where
  // this should be able to be private and accessed by it's friends
  // but this does not work for static instantiation
  ~CSystemBitmaps();

 private:
  BITMAP* theBitmaps[bmpUser];

  CSystemBitmaps();
  CSystemBitmaps(const CSystemBitmaps&); // Unimplemented
  void operator=(const CSystemBitmaps&); // Unimplemented
  
  // This just tricks the compiler from giving a
  // warning about private constructor and no friends
  friend class stuff;
  class stuff { };
};


//================================================================

// Another name for the actual system bitmap controller
extern CSystemBitmaps& SystemBitmap;


#endif
