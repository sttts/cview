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

 v_file.h - File selector dialog class

*/


#ifndef V_FILE_H_INCLUDED
#define V_FILE_H_INCLUDED


#include "v_dialog.h"


enum fileFlags { fileNone        = 0,
                 fileMustExist   = 1,
                 fileDirs        = 1<<1,
                 fileFiles       = 1<<2,
                 fileDrives      = 1<<3,
                 fileMask        = 1<<4,
                 fileInputLine   = 1<<5,
                 fileMkDir       = 1<<6,
                 fileDelFile     = 1<<7,
                 fileFileNeeded  = 1<<8,
                 fileDirUp       = 1<<9,
                 fileScaleable   = 1<<10,
                 
                 fileDefault     = fileMustExist |
                                   fileDirs |
                                   fileFiles |
                                   fileDrives |
                                   fileMask |
                                   fileInputLine |
                                   fileMkDir |
                                   fileFileNeeded |
                                   fileDirUp
               };

               
fileFlags operator~(const fileFlags one);

fileFlags operator|(const fileFlags one, const fileFlags two);
fileFlags operator&(const fileFlags one, const fileFlags two);


class CFileDlg:public CDialog
{
 public:
  CFileDlg(int apx, int apy, int asx, int asy, char *aTitle, int aFlags=fileDefault);
  ~CFileDlg();

  char *GetPath();
  char *GetDir();
  char *GetMask();
  char *GetFileName();
  char GetDrive();

 protected:
  int Flags;

  char Drive;
  char Dir[256];
  char Mask[32];
  char FileName[32];
  char SelFile[32];

  virtual int CanClose();
  virtual void UpdateContent(int aDirChanged=1);
  int HandleFile(char *aFile, int aVerify=1);
};


class CFileSelector:public CFileDlg
{
 public:
  CFileSelector(char *aPath, char *aMask, char *aTitle, char *aButton,
           fileFlags aFlags=fileDefault);
  ~CFileSelector();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  virtual void CorrectSize(int &x, int &y);

  void InsertFileMask(char *aMask);

 protected:
  class CFileList *FileList;
  class CText *DirText;
  class CFileInput *FileInput;
  class CCombobox *DriveList;
  class CCombobox *MaskList;

  virtual void UpdateContent(int aDirChanged=1);
  void MakeDir();
  void RmFile();
};


#endif
