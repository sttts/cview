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

 v_common.h - rectangle class

*/

#ifndef V_RECT_H_INCLUDED
#define V_RECT_H_INCLUDED



class CPoint
{
 public:
  CPoint(int ax, int ay) { x=ax; y=ay; }
  CPoint(const CPoint &p) { x=p.x; y=p.y; }
  ~CPoint() {}

  int x, y;
};


class CSize
{
 public:
  CSize(int aw, int ah) { w=aw; h=ah; }
  CSize(const CSize &s) { w=s.w; h=s.h; }
  ~CSize() {}

  int w, h;
};


class CRect
{
 public:
  CRect(int x, int y, int w, int h) :p(x,y),s(w,h) {}
  CRect(const CPoint &ap, const CSize &as) :p(ap),s(as) {}

  inline const int Valid() { return s.w>0 && s.h>0; }
  inline operator=(const CPoint b) { p = b; }
  inline operator=(const CSize b) { s = b; }

  CPoint p;
  CSize s;
};



inline CPoint operator+(const CPoint a, const CSize b)
  { return CPoint(a.x+b.w, a.y+b.h); }
inline CPoint operator+(const CSize a, const CPoint b)
  { return CPoint(a.w+b.x, a.h+b.y); }
inline CPoint operator+(const CPoint a, const CPoint b)
  { return CPoint(a.x+b.x, a.y+b.y); }
inline CPoint &operator+=(CPoint &a, const CSize b)
  { a.x+=b.w; a.y+=b.h; return a; }
inline CPoint &operator+=(CPoint &a, const CPoint b)
  { a.x+=b.x; a.y+=b.y; return a; }
inline CPoint &operator-=(CPoint &a, const CSize b)
  { a.x-=b.w; a.y-=b.h; return a; }
inline CPoint &operator-=(CPoint &a, const CPoint b)
  { a.x-=b.x; a.y-=b.y; return a; }


inline CSize operator-(const CPoint &a, const CPoint b)
  { return CSize(a.x-b.x, a.y-b.y); }
inline CSize operator+(const CSize &a, const CSize b)
  { return CSize(a.w+b.w, a.h+b.h); }
inline CSize &operator+=(CSize &a, const CSize b)
  { a.w+=b.w; a.h+=b.h; return a; }
inline CSize &operator-=(CSize &a, const CSize b)
  { a.w-=b.w; a.h-=b.h; return a; }


inline CRect &operator+=(CRect &a, const CPoint b) { a.p += b; return a; }
inline CRect &operator-=(CRect &a, const CPoint b) { a.p -= b; return a; }
inline CRect &operator+=(CRect &a, const CSize b) { a.s += b; return a; }
inline CRect &operator-=(CRect &a, const CSize b) { a.s -= b; return a; }

inline CRect operator|(const CRect &a, const CRect &b)
{
  CPoint p = a.p;
  CSize s = a.s;

  if (p.x>b.p.x) { s.w += p.x-b.p.x; p.x = b.p.x; }   // left
  if (p.y>b.p.y) { s.h += p.y-b.p.y; p.y = b.p.y; }   // top
  if (p.x+s.w<b.p.x+b.s.w) s.w += b.p.x+b.s.w-p.x-s.w;  // right
  if (p.y+s.h<b.p.y+b.s.h) s.h += b.p.y+b.s.h-p.y-s.h;  // bottom

  return CRect(p, s);
}


inline CRect operator&(CRect &a, CRect &b)
{
  CPoint p = a.p;
  CSize s = a.s;

  if (p.x<b.p.x) { s.w += p.x-b.p.x; p.x = b.p.x; }   // left
  if (p.y<b.p.y) { s.h += p.y-b.p.y; p.y = b.p.y; }   // top
  if (p.x+s.w>b.p.x+b.s.w) s.w += b.p.x+b.s.w-p.x-s.w;  // right
  if (p.y+s.h>b.p.y+b.s.h) s.h += b.p.y+b.s.h-p.y-s.h;  // bottom

  return CRect(p, s);
}


#endif
