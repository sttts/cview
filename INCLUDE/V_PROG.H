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

 v_prog.h - Application main class

*/


#ifndef V_PROG_H_INCLUDED
#define V_PROG_H_INCLUDED


#include "v_base.h"


void StartTimer();
int StopTimer();
void View_SetSyncSpeed(int aH);


class CProgram:public CWindow
{
 public:
  CProgram();
  virtual ~CProgram();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  virtual void* HandleKey(char Char, char Scancode);

  virtual void Insert(CView *View);
  virtual void Remove(CView *View);
  virtual void Front(CView *View);

  virtual void Draw();
  virtual void CreateBuffer();
  virtual void Update(CView *aClient, int ax, int ay, int asx, int asy);
  void SetMaxDirtyRects(int aNum);
  virtual void Idle();

  static const bool double_buffer = false;

 protected:

 private:
  virtual void DrawRect(int ax, int ay, int bx, int by);
  void DrawDirt();
  int Dirty;
  int DirtAllowed;
  int MaxDirtyRects;
  struct CDirtRect *Dirt;
};


#endif
