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

 builder.cc - CView gui builder application
              by Doug Eleveld

*/


#include <stdio.h>
#include "view.h"
#include <allegro.h>

const cmCommand cmNew             = cmCommand(cmUser+1);
const cmCommand cmOpen            = cmCommand(cmUser+2);
const cmCommand cmSave            = cmCommand(cmUser+3);
const cmCommand cmChangeVideoMode = cmCommand(cmUser+4);
const cmCommand cmInfo            = cmCommand(cmUser+5);

// New object commands
const cmCommand cmAddObject       = cmCommand(cmUser+6);
const cmCommand cmNewButton       = cmCommand(cmUser+7);


CDialogBorderWin95 BorderDrawer;

// Make a new default color scheme
CColorScheme< 32, 32, 32,   // Foreground
             180,180,180,   // Background
             255,255,255,   // Light shading
              80, 80, 80,   // Dark shading
             158,158,205    // List selection color
             > ColorScheme;


//========================================================


class CBuilderDialog:public CDialog
{
 public:
  CBuilderDialog(int apx, int apy, int asx, int asy, char *aTitle="",
                 dlgFlags aFlags=dlgDefault);
  virtual ~CBuilderDialog();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);

 protected:
};



CBuilderDialog::CBuilderDialog(int apx, int apy, int asx, int asy, char *aTitle="",
                               dlgFlags aFlags=dlgDefault)
 : CDialog(apx,apy,asx,asy,aTitle,aFlags)
{
}


CBuilderDialog::~CBuilderDialog()
{
}


void* CBuilderDialog::HandleEvent(cmCommand Event, void* Data)
{
  // Possibly add an object to the dialog
  if(Event==cmAddObject)
  {
    CView* Obj = (CView*)Data;
    Insert(Obj);
  }
  return CDialog::HandleEvent(Event,Data);
}


//========================================================


class CBuilder:public CProgram
{
 public:
  CBuilder();
  virtual ~CBuilder();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);

  void InfoDlg();
  void ChangeVideoMode();

  void FileOpen();
  void FileNew();
  void FileSave();

  void NewButton();

 protected:
  CBuilderDialog* Dialog;
};



CBuilder::CBuilder()
 : Dialog(NULL)
{
  // Make a background and put it at the back
  CView *Background = new CFilledRect(0, 0, sx, sy, Cgfx::MakeColor(180,180,180));
  Background->SetScaleMode(sclRight | sclBottom);
  Insert(Background);
  
  // Menu
  Insert(NewMenuBar(0, 0, sx, 10,
    NewMenu(View_GetString(100),                // Datei
      NewMenuText(View_GetString(101), cmNew),  // Neu
      NewMenuText(View_GetString(102), cmOpen), // ™ffnen
      NewMenuText(View_GetString(103), cmSave), // ™ffnen
      NewMenuText(View_GetString(109), cmQuit), // Beenden
      NewMenuText(View_GetString(120), cmChangeVideoMode),
      NULL),
    NewMenu("&Add",
      NewMenuText("&Button", cmNewButton),
      NULL),
    NewMenu(View_GetString(190),                // Hilfe
      NewMenuText(View_GetString(192), cmInfo), // Info
      NULL),
    NULL)
  );

  InfoDlg();
}


CBuilder::~CBuilder()
{
}


void* CBuilder::HandleEvent(cmCommand Event, void* Data)
{
  switch (Event)
  {
    case cmChangeVideoMode: ChangeVideoMode();
                      break;
    case cmInfo: InfoDlg();
                break;
    case cmNew: FileNew();
                break;
    case cmOpen: FileOpen();
                 break;
    case cmSave: FileSave();
                 break;
    case cmNewButton: NewButton();
                 break;
    default:
      return CProgram::HandleEvent(Event, Data);
  }
  return NULL;
}


void CBuilder::InfoDlg()
{
  CDialog *d = new CInfoDlg("CView GUI builder","builder.pcx");
  ExecuteWindow(d);
  delete d;
}


void CBuilder::FileOpen()
{
}


void CBuilder::FileSave()
{
  if (Dialog==NULL)
    return;

  char* Data = NULL;

  Dialog->HandleEvent(cmSerialize,&Data);
  Data = CleanupSerializeInfo(Data);

  FILE* stream = fopen("Build.cv","wt");
  if (stream!=NULL)
  {
    if (Data!=NULL)
    {
      fprintf(stream,"%s",Data);
      delete Data;
    }

    fclose(stream);
  }
}


void CBuilder::FileNew()
{
  // Remove the old stuff
  if (Dialog!=NULL)
  {
    Remove(Dialog);
    delete Dialog;
  }

  Dialog = new CBuilderDialog(100-rand()%25,100-rand()%25,100+rand()%25,100+rand()%25,"Title",dlgDefault | dlgScale);
  Insert(Dialog);
}


void CBuilder::NewButton()
{
  if (Dialog==NULL)
    return;
    
  // Make the new button
  CButton* Butt = new CTextButton(10-rand()%5,30-rand()%10,1,1,"Button");
  Butt->DefaultScale();

  // Add it to the dialog
  Dialog->HandleEvent(cmAddObject,Butt);
}


void CBuilder::ChangeVideoMode()
{
  CVideoModeDlg* Dlg = new CVideoModeDlg;
  ExecuteWindow(Dlg);
  delete Dlg;
}


//===============================================


int main()
{
  allegro_init();
  install_keyboard();
  install_timer();
  install_mouse();

  // Load a nice palette
  DATAFILE *Pal = load_datafile_object("CVIEW.DAT", "PAL");
  set_palette((PALETTE)Pal->dat);

  if (View_Init(640,480,16)==true)
  {
    SystemFont.Load("arial.ttf",8,Cgfx::fSmoothed);
    CDialog::TitleSize = SystemFont.Height();

    CColorInterface::Default = ColorScheme;
    CColorInterface::Default.Reset();
    
    CDialogBorderStyle::Default = BorderDrawer;

    CBuilder Builder;
    Builder.Run();

    View_Done();
    allegro_exit();
  } else
  {
    allegro_exit();
    printf("Error occured\n");
  }
  
  unload_datafile_object(Pal);
  return 0;
}
