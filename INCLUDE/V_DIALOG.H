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

 v_dialog.h - Dialog class and helper classes

*/


#ifndef V_DIALOG_H_INCLUDED
#define V_DIALOG_H_INCLUDED


#include "v_base.h"
#include "v_button.h"
#include "v_border.h"


enum dlgFlags { dlgNone    = 0,
                dlgClose   = 1,
                dlgCenter  = 1<<1,
                dlgDrag    = 1<<2,
                dlgScale   = 1<<3,
                dlgEscape  = 1<<4,

                dlgDefault=dlgClose | dlgDrag,

                dlgLast    = 0xffffffff
              };


dlgFlags operator|(const dlgFlags one, const dlgFlags two);
dlgFlags operator&(const dlgFlags one, const dlgFlags two);


class CClient:public CView
{
 public:
  CClient(int apx, int apy, int asx, int asy);
  virtual ~CClient();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  virtual void Draw();

  void Insert(CView *View);
  void Remove(CView *View);

  void SetBackground(BITMAP *aBack);

 protected:
  BITMAP *Back;
};


class CTitlebar:public CView
{
 public:
  CTitlebar(int apx, int apy, int asx,  int asy, char *aText, int aCentered);
  virtual ~CTitlebar();

  virtual void HandleMouse(int mx, int my, int buttons);
  virtual void* HandleEvent(cmCommand Event, void* Data=0);

  virtual void Draw();

  void SetBackground(BITMAP *aBack);

  int GetOnTop() { return OnTop; };
  const char* GetText() { return Text; };
  const int GetCentered() { return Centered; };

 protected:
  char *Text;
  int Centered;
  int OnTop;
  BITMAP *Back;
};


class CCloseButton:public CButton
{
 public:
  CCloseButton(int apx, int apy, int asx,  int asy);
  virtual ~CCloseButton();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  virtual void Draw();

 protected:
};


class CDialog;


class CBorder:public CView
{
 public:
  // Constructors
  CBorder(CDialog& aTarget);
  virtual ~CBorder();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  virtual void Draw()
  {
    Drawer.Draw(Target,*this);
  };

  CDialogBorderStyle Drawer;

 protected:
  CDialog& Target;
};


class CDialog:public CWindow
{
 public:
  CDialog(int apx, int apy, int asx, int asy, char *aTitle="",
          dlgFlags aFlags=dlgDefault);
  virtual ~CDialog();

  virtual void Draw(int ax, int ay, int asx, int asy);

  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  virtual void HandleMouse(int mx, int my, int buttons);
  virtual void* HandleKey(char Char, char Scancode);

  void SetBackground(BITMAP *aTitle, BITMAP *aClient);
  virtual void ScaleTo(int x, int y);
  virtual void CorrectSize(int &x, int &y);
  void Center();

  virtual void Enclose(const int border = CView::DefaultSpacing);

  // Some info of use to the Border drawing class
  CCloseButton* GetClose() { return Close; };
  CTitlebar* GetTitle() { return Title; };
  CClient* GetClient() { return Client; };

  int GetFlags() { return Flags; };

  // Dialogs have exchangable border styles
  void SetStyle (CDialogBorderStyle::rep& newStyle);

  // Some helpful constants
  static int TitleSize;
  static const int ResizeSpace;
  static const int DiagResizeSpace;

  static const int drgDiag;
  static const int drgHor;
  static const int drgVert;

 protected:
  dlgFlags Flags;
  int DragMode;
  CCloseButton *Close;
  CTitlebar *Title;
  CClient *Client;
  CBorder *Border;

  void UpdateLook();
};



#endif
