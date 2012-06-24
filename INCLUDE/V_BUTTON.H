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

 v_button.h - Different button classes

*/


#ifndef V_BUTTON_H_INCLUDED
#define V_BUTTON_H_INCLUDED


#include "v_base.h"
#include "v_sysbit.h"


enum btnFlags { btnDefault      = 0,
                btnMultiEvent   = 1,
                btnSwitch       = 1<<1,
                btnRadio        = 1<<2,
                btnTransparent  = 1<<3
              };
              

//Helper operations
btnFlags operator|(const btnFlags one, const btnFlags two);


class CButton:public CView
{
 public:
  CButton(int apx, int apy, int asx,  int asy, cmCommand aCommand=cmOk,
          CView *aTarget=NULL, btnFlags aFlags=btnDefault);
  virtual ~CButton();

  virtual void HandleMouse(int mx, int my, int buttons);
  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  virtual void MouseLost(int mx, int my, int buttons);
  virtual void Draw();
  virtual void Action();
  void SetState(int aClicked);
  int GetState();

 protected:
  int Flags;
  int Clicked;
  cmCommand Command;
  CView *Target;
};


class CPicButton:public CButton
{
 public:
  CPicButton(int apx, int apy, int asx,  int asy, BITMAP *aPicture,
             cmCommand aCommand=cmOk, CView *aTarget=NULL,
             btnFlags aFlags=btnDefault | btnTransparent);
  CPicButton(int apx, int apy, int asx,  int asy,
             BITMAP *aPicture1, BITMAP *aPicture2,
             cmCommand aCommand=cmOk, CView *aTarget=NULL, btnFlags aFlags=btnDefault);
  virtual ~CPicButton();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  virtual void Draw();
  
  virtual void DefaultScale();

 protected:
  BITMAP *PictureUp;
  BITMAP *PictureDown;
};


class CSystemBitmapButton:public CButton
{
 public:
  CSystemBitmapButton(int apx, int apy, int asx,  int asy, bmpBitmap aPicture,
             cmCommand aCommand=cmOk, CView *aTarget=NULL,
             btnFlags aFlags=btnDefault | btnTransparent);
  CSystemBitmapButton(int apx, int apy, int asx,  int asy,
             bmpBitmap aPicture1, bmpBitmap aPicture2,
             cmCommand aCommand=cmOk, CView *aTarget=NULL, btnFlags aFlags=btnDefault);
  virtual ~CSystemBitmapButton();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  virtual void Draw();
  
  virtual void DefaultScale();

 protected:
  bmpBitmap PictureUp;
  bmpBitmap PictureDown;
};


class CTextButton:public CButton
{
 public:
  CTextButton(int apx, int apy, int asx,  int asy, char *aText,
              cmCommand aCommand=cmOk, CView *aTarget=NULL, btnFlags aFlags=btnDefault);
  virtual ~CTextButton();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  virtual void Draw();
  virtual void* HandleAccel(char Char, char Scancode);

  virtual void DefaultScale();

 protected:
  char *Text;
};


class CTransButton:public CButton
{
 public:
  CTransButton(int apx, int apy, int asx,  int asy,
              cmCommand aCommand=cmOk, CView *aTarget=NULL, btnFlags aFlags=btnDefault);
  virtual ~CTransButton();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  virtual void Draw();

 protected:
};



#endif
