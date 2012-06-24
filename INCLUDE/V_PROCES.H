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

 v_proces.h - Process bar

*/


#ifndef V_PROCESS_H_INCLUDED
#define V_PROCESS_H_INCLUDED


#include "v_base.h"


class CProcessBar:public CView
{
 public:
  CProcessBar(int apx, int apy, int asx, int asy, int aStatus);
  virtual ~CProcessBar();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  virtual void Draw();

  int GetStatus() { return Status; }
  void SetStatus(int aStatus);

 protected:
  int Status;
};


#endif
