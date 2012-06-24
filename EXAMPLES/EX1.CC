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

 ex1.cc - simple application

*/


#include <stdio.h>
#include "view.h"
#include <allegro.h>


//===============================================


class CEx1:public CProgram
{
 public:
  CEx1();
  virtual ~CEx1();

 protected:
};


CEx1::CEx1()
{
  Insert(new CFilledRect(0, 0, sx, sy, Color(colBack)));
  Insert(new CTextButton(100, 85, 120, 30, "Quit", cmQuit));
}


CEx1::~CEx1()
{
}


//===============================================


int main()
{
  allegro_init();
  install_keyboard();
  install_timer();
  install_mouse();

  if (View_Init(320,200,8)==true)
  {
    CEx1 Ex1;
    Ex1.Run();

    View_Done();
  } else
  {
    allegro_exit();
    printf("Error occured\n");
  }
  return 0;
}
