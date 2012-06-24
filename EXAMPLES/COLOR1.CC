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

 color1.cc - Example how to use CColorScheme objects
             by Doug Eleveld

*/


#include <stdio.h>
#include "view.h"
#include <allegro.h>

// Here are some color schemes setup with RGB values
CColorScheme<192,192,0,  // Foreground (text)
             RGB_BACK,   // Background (normal)
             RGB_LIGHT,  // Light shading
             RGB_DARK,   // Dark shading
             RGB_SELECT  // Selection
             >YellowText;

CColorScheme<RGB_FORE,   // Foreground (text)
             255,0,0,    // Background (normal)
             RGB_LIGHT,  // Light shading
             RGB_DARK,   // Dark shading
             RGB_SELECT  // Selection
             >RedBackground;

CColorScheme<RGB_FORE,   // Foreground (text)
             0,0,255,    // Background (normal)
             RGB_LIGHT,  // Light shading
             RGB_DARK,   // Dark shading
             RGB_SELECT  // Selection
             >BlueBackground;


//========================================================


class CButtonDialog:public CDialog
{
 public:
  CButtonDialog(void);
  virtual ~CButtonDialog();

 protected:
  CTextButton* Button1;
  CTextButton* Button2;
  CTextButton* Button3;
  CTextButton* Button4;
  CTextButton* QuitButton;
};


CButtonDialog::CButtonDialog(void)
 :CDialog(0, 0, 10, 10, "CColorSchemeObject"),
 Button1(new CTextButton(5, 5, 5, 5, "Normal")),
 Button2(new CTextButton(5, 5, 5, 5, "Yellow")),
 Button3(new CTextButton(5, 5, 5, 5, "Red")),
 Button4(new CTextButton(5, 5, 5, 5, "Blue")),
 QuitButton(new CTextButton(5, 5, 5, 5, "Quit",cmQuit,Program))
{
  // Make sure objects are correct size
  Button1->DefaultScale();

  // Insert them into the dialog
  Client->Insert(Button1);
  Client->Insert(Button2);
  Client->Insert(Button3);
  Client->Insert(Button4);
  Client->Insert(QuitButton);

  // Place them under each other
  // and the same size
  Button2->SetScaleMode(sclSideBottom | sclSameSize);
  Button3->SetScaleMode(sclSideBottom | sclSameSize);
  Button4->SetScaleMode(sclSideBottom | sclSameSize);
  QuitButton->SetScaleMode(sclSideBottom | sclSameSize);

  // Set the color schemes of the objects
  YellowText.Reset();
  RedBackground.Reset();
  BlueBackground.Reset();

  Button2->ColorInterface = YellowText;
  Button3->ColorInterface = RedBackground;
  Button4->ColorInterface = BlueBackground;

  // Setup the dialog size
  Enclose();
  Center();
}


CButtonDialog::~CButtonDialog()
{
}

//===============================================


class CDemo:public CProgram
{
 public:
  CDemo();
  virtual ~CDemo();

 protected:
  CButtonDialog *info;

};


CDemo::CDemo()
{
  RGB Pal[256];

  // Put up a nice background
  BITMAP *Back = load_pcx("back.pcx", Pal);
  CView *Background = new CPicture(0, 0, sx, sy, Back,
    pctDefault | pctTiled | pctDestroy);
  Background->SetScaleMode(sclRight | sclBottom);
  Insert(Background);

  // Show the test dialog
  info = new CButtonDialog;
  Insert(info);
}


CDemo::~CDemo()
{
}

//===============================================


int main()
{
  allegro_init();
  install_keyboard();
  install_timer();
  install_mouse();
  install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);

  if (View_Init(640,480,16)==true)
  {
    CDemo Demo;
    Demo.Run();

    View_Done();
  } else
  {
    allegro_exit();
    printf("Error occured\n");
  }
  return 0;
}
