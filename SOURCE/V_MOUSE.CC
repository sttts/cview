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

 v_mouse.cc - Mouse cursor management

*/


#include <stdlib.h>
#include "v_mouse.h"
#include "v_common.h"
#include "v_sysbit.h"

#include <allegro.h>


//=====================================================


int DoubleClickTime=8;
int DoubleClickDist=3;
int NewMouse=0;
BITMAP *DefaultMouse=NULL;

struct
{
  // Information for current tick
  BITMAP *Bitmap;
  int Focusx;
  int Focusy;
  int Actx;
  int Acty;
  int w;
  int h;

  // Information to restore background
  BITMAP *Back;
  BITMAP *Back2;
  int Oldx;
  int Oldy;

  // Information for next tick
  BITMAP *NextBitmap;
} Mouse = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


//=====================================================


int View_GetDblClickTime()
{
  return DoubleClickTime;
}


void View_SetDblClickTime(int aTime)
{
  DoubleClickTime = aTime;
}


int View_GetDblClickDist()
{
  return DoubleClickDist;
}


void View_SetDblClickDist(int aDist)
{
  DoubleClickDist = aDist;
}

/*

Set new mouse cursor

*/
void View_SetMouse(bmpBitmap aNum, int rx, int ry)
{
  View_SetMouse(SystemBitmap[aNum], rx, ry);
}


/*

Set new mouse cursor

*/
void View_SetMouse(BITMAP *Bitmap, int rx, int ry)
{
  if (Mouse.Bitmap!=Bitmap || rx!=Mouse.Focusx || ry!=Mouse.Focusy)
  {
    Mouse.NextBitmap = Bitmap;
    Mouse.Focusx = rx;
    Mouse.Focusy = ry;
    NewMouse = 1;
  }
}


/*

Restore background of last tick to Buffer

*/
void View_RestoreMouse(BITMAP *Back, BITMAP *Buffer)
{
  if (Back!=NULL)
  {
    blit(Back, Buffer,
         0,0, Mouse.Oldx-Mouse.w, Mouse.Oldy-Mouse.h,
         Back->w, Back->h);
  }
}


/*

Restore background of last tick to Buffer

*/
void View_RestoreMouse(BITMAP *Buffer)
{
  View_RestoreMouse(Mouse.Back, Buffer);
}


/*

Save background from Buffer with current mouse information

*/
void View_SaveMouse(BITMAP *Buffer)
{
  // Save background
  blit(Buffer, Mouse.Back,
       Mouse.Actx-Mouse.w, Mouse.Acty-Mouse.h, 0, 0,
       Mouse.Back->w, Mouse.Back->h);

  Mouse.Oldx = Mouse.Actx;
  Mouse.Oldy = Mouse.Acty;
}


/*

Check for new mouse cursor and activate it

*/
void View_CheckNewMouse()
{
    if (NewMouse)
    {
      Mouse.Bitmap = Mouse.NextBitmap;
      Mouse.NextBitmap = NULL;

      if (Mouse.Back!=NULL) Cgfx::DestroyBitmap(Mouse.Back);
      if (Mouse.Back2!=NULL) Cgfx::DestroyBitmap(Mouse.Back2);

      if (Mouse.Bitmap!=NULL)
      {
        Mouse.w = Mouse.Bitmap->w;
        Mouse.h = Mouse.Bitmap->h;
        Mouse.Back = Cgfx::CreateBitmap(Mouse.w*3, Mouse.h*3);
        Mouse.Back2 = Cgfx::CreateBitmap(Mouse.w*3, Mouse.h*3);
      } else
      {
        Mouse.w = 0;
        Mouse.h = 0;
        Mouse.Back = NULL;
        Mouse.Back2 = NULL;
      }

      NewMouse = 0;
    }
}


/*

Draw mouse cursor in Buffer at position (x/y)

*/
void View_ShowMouse(BITMAP *Buffer, int x, int y)
{
  if (Mouse.Bitmap!=NULL)
  {
    draw_sprite(Buffer, Mouse.Bitmap, x, y);
  }
}


/*

Draw mouse cursor in Buffer with current mouse information

*/
void View_ShowMouse(BITMAP *Buffer)
{
  View_CheckNewMouse();
  View_SaveMouse(Buffer);
  View_ShowMouse(Buffer, Mouse.Actx, Mouse.Acty);
}



/*

Update mouse information

*/
void View_UpdateMouse(int mx = -1, int my = -1)
{
  if(mx<0 || my<0)
  {
    Mouse.Actx = mouse_x-Mouse.Focusx;
    Mouse.Acty = mouse_y-Mouse.Focusy;
  }
  else
  {
    Mouse.Actx = mx-Mouse.Focusx;
    Mouse.Acty = my-Mouse.Focusy;
  }
}


/*

Get information about mouse rectangle

*/
void View_GetMouseRect(int &x, int &y, int &sx, int &sy)
{
  x = Mouse.Actx;
  y = Mouse.Acty;

  if (Mouse.Bitmap!=NULL)
  {
    sx = Mouse.w;
    sy = Mouse.h;

    x -= sx;
    y -= sy;

    sx *= 3;
    sy *= 3;
  } else
  {
    sx = 0;
    sy = 0;
  }
}


/*

Poll current mouse information

*/
void View_PollMouse(int &x, int &y, int &sx, int &sy)
{
  x = Mouse.Actx;
  y = Mouse.Acty;

  if (Mouse.Bitmap!=NULL)
  {
    sx = Mouse.w;
    sy = Mouse.h;
  } else
  {
    sx = 0;
    sy = 0;
  }
}


/*

Update  mouse cursor to Buffer by restoring background and drawing new cursor

*/
void View_BlitMouse(BITMAP *Buffer)
{
  if (NewMouse)
  {
    View_RestoreMouse(Buffer);
    View_ShowMouse(Buffer);
  } else
  {
    // Neue Position
    int nx = Mouse.Actx;
    int ny = Mouse.Acty;

    // Alte Position
    int ox = Mouse.Oldx;
    int oy = Mouse.Oldy;

    int w = Mouse.Back->w;
    int h = Mouse.Back->h;

    if (ox<nx+w && ox+w>nx &&
        oy<ny+h && oy+h>ny)
    {
      // Alten und neuen Hintergrund austauschen
      BITMAP *Old = Mouse.Back;
      Mouse.Back = Mouse.Back2;
      Mouse.Back2 = Old;

      // Neuen Hintergrund sichern, jedoch mit aktuellem Mauszeiger
      View_SaveMouse(Buffer);

      // Aktuellen Mauszeiger mit altem Hintergrund Åberschreiben
      //blit(Mouse.Back2, Mouse.Back, 0, 0, ox-nx, oy-ny, w, h);
      blit(Mouse.Back2, Mouse.Back, Mouse.w, Mouse.h,
           ox-nx+Mouse.w, oy-ny+Mouse.h, Mouse.w, Mouse.h);

      // Neuen Mauszeiger auf alten Hintergrund zeichnen
      View_ShowMouse(Mouse.Back2, nx-ox+Mouse.w, ny-oy+Mouse.h);

      // Union aus altem und neuem Mausrechteck
      int mx = min(ox, nx);
      int my = min(oy, ny);
      int mw = max(ox, nx)+Mouse.w-mx;
      int mh = max(oy, ny)+Mouse.h-my;

      // +-------+
      // |       |
      // |  +-+  |
      // |  |+-+ |
      // |  +| | |
      // |   +-+ |
      // +-------+
      //
      // Alten Hintergrund mit neuem Mauszeiger in Buffer blitten
      //blit(Mouse.Back2, Buffer, 0, 0, ox-Mouse.w, oy-Mouse.h, w, h);
      blit(Mouse.Back2, Buffer, mx-(ox-Mouse.w), my-(oy-Mouse.h),
           mx, my, mw, mh);
    } else
    {
      View_RestoreMouse(Buffer);
      View_ShowMouse(Buffer);
    }
  }
}


