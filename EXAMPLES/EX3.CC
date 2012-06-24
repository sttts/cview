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

 ex1.cc - menu example

*/


#include <stdio.h>
#include "view.h"
#include <allegro.h>


//===============================================


const cmCommand cmInfo            = cmCommand(cmUser+0);
const cmCommand cmAbout           = cmCommand(cmUser+1);


//===============================================


class CEx1:public CProgram
{
 public:
  CEx1();
  virtual ~CEx1();

  virtual void *HandleEvent(cmCommand Event, void* Data=0);

 protected:
};


CEx1::CEx1()
{
  Insert(new CFilledRect(0, 0, sx, sy, Cgfx::MakeColor(0, 0 ,0)));

  Insert(NewMenuBar(0, 0, sx, 13,
    NewMenu("File",
      NewMenuText("Quit", cmQuit),
      NULL),
    NewMenu("Help",
      NewMenuText("Info", cmInfo),
      NewMenuText("About", cmAbout),
      NULL),
    NULL)
  );
}


CEx1::~CEx1()
{
}


void *CEx1::HandleEvent(cmCommand Event, void* Data)
{
  switch (Event)
  {
  case cmAbout: MessageBox(msgOk, msgIconInformation,
                  "About", "CView example 3");
                break;

  case cmInfo: MessageBox(msgOk, msgIconInformation,
                 "Information", "You have requested some information");
               break;

  default: return CProgram::HandleEvent(Event, Data);
  }

  return NULL;
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
