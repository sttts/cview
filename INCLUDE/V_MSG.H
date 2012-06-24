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

 v_msg.h - Message and input dialog class

*/


#ifndef V_MSG_H_INCLUDED
#define V_MSG_H_INCLUDED


#include <stdarg.h>
#include "v_dialog.h"
#include "v_input.h"


enum msgButtons { msgNone      = 0,
                  msgOk        = 1,
                  msgCancel    = 1<<1,
                  msgYes       = 1<<2,
                  msgNo        = 1<<3,
                  msgRetry     = 1<<4,
                  msgIgnore    = 1<<5
                };

enum msgIcon { msgIconNone = 0,
               msgIconQuestion,
               msgIconExclamation,
               msgIconInformation
             };


//Helper operations
msgButtons operator|(const msgButtons one, const msgButtons two);


extern int MessageBox(int aButtons, int aIcons, char *aTitle, char *aMsg, ...);
extern int InputBox(char *aTitle, char *aLabel, char *aInput, char *Buffer, int nMaxLen);


class CMessageBox:public CDialog
{
 public:
  CMessageBox(int aButtons, int aIcons, int Width, int Height,
              char *aTitle, char *aMsg, ...);
  CMessageBox(int aButtons, int aIcons, char *aTitle, char *aMsg, ...);
  CMessageBox(int aButtons, int aIcons, int Width, int Height,
              char *aTitle, char *aMsg, va_list arg);
  CMessageBox(int aButtons, int aIcons, char *aTitle, char *aMsg, va_list arg);
  virtual ~CMessageBox();

 protected:
  void SetupWindow(int aButtons, int aIcons, int Width, int Height,
    char *aTitle, char *aMsg, va_list arg);
};


class CInputBox:public CDialog
{
 public:
  CInputBox(int asx, char *aTitle, char *aLabel,
    int aMaxLen, char *aInput=NULL, char *aKeys=NULL);
  CInputBox(char *aTitle, char *aLabel,
    int aMaxLen, char *aInput=NULL, char *aKeys=NULL);
  virtual ~CInputBox();

  char *GetInput();
  void SetInput(char *aInput);

 protected:
  CInputLine *Input;
};


#endif

