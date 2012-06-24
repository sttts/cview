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

 v_save1.cc - Serializing cleanup functions

*/


#include <stdio.h>
#include <string.h>
#include "v_save.h"
#include "v_load.h"


//===============================================================


struct CleanupInfo { char* Cleaned;
                     char* Finished;
                     int Level;
                   };

bool CleanupObjectSerializeInfo (CleanupInfo* info)
{
  char* name;
  bool found_base = false;

  // Skip stuff till we come to a vaild object
  char* type = ReadTag(&info->Finished);

  while(type)
  {
    if(strcmp(type,"{")==0)
    {
      info->Level++;
      break;
    }
    if(strcmp(type,"}")==0)
      info->Level--;

    type = ReadTag(&info->Finished);
  }
  // Make sure we found something
  if(!info->Finished)
    return false;

  // Check the next tag
  type = ReadTag(&info->Finished);
  
  // Read the name and type off
  name = ReadTag(&info->Finished);

  // Find out if this is supposed to be base class
  if((strcmp(type,"CView")==0)||(strcmp(type,"CGroupEnd")==0))
    found_base = true;

  // Add those to the output file
  for(int i=0;i<info->Level;i++)
    strcat(info->Cleaned," ");
    
  strcat(info->Cleaned,type);
  strcat(info->Cleaned," ");
  strcat(info->Cleaned,name);
  strcat(info->Cleaned,"\n");
  
  for(int i=0;i<info->Level;i++)
    strcat(info->Cleaned," ");

  // Read tags in and ignore internal types and names
  char* temp = ReadTag(&info->Finished);

  while(temp)
  {
    if(strcmp(temp,"{")==0)
    {
      info->Level++;

      // If the next entry is the base class
      // then we have found the base class
      temp = ReadTag(&info->Finished);
      if((strcmp(temp,"CView")==0)||(strcmp(temp,"CGroupEnd")==0))
        found_base = true;

      // Even if it wasn't we still skip the name
      temp = ReadTag(&info->Finished);
    }

    else if(strcmp(temp,"}")==0)
    {
      info->Level--;

      // If this closes the base, then the
      // next is a new object
      if(found_base==true)
      {
        strcat(info->Cleaned,"\n");
        if(CleanupObjectSerializeInfo(info)==false)
          return false;
      }
    }
    else
    {
      strcat(info->Cleaned,temp);
      strcat(info->Cleaned," ");
    }

    // Read the next tag in
    temp = ReadTag(&info->Finished);
  }
  return false;
}
                             

char* CleanupSerializeInfo (char* info)
{
  CleanupInfo Data;
  Data.Finished = info;
  Data.Cleaned = new char[strlen(info)+2];
  Data.Level = -1;
  strcpy(Data.Cleaned,"");

  CleanupObjectSerializeInfo(&Data);

  delete info;
  return Data.Cleaned;
}
