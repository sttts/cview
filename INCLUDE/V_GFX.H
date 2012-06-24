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

 v_gfx.h - Graphics interface functions

*/


#ifndef V_GFX_H_INCLUDED
#define V_GFX_H_INCLUDED


#include <values.h>
//#include "zfortify.h"


//#include <allegro.h>
struct RGB;
struct BITMAP;
struct FONT;
struct RLE_SPRITE;


// Opaque color handling type
enum CColor { colLow = -MAXINT,
              colHigh = MAXINT
            };
            

// This class is only used like a namespace to contain
// the graphic lib functions so we can reduce name pollution
class Cgfx
{
 public:
   // Publicly visible flags
   static const int fDestroy = 1;
   static const int fSmoothed = 1<<1;

 public:
   // Mode setting variables and functions
   static char *Modes[];
   static char *Depths[];
   
 public:
   // initialization and deinitialization routines
   static void Init();
   static void Done();

   // Access to mode changing and info
   static bool Mode(const char* aMode, const char* aDepth);
   static bool Mode(int w, int h, int d);
   static void TextMode();

   static int Width();
   static int Height();
   static int Depth();

   // Wrapper for Font access
   class Font
   {
     private:
      FONT* Data; // Opaque pointer to system specific font data
      int Flags;

      void ensure();
      void destroy();

     public:
      // Constructors and destructors
      Font();
      ~Font();

      // Load a font from a file
      void Load(const char*, int aPoints, int aFlags);

      // Find the pixel sizes
      int Length(const char*);
      int Height();
      
     // Let the static drawing functions access the font data
     friend class Cgfx;
   };

   static BITMAP* CreateBitmap(int w, int h);
   static BITMAP* CreateBitmap(BITMAP*, int x, int y, int w, int h);
   static void DestroyBitmap(BITMAP* bit);

   // Wrapper for bitmap Access
   class Bitmap
   {
     private:
      BITMAP* Data;

     public:
      // Constructors and destructors
      Bitmap();
      Bitmap(int w, int h);
      ~Bitmap();

      // Load a font from a file
      void Load(const char*);

      // Make it a subbitmap of another bitmap
      void Sub(Bitmap& Bit, int x, int y, int w, int h);

      // Find the pixel sizes
      int Width();
      int Height();
      
     // Let the static drawing functions access the bitmap data
     friend class Cgfx;
   };

   // Text drawing related functions
   static void Text(BITMAP *bmp, const Font& fon, char *string, int x, int y, CColor color);
   static void TextCenter(BITMAP *bmp, const Font& fon, char *string, int x, int y, CColor color);

   // Line and rectangle drawing related functions
   static void VLine(BITMAP *bmp, int x, int y1, int y2, CColor color);
   static void HLine(BITMAP *bmp, int x1, int y, int x2, CColor color);
   static void Line(BITMAP *bmp, int x1, int y1, int x2, int y2, CColor color);
   
   static void Rect(BITMAP *bmp, int x1, int y1, int x2, int y2, CColor color);
   static void Rectfill(BITMAP *bmp, int x1, int y1, int x2, int y2, CColor color);

   // Pixel related functions
   static void Pixel(BITMAP *bmp, int x, int y, CColor color);

   // Color related functions
   static CColor MakeColor (int, int, int);
   
};

#endif
