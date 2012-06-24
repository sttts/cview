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

 v_string.cc - String management

*/


#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v_string.h"


//==========================================================

char** Strings = NULL;

int StringNum=0;

char InvalidString[] = "INVALID STRING";


//==========================================================


void View_AddString(char *Text, int Num)
{
  if (Num<StringNum)
  {
    if (Strings[Num]!=NULL) free(Strings[Num]);

    Strings[Num] = strdup(Text);
  }
}


int View_LoadStrings(char *FileName)
{
  ifstream s(FileName);

  char Buf[1024];
  int Num;

  while (!s.eof())
  {
    Num = -1;
    s >> Num;
    s.get();
    s.get(Buf, 256);
    if (Num>=0)
    {
      View_AddString(Buf, Num);
    }
  }

  return 0;
}


int View_InitStrings(char *FileName, int MaxStrings=1024)
{
  Strings = (char**)malloc(MaxStrings*sizeof(char*));
  StringNum = MaxStrings;

  for (int n=0; n<StringNum; n++) Strings[n] = NULL;

  return View_LoadStrings(FileName);
}


void View_DoneStrings()
{
  for (int n=0; n<StringNum; n++) free(Strings[n]);
  free(Strings);
}


char *View_GetString(int Num)
{
  if (Num>=StringNum || Strings[Num]==NULL) return InvalidString; else
    return Strings[Num];
}

