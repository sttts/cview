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

 v_file.cc - File selector dialog class

*/


#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dir.h>
#include <limits.h>
#include <ctype.h>
#include <fcntl.h>

#include "v_file.h"
#include "v_msg.h"
#include "v_list.h"
#include "v_common.h"
#include "v_gfx.h"
#include "v_string.h"
#include "v_combo.h"
#include "v_text.h"
#include "v_sysbit.h"
#include "v_system.h"

#include <allegro.h>


//===================================================


fileFlags operator~(const fileFlags one)
{
  return (fileFlags)(~one);
}


fileFlags operator|(const fileFlags one, const fileFlags two)
{
  return (fileFlags)((int)one | (int)two);
}


fileFlags operator&(const fileFlags one, const fileFlags two)
{
  return (fileFlags)((int)one & (int)two);
}


static char PathBuf[256];

char *MakePath(char aDrive, char *aDir=NULL, char *aFile=NULL)
{
  strcpy(PathBuf, "C:");
  PathBuf[0] = toupper(aDrive);
  if (aDir) strcat(PathBuf, aDir);
  put_backslash(PathBuf);
  if (aFile) strcat(PathBuf, aFile);
  return PathBuf;
}


void FixPath(char *Path)
{
  while (*Path)
  {
    if (*Path=='/') *Path='\\';
    Path++;
  }
}


int get_fileattrib(char *f, int &attrib)
{
  return file_exists(f, 0xffffffff, &attrib);
}


int IsDir(char *f)
{
  int a=0;
//  int r=
  get_fileattrib(f, a);
  return (a & FA_DIREC)!=0;
}


void ErrorMsg(char *Msg)
{
  MessageBox(msgCancel, msgIconExclamation, "Error", Msg);
}


// Drive ready function taken from Allegro
bool DriveReady (const char drive)
{
  __dpmi_regs r;

  const char driveletter = tolower(drive);

  if((driveletter=='a')||(driveletter=='b'))
  {
    r.x.ax = 0x440E;
    r.x.bx = 1;
    __dpmi_int(0x21, &r);

    if((r.h.al!=0)&&(r.h.al!=(driveletter-'a'+1)))
       return false;
  }
  return true;
}


char CurrentDrive()
{
  return getdisk()+'A';
}


int SetDrive(char Drive)
{
  // First check if the drive is valid
  if (DriveReady(Drive)==true)
  {
    Drive = toupper(Drive);
    setdisk(Drive-'A');
  }
  return toupper(getdisk())==Drive-'A';
}


char *CurrentPath(char aDrive)
{
  char OldDrive = CurrentDrive();

  SetDrive(aDrive);
  getwd(PathBuf);
  SetDrive(OldDrive);

  char *DriveEnd = strchr(PathBuf, ':');

  return DriveEnd+1;
}


int ExistDrive(char Drive)
{
  char OldDrive = CurrentDrive();
  int r = SetDrive(Drive);
  SetDrive(OldDrive);
  return r;
}


//===================================================


class CIconTextItem:public CListItem
{
 public:
  CIconTextItem(int aId, char *aText, bmpBitmap aIcon);
  virtual ~CIconTextItem();

  virtual int Height();
  virtual int Width();

  virtual void Draw(BITMAP *Buffer, int x, int y);

 protected:
  int h;
  int w;
  bmpBitmap Icon;
};


CIconTextItem::CIconTextItem(int aId, char *aText, bmpBitmap aIcon)
 : CListItem(aId, aText)
{
  Icon = aIcon;

  h = SystemFont.Height() + 2;
  
  if (h<SystemBitmap[Icon]->h+2) h = SystemBitmap[Icon]->h+2;
  w = SystemFont.Length(Text) + 2 + SystemBitmap[Icon]->w + 2;
}


CIconTextItem::~CIconTextItem()
{
}


int CIconTextItem::Height()
{
  return h;
}


int CIconTextItem::Width()
{
  return w;
}


void CIconTextItem::Draw(BITMAP *Buffer, int x, int y)
{
  draw_sprite(Buffer, SystemBitmap[Icon], x+1, y+1);
  Cgfx::Text(Buffer, SystemFont, Text,
          x+SystemBitmap[Icon]->w+3, y+1, Owner->Color(colFore));
}


//===================================================


const fileFlags flDirs         = fileDirs;
const fileFlags flDrives       = fileDrives;
const fileFlags flFiles        = fileFiles;
const fileFlags flDefault      = flDirs | flFiles;


class CFileList:public CListbox
{
 public:
  CFileList(int apx, int apy, int asx, int asy, CScrollbar *aScroller,
            fileFlags aFlags=flDefault);
  virtual ~CFileList();

  void FillDir(char aDrive, char *aDir, char *aMask);
  virtual int Compare(CListItem *Item1, CListItem *Item2);

 protected:
  fileFlags Flags;
};


CFileList::CFileList(int apx, int apy, int asx, int asy, CScrollbar *aScroller,
                     fileFlags aFlags)
 : CListbox(apx, apy, asx, asy, lstSelect | lstShowSel, NULL, NULL, aScroller)
{
  Flags = aFlags;
}


CFileList::~CFileList()
{
}


int CFileList::Compare(CListItem *Item1, CListItem *Item2)
{
  int d=Item1->GetId()-Item2->GetId();
  if (d==0)
    return stricmp(Item1->GetText(), Item2->GetText());
  else
    return d;
}


CFileList *ActFileList=NULL;


void InsertFile(char *Name, int Attrib, int Param)
{
  (void)Attrib;
  (void)Param;

  ActFileList->Insert(new CIconTextItem(2, get_filename(Name),
    bmpDocument));
}


void InsertDirs(char *Name, int Attrib, int Param)
{
  (void)Param;

  if ((Attrib & FA_DIREC)==0) return;
  if (strcmp(get_filename(Name), "..")==0 || strcmp(get_filename(Name), ".")==0) return;

  ActFileList->Insert(new CIconTextItem(1, get_filename(Name),
    bmpFolder));
}


void CFileList::FillDir(char aDrive, char *aDir, char *aMask)
{
  RemoveAll();

  if (Flags & flDrives)
  {
    for (char d='A'; d<='Z'; d++)
    {
      char DriveItem[]="C:";
      DriveItem[0]=d;

      if (ExistDrive(d))
      {
        if (d<'C')
          Insert(new CIconTextItem(0, DriveItem, bmpDisk));
        else
          Insert(new CIconTextItem(0, DriveItem, bmpHarddisk));
      }
    }
  }

  // Make sure the drive is ready first
  if (DriveReady(aDrive)==false)
     return;

  ActFileList = this;

  if (Flags & flDirs)
  {
    if (strcmp(aDir, "\\")!=0)
      Insert(new CIconTextItem(1, "..", bmpFolder));
    for_each_file(MakePath(aDrive, aDir, "*.*"), FA_DIREC, (void (*)(...))InsertDirs, 0);
  }

  if (Flags & flFiles)
  {
    for_each_file(MakePath(aDrive, aDir, aMask), FA_ARCH, (void (*)(...))InsertFile, 0);
  }

  Redraw();
}


//===================================================


class CFileInput:public CInputLine
{
 public:
  CFileInput(int apx, int apy, int asx, int asy);
  virtual ~CFileInput();

 protected:
};


CFileInput::CFileInput(int apx, int apy, int asx, int asy)
 : CInputLine(apx, apy, asx, asy, 256)
{
}


CFileInput::~CFileInput()
{
}


//===================================================


CFileDlg::CFileDlg(int apx, int apy, int asx, int asy, char *aTitle, int aFlags)
 : CDialog(apx, apy, asx, asy, aTitle, dlgDrag | dlgCenter | dlgEscape)
{
  // Init members
  Flags = aFlags;

  if (Flags & fileScaleable) CDialog::Flags = CDialog::Flags | dlgScale;

  strcpy(Dir, "\\");
  strcpy(FileName, "");
  strcpy(SelFile, "");
  strcpy(Mask, "*.*");
  Drive=getdisk()+'A';
  HandleFile(CurrentPath(Drive), 0);
}


CFileDlg::~CFileDlg()
{
}


void CFileDlg::UpdateContent(int aDirChanged)
{
  (void)aDirChanged;
  Redraw();
}


char Buf[256];
char NewDir[256];
char NewFile[13];
char NewMask[13];
char NewDrive;


#define Error(Msg) { ErrorMsg(Msg); DirChanged=0; Result=0; goto End; }


int CFileDlg::HandleFile(char *aFile, int aVerify)
{
  if (aFile==NULL) return 0;

  int Result=1;

  strcpy(Buf, aFile);
  strcpy(NewDir, Dir);
  strcpy(NewFile, FileName);
  strcpy(NewMask, Mask);
  NewDrive=Drive;
  int DirChanged=0;

  char *Pos = Buf;

  // Slash -> Backslash
  FixPath(Buf);

  // Doppelpunkt fr Laufwerk vorhanden?
  char *Doppel = strchr(Pos, ':');
  if (Doppel)
  {
    if (Doppel-Pos==1)
    {
      NewDrive = *Pos;
      Pos+=2;
      if (!ExistDrive(NewDrive)) Error(View_GetString(strInvalidDrive));
      strcpy(NewDir, CurrentPath(NewDrive));
      FixPath(NewDir);
      strcpy(NewFile, "");
      DirChanged=1;
    } else Error(View_GetString(strInvalidPath));
  }

  // Backslash -> absoluter Pfad
  if (*Pos=='\\')
  {
    strcpy(NewDir, "\\");
    strcpy(NewFile, "");
    Pos++;
    DirChanged=1;
  }

  while (*Pos)
  {
    // Spezielle Pfade
    if (strncmp(Pos, "..", 2)==0)
    {
      char *Right=NewDir+strlen(NewDir)-2;

      while (*Right!='\\' && Right!=NewDir) Right--;
      if (Right>=NewDir)
      {
        Right++;
        *Right='\0';
        DirChanged=1;
      }

      Pos+=2;
    } else
    if (strncmp(Pos, ".", 1)==0)
    {
      Pos++;
      DirChanged=1;
    } else
    // Slash
    if (*Pos=='\\')
    {
      Pos++;
    } else
    // Allgemeine Namen
    {
      // Namen extrahieren
      char *Left=Pos;
      while (*Pos && *Pos!='\\') Pos++;
      strncpy(NewFile, Left, Pos-Left);
      NewFile[Pos-Left]='\0';

      // Wildcards
      if (strchr(NewFile, '?') || strchr(NewFile, '*'))
      {
        if (*Pos==0)
        {
          strcpy(NewMask, NewFile);
          strcpy(NewFile, "");
          DirChanged=1;
        } else Error(View_GetString(strInvalidPath));
      } else
      {
        // Datei vorhanden?
        int attrib;
        if (get_fileattrib(MakePath(NewDrive, NewDir, NewFile), attrib))
        {
          // Verzeichnis?
          if (attrib & FA_DIREC)
          {
            // Verezcihnis an Pfad anh„ngen
            strcat(NewDir, NewFile);
            strcat(NewDir, "\\");
            strcpy(NewFile, "");
            DirChanged=1;
          } else
          {
            // Dateiname am Ende?
            if (*Pos!=0) Error(View_GetString(strInvalidPath));
          }
        } else
        {
          // Datei nicht gefunden
          if (Flags & fileMustExist) Error(View_GetString(strFileMustExist));
        }
      }
    }
  }

  if (aVerify)
  {
    if ((Flags & fileDrives)!=0 || Drive==toupper(NewDrive))
      Drive = toupper(NewDrive);
    else Error(View_GetString(strDriveChangeForbidden));

    if ((Flags & fileFiles)!=0 || stricmp(FileName, NewFile)==0)
      strcpy(FileName, NewFile);
    else strcpy(FileName, "");

    if ((Flags & fileDirs)!=0 || stricmp(Dir, NewDir)==0)
      strcpy(Dir, NewDir);
    else Error(View_GetString(strDirChangeForbidden));

    if ((Flags & fileMask)!=0 || stricmp(Mask, NewMask)==0)
      strcpy(Mask, NewMask);
    else Error(View_GetString(strMaskChangeForbidden));
  } else
  {
    Drive = toupper(NewDrive);
    strcpy(FileName, NewFile);
    strcpy(Dir, NewDir);
    strcpy(Mask, NewMask);
  }

  strupr(Mask);
  strupr(Dir);
  strupr(FileName);
 End:
  strcpy(SelFile, FileName);
  if (aVerify) UpdateContent(DirChanged); else UpdateContent(1);
  return Result;
}


char *CFileDlg::GetPath()
{
  return MakePath(Drive, Dir, FileName);
}


char *CFileDlg::GetDir()
{
  return Dir;
}


char *CFileDlg::GetMask()
{
  return Mask;
}


char *CFileDlg::GetFileName()
{
  return FileName;
}


char CFileDlg::GetDrive()
{
  return Drive;
}


int CFileDlg::CanClose()
{
  int r=CDialog::CanClose();

  if (r)
  {
    if ((Flags & fileFileNeeded) && (strlen(FileName)==0))
    {
      ErrorMsg(View_GetString(strFileNeeded));
      return 0;
    } else return 1;
  }

  return 0;
}


//================================================


const BORDER            = CView::DefaultSpacing;
const SBUTTONW          = 12;
const SBUTTONH          = 12;

CFileSelector::CFileSelector(char *aPath, char *aMask, char *aTitle, char *aButton,
           fileFlags aFlags)
 :CFileDlg(0, 0, 1, 1, aTitle, aFlags)
{
  CView* temp;

  // Make sure the objects are a reasonable size
  int CharWidth = SystemFont.Length("A");
  int CharHeight = SystemFont.Height();

  const int ListWidth = CharWidth*20;
  const int ListHeight = CharHeight*10;
  const int MaskWidth = CharWidth*10;
  const int MaskHeight = CharHeight*7;

  const int GenericHeight = CharHeight+2;

  // Drive box
  if (Flags & fileDrives)
  {
    DriveList = new CCombobox(BORDER, BORDER, MaskWidth, MaskHeight, "", comboPopup);
    DriveList->SetScaleMode(sclRef);
    DriveList->DefaultScale();
    Client->Insert(DriveList);

    for (char d='A'; d<='Z'; d++)
    {
      char DriveItem[]="C:";
      DriveItem[0]=d;

      if (ExistDrive(d))
      {
        if (d<'C')
          DriveList->Insert(new CIconTextItem(d, DriveItem, bmpDisk));
        else
          DriveList->Insert(new CIconTextItem(d, DriveItem, bmpHarddisk));
      }
    }
  } else DriveList=NULL;

  // Small buttons
  // Directory up
  if (Flags & fileDirUp)
  {
    temp = new CSystemBitmapButton(1, 1, SBUTTONW, SBUTTONH, bmpDirUp, cmFileDirUp);
    temp->SetScaleMode(sclSideRight,2);
    Client->Insert(temp);
  }

  // Create dir
  if (Flags & fileMkDir)
  {
    temp = new CSystemBitmapButton(1, 1, SBUTTONW, SBUTTONH, bmpFolder, cmFileMkDir);
    temp->SetScaleMode(sclSideRight,2);
    Client->Insert(temp);
  }

  // Del file
  if (Flags & fileDelFile)
  {
    temp = new CSystemBitmapButton(1, 1, SBUTTONW, SBUTTONH, bmpDelFile, cmFileDel);
    temp->SetScaleMode(sclSideRight,2);
//    temp->DefaultScale();
    Client->Insert(temp);
  }

  // Current directory
  if (Flags & fileDirs)
  {
    DirText = new CText(1, 1, "");
    DirText->SetScaleMode(sclSideBottom | sclRefRelative | sclRef);
//    DirText->DefaultScale();
    Client->Insert(DirText);
  } else DirText=NULL;

  // Listbox and scrollbar for files
  CScrollbar *s = new CScrollbar(1, 1, 9, ListHeight, NULL, scrDefault & ~scrFastScroll);

  fileFlags ListFlags = fileNone;
  if (Flags & fileDirs) ListFlags = ListFlags | flDirs;
  if (Flags & fileFiles) ListFlags = ListFlags | flFiles;
  FileList = new CFileList(1, 1, ListWidth, ListHeight, s, ListFlags);
  FileList->SetScaleMode(sclSideBottom | sclRefRelative | sclRef);
//  FileList->DefaultScale();
  Client->Insert(FileList);

  s->SetScaleMode(sclSideRight | sclSameHeight, 0);
  Client->Insert(s);

  // Insert input line
  if (Flags & fileInputLine)
  {
    // Input line for file name
    FileInput = new CFileInput(1, 1, ListWidth-MaskWidth, GenericHeight);
//    FileInput->DefaultScale();
    FileInput->SetScaleMode(sclSideBottom | sclRefRelative | sclRef);
    Client->Insert(FileInput);
  } else FileInput = NULL;

  // Insert mask list
  if (Flags & fileMask)
  {
    MaskList = new CCombobox(1, 1, MaskWidth, MaskHeight, "", comboPopup);
    MaskList->DefaultScale();
    MaskList->SetScaleMode(sclSideRight);
    Client->Insert(MaskList);
  } else MaskList = NULL;

  // Buttons
  temp = new CTextButton(1, 1, 1, 1, aButton);
  temp->DefaultScale();
  temp->SetScaleMode(sclSideBottom | sclRefRelative);
  Client->Insert(temp);

  temp = new CTextButton(1, 1, 1, 1, View_GetString(1), cmCancel);
  temp->SetScaleMode(sclSideRight | sclSameSize);
  temp->DefaultScale();
  Client->Insert(temp);

  // Scale to right size
  Enclose();
  Center();

  // Set the
  FileList->SetScaleMode(sclSideBottom | sclRefRelative | sclRight | sclBottom | sclRef);
  if(FileInput)
    FileInput->SetScaleMode(sclSideBottom | sclRefRelative | sclRef | sclRight);

  // Set content
  if (aMask!=NULL) strcpy(Mask, aMask);
  HandleFile(aPath, 0);
}


CFileSelector::~CFileSelector()
{
}


void CFileSelector::CorrectSize(int &x, int &y)
{
  if (y<120) y=120;
  if (x<50) x=50;
}


void CFileSelector::MakeDir()
{
  char DirName[13]="";
  int r=InputBox(View_GetString(strMakeDir), View_GetString(strDirName),
                 "", DirName, 12);
  if (r==cmOk)
  {
    r=mkdir(MakePath(Drive, Dir, DirName), 0);
    if (r==0)
    {
      UpdateContent(1);
    } else
      MessageBox(msgOk, msgIconInformation,
        View_GetString(strInformation), View_GetString(strMkDirFailed));
  }
}


void CFileSelector::RmFile()
{
  char *s=MakePath(Drive, Dir, SelFile);

  if (MessageBox(msgYes | msgNo, msgIconQuestion,
        View_GetString(strConfirmation), View_GetString(strDeleteFile), s)==
      cmYes)
  {
    int r=0;
    if (IsDir(s)) r=rmdir(s); else r=delete_file(s);
    if (r==0)
    {
      strcpy(FileName, "");
      UpdateContent(1);
    } else
    MessageBox(msgOk, msgIconInformation,
      View_GetString(strInformation), View_GetString(strDeleteFailed));
  }
}


void* CFileSelector::HandleEvent(cmCommand Event, void* Data)
{
  switch (Event)
  {
  case cmInputChanged:
    if (Data && (CInputLine*)Data==FileInput) HandleFile(FileInput->GetInput());
    break;

  case cmListSelectDbl:
    if (Data && (CListbox*)Data==FileList)
    {
      CListItem *i = FileList->GetSelectedItem();
      int Id=i->GetId();
      int r=HandleFile(i->GetText());
      if (Id==2 && r==1) Stop(cmOk);
    }
    break;

  case cmListSelect:
    if (Data && (CListbox*)Data==FileList)
    {
      CListItem *i = FileList->GetSelectedItem();
      if (i->GetId()==2) HandleFile(i->GetText());
      else strcpy(SelFile, i->GetText());
    }
    break;

  case cmComboChanged:
    {
      CCombobox *c = (CCombobox *)Data;
      if (c==MaskList || c==DriveList) HandleFile(c->GetInput());
    }
    break;

  case cmFileMkDir: MakeDir();
                    break;

  case cmFileDel: RmFile();
                  break;

  case cmFileDirUp: HandleFile("..");
                    break;
  default: { };
  }

  return CFileDlg::HandleEvent(Event, Data);
}


void CFileSelector::InsertFileMask(char *aMask)
{
  if (MaskList) MaskList->Insert(aMask);
}


void CFileSelector::UpdateContent(int aDirChanged)
{
  if (FileInput)              FileInput->SetInput(FileName);
  if (FileList && aDirChanged)FileList->FillDir(Drive, Dir, Mask);
  if (DirText)                DirText->SetText(MakePath(Drive, Dir));
  if (DriveList)              DriveList->Select((int)Drive);
  if (MaskList)               MaskList->Select(Mask);

  CFileDlg::UpdateContent(aDirChanged);
}
