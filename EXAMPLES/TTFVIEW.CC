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

 demo.cc - True Type font viewer by Doug Eleveld

*/


#include <stdio.h>

#include "view.h"

#include <allegttf.h>

const cmCommand cmOpen            = cmCommand(cmUser+1);
const cmCommand cmInfo            = cmCommand(cmUser+2);


// Make a new default color scheme
CColorScheme< 32, 32, 32,   // Foreground
             180,180,180,   // Background
             255,255,255,   // Light shading
              80, 80, 80,   // Dark shading
             128,128,205    // List selection color
             > SecondColorScheme;


// Modify the default border drawer
CDialogBorderWin31 BorderDrawer;

// Modify the default menu style
CMenuStyleBox BoxMenuStyle;


//===============================================


class CFontInfoDlg:public CDialog
{
 public:
  CFontInfoDlg(void);
  virtual ~CFontInfoDlg();

  int PointSize();
  int Smoothing();

 protected:
  CText* FontSizeText;
  CInputLine* FontSize;

  CRadioButton* NoSmooth;
  CRadioButton* TTFSmooth;
  CRadioButton* RealSmooth;

  CTextButton* OkButton;
  CTextButton* CancelButton;
};


CFontInfoDlg::CFontInfoDlg(void)
 :CDialog(0, 0, 10, 10, "TTF parameters"),
 FontSizeText(new CText(5,5,"Point size")),
 FontSize(new CInputLine(5, 5, 5, 5, 5, "0", "0123456789")),
 NoSmooth(new CRadioButton(5,5,"No Smoothing",1)),
 TTFSmooth(new CRadioButton(5,5,"TTF Smoothing")),
 RealSmooth(new CRadioButton(5,5,"Real Smoothing")),
 OkButton(new CTextButton(5, 5, 5, 5, "Ok",cmOk)),
 CancelButton(new CTextButton(5, 5, 5, 5, "Cancel",cmCancel))
{
  // Make sure objects are correct size
  FontSize->DefaultScale();
  CancelButton->DefaultScale();
  OkButton->ScaleTo(CancelButton->Width(),CancelButton->Height());
  NoSmooth->DefaultScale();
  TTFSmooth->DefaultScale();
  RealSmooth->DefaultScale();

  // Insert them into the dialog
  Client->Insert(FontSizeText);
  Client->Insert(FontSize);
  Client->Insert(NoSmooth);
  Client->Insert(TTFSmooth);
  Client->Insert(RealSmooth);
  Client->Insert(OkButton);
  Client->Insert(CancelButton);

  // Place them under each other make the first elements a reference
  // so that the text can be placed next to it
  FontSizeText->SetScaleMode(sclRef);
  FontSize->SetScaleMode(sclSideRight);
  NoSmooth->SetScaleMode(sclSideBottom | sclRefRelative);
  TTFSmooth->SetScaleMode(sclSideBottom);
  RealSmooth->SetScaleMode(sclSideBottom);

  // Put the buttons next to each other under everything
  OkButton->SetScaleMode(sclSideBottom);
  CancelButton->SetScaleMode(sclSideRight);

  // Default font size
  FontSize->SetInput("10");

  // Setup the dialog size
  int mx = CancelButton->Posx() + CancelButton->Width() + CView::DefaultSpacing;
  int my = CancelButton->Posy() + CancelButton->Height() + CView::DefaultSpacing;
  ScaleTo(mx,my);
  Center();

  Enclose();
}


CFontInfoDlg::~CFontInfoDlg()
{
}


int CFontInfoDlg::PointSize()
{
  return atoi(FontSize->GetInput());
}


int CFontInfoDlg::Smoothing()
{
  if (TTFSmooth->GetStatus())
     return ALLEGTTF_TTFSMOOTH;
  else if (RealSmooth->GetStatus())
     return ALLEGTTF_REALSMOOTH;
  return ALLEGTTF_NOSMOOTH;
}


//===============================================


class CDemo:public CProgram
{
 public:
  CDemo();
  virtual ~CDemo();

  virtual void *HandleEvent(cmCommand Event, void* Data=0);

  void InfoDlg();
  void VideoMode(int n);

  void FileOpen();

 protected:
};


CDemo::CDemo()
{
  RGB Pal[256];

  BITMAP *Back = load_pcx("back.pcx", Pal);
  View_PaletteConvert(Back,Pal);
  CView *Background = new CPicture(0, 0, sx, sy, Back,
    pctDefault | pctTiled | pctDestroy);
  Background->SetScaleMode(sclRight | sclBottom);
  Insert(Background);

  // Menu
  Insert(NewMenuBar(0, 0, sx, 10,
    NewMenu(View_GetString(100),                // Datei
      NewMenuText(View_GetString(102), cmOpen), // ™ffnen
      NewMenuText(View_GetString(109), cmQuit), // Beenden
      NULL),
    NewMenu(View_GetString(190),                // Hilfe
      NewMenuText(View_GetString(192), cmInfo), // Info
      NULL),
    NULL)
  );

  InfoDlg();
}


CDemo::~CDemo()
{
}


void *CDemo::HandleEvent(cmCommand Event, void* Data)
{
  switch (Event)
  {
    case cmInfo: InfoDlg();
                 break;
    case cmOpen: FileOpen();
                 break;
    default: return CProgram::HandleEvent(Event, Data);
  }

  return NULL;
}


void CDemo::InfoDlg()
{
  CDialog *d = new CInfoDlg("TTFview", "ttfview.pcx");
  ExecuteWindow(d);
  delete d;
}


void CDemo::FileOpen()
{
  CFileSelector *Dlg = new CFileSelector("", "*.ttf", View_GetString(400), View_GetString(401),
    fileMustExist    |
    fileDirs         |
    fileFiles        |
    fileDrives       |
    fileInputLine    |
    fileFileNeeded   |
    fileMask         |
    fileScaleable    |
    fileDirUp);
  Dlg->InsertFileMask("*.ttf");

  if (ExecuteWindow(Dlg)==cmOk)
  {
    char *p = Dlg->GetPath();
    {
      // Ask the user point sizes etc
      CFontInfoDlg *info = new CFontInfoDlg;
      if (ExecuteWindow(info)==cmOk)
      {
        // Get the info from the font info dialog
        int points = info->PointSize();
        int smooth = info->Smoothing();

        // Make a bitmap for the screen
        BITMAP *b = Cgfx::CreateBitmap(points*16+20,points*6+20);
        clear_to_color(b,Color(colBack));

        if (b!=NULL)
        {
          char s[3] = " ";

          // Load the TTF file
          FONT* ttf = load_ttf_font_ex(p,points,points,1,smooth);
          if(ttf!=NULL)
          {
            text_mode(-1);

            // Print the TTF file to a bitmap
            for(int x=0;x<6;x++)
            {
              for(int y=0;y<16;y++)
              {
                aatextout(b,ttf,s,y*points+10,x*points+10,Color(colFore));
                s[0]++;
              }
            }
            Insert(new CPicDialog(30, 30, get_filename(p), b));

            destroy_font(ttf);
          }
          else  MessageBox(msgOk, msgIconNone, "Error", "Couldn't open file %s",p);

        }
        else MessageBox(msgOk, msgIconNone, "Error", "Couldn't create bitmap");
      }
      delete info;
    }
  }
  delete Dlg;
}


//===============================================


int main()
{
  allegro_init();
  install_keyboard();
  install_timer();
  install_mouse();
  install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);

  // Load a nice palette
  PALETTE pal;
  generate_332_palette(pal);
  set_palette(pal);

  RGB black = {0,0,0};
  set_color(0,&black);

  if (View_Init(640,480,16)==true)
  {
    SystemFont.Load("arial.ttf",10,Cgfx::fSmoothed);
    CDialog::TitleSize = SystemFont.Height();

    // Modify the default border
    CDialogBorderStyle::Default = BorderDrawer;

    // Mofify the default color scheme
    CColorInterface::Default = SecondColorScheme;
    CColorInterface::Default.Reset();

    // Mofify the default menu style
    CMenuStyle::Default = BoxMenuStyle;

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
