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

 v_pictur.cc - Simple picture class

*/


#include "v_pictur.h"
#include "v_common.h"
#include "v_sysbit.h"
#include "v_save.h"

#include <allegro.h>


//const LineSpacing=2;

pctFlags operator|(const pctFlags one, const pctFlags two)
{
  return (pctFlags)((int)one | (int)two);
}


//===================================================


CPicture::CPicture(int apx, int apy, int asx,  int asy, BITMAP *aPicture, pctFlags aFlags)
 : CView(apx, apy, asx, asy)
{
  Flags = aFlags;

  if (aPicture==NULL)
  {
    aPicture=SystemBitmap[bmpInValid];
    Flags = Flags &= ~pctDestroy;
    Flags = Flags &= ~pctCopy;
  }

  if (Flags & pctCopy)
  {
    Flags |= pctDestroy;
    Picture = Cgfx::CreateBitmap(aPicture->w, aPicture->h);
    clear(Picture);
    draw_sprite(Picture, aPicture, 0, 0);
  } else
  {
    Picture = aPicture;
  }

  if (Flags & pctTrans) SetTransparent(1);
}


CPicture::CPicture(int apx, int apy, int asx,  int asy, RLE_SPRITE *aPicture, pctFlags aFlags)
 : CView(apx, apy, asx, asy)
{
  Flags = aFlags;

  if (aPicture==NULL) aPicture=get_rle_sprite(SystemBitmap[bmpInValid]);

  Flags |= pctDestroy;
  Picture = Cgfx::CreateBitmap(aPicture->w, aPicture->h);
  clear(Picture);
  draw_rle_sprite(Picture, aPicture, 0, 0);

  if (Flags & pctTrans) SetTransparent(1);
}



CPicture::~CPicture()
{
  if (Flags & pctDestroy)
    Cgfx::DestroyBitmap(Picture);
}


void* CPicture::HandleEvent(cmCommand Event, void* Data)
{
  if(Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CPicture P%p Picture=%p Flags=%i }\n",this,Picture,Flags);
    AddSerializeInfo(Data,info);
  }
  return CView::HandleEvent(Event, Data);
}


void CPicture::Draw()
{
  if (Owner!=NULL && Picture!=NULL)
  {
    BITMAP *Buffer = Owner->GetBuffer();

    if (Flags & pctTiled)
    {
      if (Flags & pctTrans)
      {
        for (int y=0; y<sy; y+=Picture->h)
          for (int x=0; x<sx; x+=Picture->w)
            draw_sprite(Buffer, Picture, px+x, py+y);
      } else
      {
        for (int y=0; y<sy; y+=Picture->h)
          for (int x=0; x<sx; x+=Picture->w)
            blit(Picture, Buffer, 0, 0, px+x, py+y,
              Picture->w, Picture->h);
      }
    } else
    if (Flags & pctScale)
    {
      if (Flags & pctTrans)
        stretch_sprite(Buffer, Picture, px, py, sx, sy);
      else
        stretch_blit(Picture, Buffer, 0, 0, Picture->w, Picture->h,
          px, py, sx, sy);
    } else
    {
      int x=0;
      int y=0;

      if (Flags & pctCentered)
      {
        x = (sx-Picture->w)/2;
        y = (sy-Picture->h)/2;
      }

      if (Flags & pctTrans)
        draw_sprite(Buffer, Picture, px+x, py+y);
      else
        blit(Picture, Buffer, 0, 0, px+x, py+y, Picture->w, Picture->h);

    }
  }
}


void CPicture::SetPicture(BITMAP *aPicture, pctFlags aFlags)
{
  if (Picture!=NULL && Flags & pctDestroy) Cgfx::DestroyBitmap(Picture);
  if (aFlags!=pctKeep) Flags = aFlags;

  if (aPicture==NULL)
  {
    aPicture=SystemBitmap[bmpInValid];
    Flags = Flags &= ~pctDestroy;
    Flags = Flags &= ~pctCopy;
  }

  if (Flags & pctCopy)
  {
    Flags |= pctDestroy;

    Picture = Cgfx::CreateBitmap(aPicture->w, aPicture->h);
    clear(Picture);
    draw_sprite(Picture, aPicture, 0, 0);
  } else
  {
    Picture = aPicture;
  }

  if (Flags & pctTrans) SetTransparent(1);

  Redraw();
}


void CPicture::SetPicture(RLE_SPRITE *aPicture, pctFlags aFlags)
{
  if (Picture!=NULL && Flags & pctDestroy) Cgfx::DestroyBitmap(Picture);
  if (aFlags!=pctKeep) Flags = aFlags;

  if (aPicture==NULL) aPicture=get_rle_sprite(SystemBitmap[bmpInValid]);

  Flags |= pctDestroy;
  Picture = Cgfx::CreateBitmap(aPicture->w, aPicture->h);
  clear(Picture);
  draw_rle_sprite(Picture, aPicture, 0, 0);

  if (Flags & pctTrans) SetTransparent(1);

  Redraw();
}


void CPicture::DefaultScale()
{
  sy = Picture->h;
  sx = Picture->w;
}





