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

 v_common.cc - Common helper function

*/


#include <stdio.h>

#include "v_common.h"
#include "v_color.h"
#include "v_gfx.h"
#include "v_string.h"
#include "v_mouse.h"
#include "v_prog.h"
#include "v_sysbit.h"

#include <allegro.h>


//===============================================================


class CView *Focused=NULL;
class CGroup *Bill=NULL; // the ruling object
class CView *Dragged=NULL;
class CProgram *Program=NULL;

int Dragx=0;
int Dragy=0;

int DebugMode=0;
//int DebugMode=1;

// Added RGBMAP for faster 8 bit modes
static RGB_MAP *rgb_table = NULL;


//==========================================================


bool View_Init(int sx, int sy, int depth)
{
  View_SetVideoMode(sx,sy,depth);

  // Set up the graphics stuff
  Cgfx::Init();

  // Reset the default color scheme after the palette is set
  CColorInterface::Default.Reset();

  // Make sure the mouse bitmap gets read in now
  SystemBitmap.Reload();

  if (View_InitStrings("STRINGS.INF"))
    return false;

  return true;
}


void View_Done()
{
  View_DoneStrings();
  
  rgb_map = NULL;
  if(rgb_table!=NULL)
    delete rgb_table;
  rgb_table = NULL;
  
  // Close down the graphics stuff
  Cgfx::Done();
}


void View_SetDebugMode(int aMode)
{
  DebugMode = aMode;
}


static void cleanup_rgbmap (void)
{
  if (rgb_map==rgb_table)
    rgb_map = NULL;

  if (rgb_table!=NULL)
    delete rgb_table;

  rgb_table = NULL;
}


static void create_rgbmap (PALETTE pal)
{
  cleanup_rgbmap();

  // Setup the RGBMAP for fast 8 bit modes
  if (rgb_map==NULL)
  {
    // Make sure the rgbmap gets cleaned up at program exit
    static bool setup = false;
    if (setup==false)
    {
      atexit(cleanup_rgbmap);
      setup = true;
    }

    // Make a new rgbmap
    rgb_table = new RGB_MAP;
    create_rgb_table(rgb_table,pal,NULL);
    rgb_map = rgb_table;
  }
}


bool View_SetVideoMode(int sx, int sy, int depth)
{
  cleanup_rgbmap();

  int oldheight = Cgfx::Height();
  int oldwidth = Cgfx::Width();
  int olddepth = Cgfx::Depth();

  // Setup the RGBMAP for fast 8 bit modes
  PALETTE pal;
  get_palette(pal);
  
  if (depth==8)
    create_rgbmap(pal);
  else
    cleanup_rgbmap();
  
  // Set new graphic mode
  bool res = true;
  set_color_depth(depth);
  if (set_gfx_mode(GFX_AUTODETECT, sx, sy, 0, 0)<0)
  {
    // Didnt work, try the old mode
    set_color_depth(olddepth);
    if (set_gfx_mode(GFX_AUTODETECT, oldwidth, oldheight, 0, 0)<0)
    {
      // Didnt work, try the basic vga mode
      set_color_depth(8);
      set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0);

      // Setup the palette correctly if we were asked for a
      // truecolor mode but only got an 8 bit mode
      if(depth!=8)
      {
        create_rgbmap(pal);
        set_palette(pal);
      }
    }
    res = false;
  }

  // If we are going to an 8 bit mode, then we
  // have to replace the palette
  if (depth==8)
    set_palette(pal);

  // Update the program size etc
  if(Program!=NULL)
    Program->HandleEvent(cmVideoMode);
  
  // Make sure the mouse bitmap gets read in
  // if the color depth has changed
  if (olddepth!=Cgfx::Depth())
    SystemBitmap.Reload();

  // Reset the default color scheme after the palette is set
  CColorInterface::Default.Reset();

  return res;
}


cmCommand ExecuteWindow(class CWindow *Win, int aForeground)
{
  if (Program!=NULL)
  {
    Program->Insert(Win);
    cmCommand Result = Win->Run(aForeground);
    Program->Remove(Win);
    Program->Redraw();
    return Result;
  } else
  {
    return cmNone;
  }
}


void InsertWindow(class CWindow *Win)
{
  if (Program!=NULL)
  {
    Program->Insert(Win);
  }
}


void RemoveWindow(class CWindow *Win)
{
  if (Program!=NULL)
  {
    Program->Remove(Win);
  }
}


void button(BITMAP *Buffer, int px, int py, int sx, int sy,
  CColor lu, CColor rd, CColor in)
{
  emptybutton(Buffer,px,py,sx,sy,lu,rd,in);
  Cgfx::Rectfill(Buffer, px+1, py+1, px+sx-2, py+sy-2, in);
}


void emptybutton(BITMAP *Buffer, int px, int py, int sx, int sy,
  CColor lu, CColor rd, CColor in)
{
  Cgfx::Line(Buffer, px, py, px+sx-1, py, lu);
  Cgfx::Line(Buffer, px, py+1, px, py+sy-2, lu);
  Cgfx::Line(Buffer, px+sx-1, py+1, px+sx-1, py+sy-1, rd);
  Cgfx::Line(Buffer, px+1, py+sy-1, px+sx-1, py+sy-1, rd);

  Cgfx::Pixel(Buffer, px+sx-1, py, in);
  Cgfx::Pixel(Buffer, px, py+sy-1, in);
}

void panel(BITMAP *Buffer, int px, int py, int sx, int sy,
  CColor lu, CColor rd, CColor in)
{
  emptypanel(Buffer,px,py,sx,sy,lu,rd);
  Cgfx::Rectfill(Buffer, px+1, py+1, px+sx-2, py+sy-2, in);
}


void emptypanel(BITMAP *Buffer, int px, int py, int sx, int sy,
  CColor lu, CColor rd)
{
  Cgfx::Line(Buffer, px, py, px+sx-1, py, lu);
  Cgfx::Line(Buffer, px, py+1, px, py+sy-1, lu);
  Cgfx::Line(Buffer, px+sx-1, py+1, px+sx-1, py+sy-1, rd);
  Cgfx::Line(Buffer, px+1, py+sy-1, px+sx-1, py+sy-1, rd);
}


void View_PaletteConvert(BITMAP* bit, RGB* in, bool preserve_trans)
{
  int i, j;

  if(Cgfx::Depth()!=8)
    return;

  for(j=0;j<bit->h;j++)
  {
    for(i=0;i<bit->w;i++)
    {
      int index = bit->line[j][i];
      
      bit->line[j][i] = makecol8((in[index].r)<<2,
                                 (in[index].g)<<2,
                                 (in[index].b)<<2);
                                 
      if (index==0 && preserve_trans)
        bit->line[j][i] = 0;
    }
  }
}


// Adjust the bitmap to that it looks correct given
// the current color depth and palette
BITMAP* View_FixupBitmap(BITMAP* bmp, RGB* pal)
{
  // Make a transparant bitmap in the correct depth
  BITMAP* ret = Cgfx::CreateBitmap(bmp->w,bmp->h);
  clear_to_color(ret,bitmap_mask_color(ret));

  // Source bitmap is 8 bit
  if (bitmap_color_depth(bmp)==8)
  {
    if (Cgfx::Depth()==8 && bitmap_color_depth(bmp)==8)
    {
      View_PaletteConvert(bmp,pal);

      // Copy the bitmap over the clear background
      draw_sprite(ret,bmp,0,0);
    }
    else
    {
      // Copy the bitmap over the clear background
      // using the bitmap, and preserving transparency
      for(int j=0;j<bmp->h;j++)
      {
        for(int i=0;i<bmp->w;i++)
        {
          int index = bmp->line[j][i];

          if(index!=0)
          {
            int r = (pal[index].r);
            int g = (pal[index].g);
            int b = (pal[index].b);

            index = bestfit_color(pal,r,g,b);

            r = (pal[index].r)<<2;
            g = (pal[index].g)<<2;
            b = (pal[index].b)<<2;

            putpixel(ret,i,j,makecol(r,g,b));
          }
        }
      }
    }
  }
  // Source bitmap is not 8 bit
  else
  {
    // Copy the bitmap over the clear background
    draw_sprite(ret,bmp,0,0);
  }
  return ret;
}


// Scaling operations
sclScaling operator|(const sclScaling one, const sclScaling two)
{
  return (sclScaling)((int)one | (int)two);
}


// Return a pointer to the Accel of a string
char ViewAccel (char *string)
{
  char ans = 0;

  char *ptr = ViewAccelMarker(string);

  if (ptr)
    ans = *ptr;

  return ans;
}


// Return a pointer to the Accel of a string
char* ViewAccelMarker (char *string)
{
  char *ptr = string;

  // Go forward looking for an & character
  while(*ptr && *ptr!='&')
  {
    ptr++;
  }

  // Are we at the end of the real string
  if (*ptr)
  {
    ptr++;

    // Check if & was last character or not
    if (*ptr)
      return ptr;
    else
      return NULL;
  }
  else
  {
    return NULL;
  }
}


// Write a string to the screen using gui & character marking
int ViewTextout (BITMAP *bmp, Cgfx::Font& font, char *s, int x, int y, CColor color, bool underline)
{
  char temp[3] = "_";

  int more = FALSE;
  char *ptr = ViewAccelMarker(s);

  // Are we at the end of the real string
  if (ptr && *ptr)
  {
    ptr--;
    more = TRUE;
    *ptr = '\0';
  }

  // Figure out the length of the initial text
  int xx = x + font.Length(s);

  // Set a fake end and draw until that
  if (bmp)
    Cgfx::Text(bmp,font,s,x,y,color);

  // Make sure that the & is not the last character in the string
  if (more)
    {
    // Put the & marker back
    *ptr = '&';

    ptr++;
    if (*ptr)
    {
      // Draw the second half text
      if (bmp)
      {
        Cgfx::Text(bmp,font,ptr,xx,y,color);

        // Underline the char after the & character
        if(underline && *ptr!='&')
        {
          temp[0] = *ptr;
	  // -2 because the char includes the one column space
          Cgfx::HLine(bmp,xx,y+font.Height()-1,xx+font.Length(temp)-2,color);
        }
      }
      // Find the extra length
      xx += font.Length(ptr);
    }
  }
  // Return the length in pixels
  return xx - x;
}


// Find the length in pixels of text string, properly handling Accels
int ViewTextLength (Cgfx::Font& font, char *s)
{
  return ViewTextout (NULL,font,s,0,0,(CColor)0,0);
}



