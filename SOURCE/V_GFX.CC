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

 v_gfx.cc - Graphics interface functions
            by Doug Eleveld

*/


#include "v_gfx.h"
#include "allegttf.h"


//================================================================


char *Cgfx::Modes[] = { "256x224",
                        "256x240",
                        "320x100",
                        "320x200",
                        "320x240",
                        "320x400",
                        "320x480",
                        "320x600",
                        "360x200",
                        "360x240",
                        "360x360",
                        "360x400",
                        "360x480",
                        "400x300",
                        "640x350",
                        "640x400",
                        "640x480",
                        "800x600",
                        "1024x768",
                        "1280x1024",
                        "1600x1280",
                        NULL
                      };

char *Cgfx::Depths[] = { "8",
                         "15",
                         "16",
                         "24",
                         "32",
                         NULL
                       };
   

//================================================================


void Cgfx::Init()
{
}


void Cgfx::Done()
{
}


//================================================================


bool Cgfx::Mode(const char* aMode, const char* aDepth)
{
  return true;
}


bool Cgfx::Mode(int width, int height, int depth)
{
  return true;
}


void Cgfx::TextMode()
{
}


int Cgfx::Width()
{
  return SCREEN_W;
}


int Cgfx::Height()
{
  return SCREEN_H;
}


int Cgfx::Depth()
{
  return _color_depth;
}


//================================================================


Cgfx::Font::Font()
  : Data(::font),
    Flags(0)
{
}


Cgfx::Font::~Font()
{
  destroy();
}


void Cgfx::Font::ensure()
{
  if (Data==NULL)
  {
    Data = ::font;
    Flags &= ~fDestroy;
  }
}


void Cgfx::Font::destroy()
{
  if (Flags& fDestroy)
    destroy_font(Data);

  Data = ::font;
  Flags &= ~fDestroy;
}

void Cgfx::Font::Load(const char* filename, int aPoints, int aFlags)
{
  int allegttf_flags = ALLEGTTF_NOSMOOTH;

  if (aFlags&fSmoothed)
    allegttf_flags = ALLEGTTF_REALSMOOTH;

  destroy();
//  Data = load_ttf_font_ex(filename,aPoints,aPoints,allegttf_flags);
  Data = load_ttf_font_ex(filename,aPoints,aPoints,1,0);
  Flags |= fDestroy;

  ensure();
}

      
int Cgfx::Font::Length(const char* s)
{
  return text_length(Data,(char*)s);
}


int Cgfx::Font::Height()
{
  return text_height(Data);
}


//================================================================


BITMAP* Cgfx::CreateBitmap(int w, int h)
{
  BITMAP* ret = create_bitmap(w,h);
  return ret;
}


BITMAP* Cgfx::CreateBitmap(BITMAP* parent, int x, int y, int w, int h)
{
  BITMAP* ret = create_sub_bitmap(parent,x,y,w,h);
  return ret;
}


void Cgfx::DestroyBitmap(BITMAP* bit)
{
  if(bit!=NULL && bit != ::screen)
  {
    destroy_bitmap(bit);
  }
}


//================================================================


void Cgfx::Text (BITMAP *bmp, const Font& fon, char *string, int x, int y, CColor col)
{
  aatextout(bmp,fon.Data,string,x,y,col);
}


void Cgfx::TextCenter (BITMAP *bmp, const Font& fon, char *string, int x, int y, CColor col)
{
  aatextout_center(bmp,fon.Data,string,x,y,col);
}


void Cgfx::VLine(BITMAP *bmp, int x, int y1, int y2, CColor col)
{
  vline(bmp,x,y1,y2,col);
}


void Cgfx::HLine(BITMAP *bmp, int x1, int y, int x2, CColor col)
{
  hline(bmp,x1,y,x2,col);
}


void Cgfx::Line(BITMAP *bmp, int x1, int y1, int x2, int y2, CColor col)
{
  line(bmp,x1,y1,x2,y2,col);
}


void Cgfx::Rect(BITMAP *bmp, int x1, int y1, int x2, int y2, CColor col)
{
  rect(bmp,x1,y1,x2,y2,col);
}


void Cgfx::Rectfill(BITMAP *bmp, int x1, int y1, int x2, int y2, CColor col)
{
  rectfill(bmp,x1,y1,x2,y2,col);
}


void Cgfx::Pixel(BITMAP *bmp, int x, int y, CColor col)
{
  putpixel(bmp,x,y,col);
}


CColor Cgfx::MakeColor(int r, int g, int b)
{
  return (CColor)makecol(r,g,b);
}



