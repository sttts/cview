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

 v_utils.h - Different utility classes

*/


#ifndef V_UTILS_H_INCLUDED
#define V_UTILS_H_INCLUDED


#include "v_base.h"


class C3dOut:public CView
{
 public:
  C3dOut(int apx, int apy, int asx, int asy, bool doFill = true);
  virtual ~C3dOut();

  void* HandleEvent(cmCommand Event, void* Data);
  virtual void Draw();

 protected:
  bool Fill;
};


class C3dIn:public CView
{
 public:
  C3dIn(int apx, int apy, int asx, int asy, bool doFill = true);
  virtual ~C3dIn();

  void* HandleEvent(cmCommand Event, void* Data);
  virtual void Draw();

 protected:
  bool Fill;
};


class CFilledRect:public CView
{
 public:
  CFilledRect(int apx, int apy, int asx, int asy, CColor aColor);
  virtual ~CFilledRect();

  void* HandleEvent(cmCommand Event, void* Data);
  virtual void Draw();

  void SetColor(CColor aColor);
  CColor GetColor();

 protected:
  CColor Col;
};

#endif
