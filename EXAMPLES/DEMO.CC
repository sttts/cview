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

 demo.cc - Demo application
           Modified by Doug Eleveld

*/


//#define DOSAMP


#include <stdio.h>
#include "view.h"

#include <allegro.h>

#ifdef DOSAMP
  #include "libamp.h"
#endif

const cmCommand cmNew             = cmCommand(cmUser+1);
const cmCommand cmOpen            = cmCommand(cmUser+2);
const cmCommand cmInfo            = cmCommand(cmUser+3);
const cmCommand cmPlayMp3         = cmCommand(cmUser+4);
const cmCommand cmStopMp3         = cmCommand(cmUser+5);
const cmCommand cmChangeStyle     = cmCommand(cmUser+6);
const cmCommand cmDebugRect       = cmCommand(cmUser+7);
const cmCommand cmFileDlg         = cmCommand(cmUser+8);
const cmCommand cmChangeVideoMode = cmCommand(cmUser+9);
//const cmCommand cmDoSerialize     = cmCommand(cmUser+10);


char Mp3[256]="";
int LoopMp3=0;

int DebugRect=0;

fileFlags FileDlgFlags = fileDefault;

// Another border style so the user can toggle between them
// The normal drawer is called CDialogBorder::DefaultDrawer

//CDialogBorderWin31 BorderDrawerSecond;
CDialogBorderFlat  BorderDrawerFirst;
CDialogBorderWin95 BorderDrawerSecond;
//CDialogBorderNone  BorderDrawer4;


// Make a new default color scheme
CColorScheme< 32, 32, 32,   // Foreground
             180,180,180,   // Background
             255,255,255,   // Light shading
              80, 80, 80,   // Dark shading
             158,158,205    // List selection color
             > SecondColorScheme;

// Make another menu style
CMenuStyleBox BoxMenuStyle;

//========================================================


class CTestDlg:public CDialog
{
 public:
  CTestDlg(int apx, int apy);
  virtual ~CTestDlg();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);

 protected:
  CScrollbar *Scroller;
  CInputLine *inpValue;
  CInputLine *inpMax;
  CInputLine *inpPage;

  CCheckbox *chkLoop;

  int Value;
  int Max;
  int Page;

  void UpdateContent();
};


CTestDlg::CTestDlg(int apx, int apy)
 : CDialog(apx, apy, 155, 100, "Test-Dialog", dlgClose | dlgDrag | dlgScale)
{
  Scroller = new CScrollbar(136, 5, 9, 80);
  Scroller->SetScaleMode(sclLeft | sclRight | sclBottom);
  Client->Insert(Scroller);

  inpValue = new CInputLine(5, 5,  100, 12, 5, "0", "0123456789");
  inpMax =   new CInputLine(5, 20, 100, 12, 5, "100", "0123456789");
  inpPage =  new CInputLine(5, 35, 100, 12, 5, "10", "0123456789");

  Client->Insert(inpValue);
  Client->Insert(inpMax);
  Client->Insert(inpPage);

  Value = 0;
  Max   = 100;
  Page  = 10;

  UpdateContent();

  Client->Insert(new CTextButton(5,  50, 30, 12, "&Play", cmPlayMp3, Program));
  Client->Insert(new CTextButton(40, 50, 30, 12, "&Stop", cmStopMp3, Program));
  Client->Insert(new CTextButton(75, 50, 30, 12, "&Open", cmOpen, Program));

  chkLoop = new CCheckbox(110, 50, "&Loop", LoopMp3);
  chkLoop->DefaultScale();
  Client->Insert(chkLoop);
//  Client->Insert(new CText(125, 50, "Loop"));

  // Menu
//  Insert(NewMenuBar(0, 70, sx, 10,
//    NewMenu(View_GetString(100),                // Datei
//      NewMenuText(View_GetString(101), cmNew),  // Neu
//      NewMenuText(View_GetString(102), cmOpen), // ™ffnen
//      NewMenuText(View_GetString(109), cmQuit), // Beenden
//      NewMenuText("&File1", cmFileDlg),
//      NewMenuText("&File2", cmFileDlg),
//      NULL),
//    NewMenu(View_GetString(190),                // Hilfe
//      NewMenuText(View_GetString(192), cmInfo), // Info
//      NULL),
//    NewMenu("Debug",
//      NewMenuText("Rectangles", cmDebugRect),
//      NULL),
//    NULL)
//  );

  Enclose();
}


CTestDlg::~CTestDlg()
{
}


void CTestDlg::UpdateContent()
{
  char s[6];

  inpValue->SetInput(itoa(Value, s, 10));
  inpMax->SetInput(itoa(Max, s, 10));
  inpPage->SetInput(itoa(Page, s, 10));

  Scroller->Update(Value, Max, Page);
}


void *CTestDlg::HandleEvent(cmCommand Event, void* Data)
{
  switch (Event)
  {
  case cmScrolled:
    if ((CScrollbar*)Data==Scroller)
    {
      Value = Scroller->GetNewPos();
      UpdateContent();
    }
    break;

  case cmInputChanged:
    if ((CInputLine*)Data==inpValue) Value = atoi(inpValue->GetInput()); else
    if ((CInputLine*)Data==inpMax) Max = atoi(inpMax->GetInput()); else
    if ((CInputLine*)Data==inpPage) Page = atoi(inpPage->GetInput());
    UpdateContent();
    break;

  case cmCheckSelect: if ((CCheckbox *)Data==chkLoop)
                      {
                        LoopMp3 = chkLoop->GetStatus();
                      }
                      break;

  default: return CDialog::HandleEvent(Event, Data);
  }

  return NULL;
}


//===============================================


struct
{
  fileFlags Flag;
  char *Text;
  CCheckbox *Checkbox;
} FileFlags[] =
{
  { fileMustExist, "fileMust&Exist" , NULL },
  { fileDirs,      "file&Dirs"      , NULL },
  { fileFiles,     "file&Files"     , NULL },
  { fileDrives,    "file&Drives"    , NULL },
  { fileMask,      "file&Mask"      , NULL },
  { fileInputLine, "file&InputLine" , NULL },
  { fileMkDir,     "file&MkDir"     , NULL },
  { fileDelFile,   "file&DelFile"   , NULL },
  { fileFileNeeded,"file&FileNeeded", NULL },
  { fileDirUp,     "file&DirUp"     , NULL },
  { fileScaleable, "file&Scaleable" , NULL },
  { (fileFlags)0,   NULL            , NULL }
};



class CFileOptions:public CDialog
{
 public:
  CFileOptions(int apx, int apy);
  virtual ~CFileOptions();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);

 protected:

};


CFileOptions::CFileOptions(int apx, int apy)
 : CDialog(apx, apy, 10, 10, "File-Options", dlgDrag | dlgCenter)
{
  // Add the first one in
  FileFlags[0].Checkbox = new CCheckbox(5, 2, FileFlags[0].Text, FileDlgFlags & FileFlags[0].Flag);
  FileFlags[0].Checkbox->DefaultScale();
  Client->Insert(FileFlags[0].Checkbox);

  // Add the other ones underneath
  int n = 1;
  while (FileFlags[n].Flag)
  {
    FileFlags[n].Checkbox = new CCheckbox(5, 5, FileFlags[n].Text, FileDlgFlags & FileFlags[n].Flag);
    FileFlags[n].Checkbox->DefaultScale();
    FileFlags[n].Checkbox->SetScaleMode(sclSideBottom,2);
    Client->Insert(FileFlags[n].Checkbox);
    n++;
  }

  CView* button = new CTextButton(5, 5, 5, 5, "CFileSelector", cmFileDlg);
  button->DefaultScale();
  button->SetScaleMode(sclSideBottom);
  Client->Insert(button);

  Enclose();
}


CFileOptions::~CFileOptions()
{
}


void* CFileOptions::HandleEvent(cmCommand Event, void* Data)
{
  int n;

  switch (Event)
  {
    case cmCheckSelect:
    {
      CCheckbox *c = (CCheckbox *)Data;
      for (n=0; FileFlags[n].Flag && FileFlags[n].Checkbox!=c; n++);

      if (FileFlags[n].Flag)
      {
        if (c->GetStatus())
          FileDlgFlags = FileDlgFlags | FileFlags[n].Flag;
        else
          FileDlgFlags = FileDlgFlags & ~FileFlags[n].Flag;
      }
    }
    break;

    case cmFileDlg:
    {
      CFileSelector *Dlg = new CFileSelector("C:\\", "*.*", View_GetString(400),
        View_GetString(401), FileDlgFlags);

      Dlg->InsertFileMask("*.*");
      Dlg->InsertFileMask("*.C");
      Dlg->InsertFileMask("*.CC");
      Dlg->InsertFileMask("*.CPP");
      Dlg->InsertFileMask("*.H");
      ExecuteWindow(Dlg, 1);
      delete Dlg;
    }
    break;

  default: { };
  }

  return CDialog::HandleEvent(Event, Data);
}


//===============================================


class CDemo:public CProgram
{
 public:
  CDemo();
  virtual ~CDemo();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);

  void InfoDlg();
  void ChangeVideoMode();

  void FileOpen();
  void FileNew();

  virtual void Idle();

 protected:
};


static CFileOptions* FileOptions = NULL;


CDemo::CDemo()
{
  RGB Pal[256];

  BITMAP *Back = load_pcx("back.pcx", Pal);
  View_PaletteConvert(Back,Pal);
  CView *Background = new CPicture(0, 0, sx, sy, Back,
    pctDefault | pctTiled | pctDestroy);
  Background->SetScaleMode(sclRight | sclBottom);
  Insert(Background);

  // Add the button to change the dialog style
  CTextButton* ChangeStyle = new CTextButton(20, 20, 10, 10, "Toggle Style", cmChangeStyle);
  ChangeStyle->DefaultScale();
  Insert(ChangeStyle);

  // Add the button to wite out
//  CTextButton* Serialize = new CTextButton(20, 20, 10, 10, "Serialize", cmDoSerialize);
//  Serialize->DefaultScale();
//  Insert(Serialize);
//  Serialize->SetScaleMode(sclSideBottom);

//  CMenuItem *VideoMenu;

  // Menu
  Insert(NewMenuBar(0, 0, sx, 10,
    NewMenu(View_GetString(100),                // Datei
      NewMenuText(View_GetString(101), cmNew),  // Neu
      NewMenuText(View_GetString(102), cmOpen), // ™ffnen
      NewMenuText(View_GetString(109), cmQuit), // Beenden
      NewMenuText(View_GetString(120), cmChangeVideoMode),
      NewMenuText("&File2", cmFileDlg),
      NULL),
    NewMenu(View_GetString(190),                // Hilfe
      NewMenuText(View_GetString(192), cmInfo), // Info
      NULL),
    NewMenu("Debug",
      NewMenuText("Rectangles", cmDebugRect),
      NULL),
    NULL)
  );

  Insert(new CTestDlg(50, 50));

  FileOptions = new CFileOptions(30, 80);
  Insert(FileOptions);

  InfoDlg();
}


CDemo::~CDemo()
{
}


void View_SwitchRects(int aOn);


void* CDemo::HandleEvent(cmCommand Event, void* Data)
{
  static bool BorderType = false;

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
    case cmDebugRect: DebugRect = !DebugRect;
                      View_SwitchRects(DebugRect);
                      break;
                        
    // Toggle the Dialog border style of newly created
    // created dialogs
    case cmChangeStyle:
    {
      if (BorderType==true)
      {
        CColorInterface::Default = DefaultColorScheme;
        CDialogBorderStyle::Default = BorderDrawerFirst;

        // Broadcast a border/color change
        HandleEvent(cmDialogStyle,&BorderDrawerFirst);
        HandleEvent(cmColorScheme,&DefaultColorScheme);
        HandleEvent(cmMenuStyle,&BoxMenuStyle);

        BUFFER_T Mode = BufferFull;
        HandleEvent(cmWindowBufferMode,&Mode);

        BorderType = false;
      }
      else
      {
        SecondColorScheme.Reset();
        CColorInterface::Default = SecondColorScheme;
          
        CDialogBorderStyle::Default = BorderDrawerSecond;
          
        // Broadcast a border/color change
        HandleEvent(cmDialogStyle,&BorderDrawerSecond);
        HandleEvent(cmColorScheme,&SecondColorScheme);
        HandleEvent(cmMenuStyle,&CMenuStyle::DefaultDrawer);
        
        BUFFER_T Mode = BufferSub;
        HandleEvent(cmWindowBufferMode,&Mode);

        BorderType = true;
      }
      break;
    }
//    case cmDoSerialize:
//    {
//      char* output = NULL;
//
//      HandleEvent(cmSerialize,&output);
//      output = CleanupSerializeInfo(output);
      
//      clog << output;
//      delete output;
//
//      break;
//    }
      
#ifdef DOSAMP
    case cmPlayMp3: load_amp(Mp3, LoopMp3);
                    break;

    case cmStopMp3: unload_amp();
                    install_amp();
                    break;
#endif
    default:
      return CProgram::HandleEvent(Event, Data);
  }
  return NULL;
}


void CDemo::InfoDlg()
{
  CDialog *d = new CInfoDlg("CView Demo", "demo.pcx");
  ExecuteWindow(d);
  delete d;
}


void CDemo::FileOpen()
{
  strcpy(Mp3, "");
  CFileSelector *Dlg = new CFileSelector("", "*.*", View_GetString(400), View_GetString(401),
    fileMustExist    |
    fileDirs         |
    fileFiles        |
    fileDrives       |
    fileInputLine    |
    fileFileNeeded   |
    fileMask         |
    fileScaleable    |
    fileDirUp);
  Dlg->InsertFileMask("*.*");
  Dlg->InsertFileMask("*.pcx");
  Dlg->InsertFileMask("*.tga");
  Dlg->InsertFileMask("*.bmp");
  Dlg->InsertFileMask("*.lbm");

#ifdef DOSAMP
  Dlg->InsertFileMask("*.mp3");
#endif

  if (ExecuteWindow(Dlg)==cmOk)
  {
    char *p = Dlg->GetPath();

#ifdef DOSAMP
    if (strstr(p, ".mp")!=NULL || strstr(p, ".MP")!=NULL)
    {
      if (!load_amp(p, 0))
      {
        MessageBox(msgOk, 0, "Error", "Couldn't open file %s.", p);
      } else strcpy(Mp3, p);
    } else
#endif
    {
      RGB Pal[256];
      BITMAP *b = load_bitmap(p, Pal);
      View_PaletteConvert(b,Pal,0);
      if (b!=NULL)
      {
        Insert(new CPicDialog(30, 30, p, b));
      } else MessageBox(msgOk, 0, "Error", "Couldn't open file %s.", p);
    }
  }
  delete Dlg;
}


void CDemo::FileNew()
{
  MessageBox(msgOk, msgIconInformation, "Titel", View_GetString(500));
}


void CDemo::ChangeVideoMode()
{
  CVideoModeDlg* Dlg = new CVideoModeDlg;
  ExecuteWindow(Dlg);
  delete Dlg;
}


void CDemo::Idle()
{
#ifdef DOSAMP
  poll_amp();
#endif
  CProgram::Idle();
}


//===============================================


int main()
{
  allegro_init();
  install_keyboard();
  install_timer();
  install_mouse();
  install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
#ifdef DOSAMP
  install_amp();
#endif

  // Load a nice palette
  DATAFILE *Pal = load_datafile_object("CVIEW.DAT", "PAL");
  set_palette((PALETTE)Pal->dat);

  if (View_Init(640,480,16)==true)
  {
    SystemFont.Load("arial.ttf",8,Cgfx::fSmoothed);
    CDialog::TitleSize = SystemFont.Height();

    CDemo Demo;
    Demo.Run();

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
