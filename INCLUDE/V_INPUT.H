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

 v_input.h - Input line class

*/


#ifndef V_INPUT_H_INCLUDED
#define V_INPUT_H_INCLUDED


#include "v_base.h"


const inpEditable               = 0x0001;
const inpDefault                = inpEditable;


class CInputLine:public CView
{
 public:
  CInputLine(int apx, int apy, int asx, int asy, int aMaxLen, char* aInput="",
    char* aKeys=NULL, int aFlags=inpEditable);
  virtual ~CInputLine();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  virtual void HandleMouse(int mx, int my, int buttons);
  virtual void* HandleKey(char Char, char Scancode);

  virtual void Draw();

  virtual int Focus();
  virtual int DeFocus();
  virtual int CanClose();

  virtual void DefaultScale();

  char *GetInput();
  void SetInput(char *aInput);

 protected:
  char* Input;
  char* OldInput;
  int MaxLen;
  char* Keys;
  unsigned int Pos;
  unsigned int Cur;
  int VisChars;
  int Flags;

  void Insert(char c);
  void BackSpace();
  void Delete();
  void MoveCursor(unsigned int c);
};


#endif
