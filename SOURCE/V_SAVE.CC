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

 v_save.cc - Serializing helper functions

*/


#include <stdio.h>
#include <string.h>
#include "v_save.h"


//===============================================================


void AddSerializeInfo (void* Data, const char* info)
{
  // Get the pointer to the real string data
  char* one = NULL;

  char** stuff = (char**)Data;
  
  if(Data!=NULL)
    one = *stuff;

  // Find out how long the internal data is
  int len = 0;
  if(one!=NULL)
    len = strlen(one);
  if(info!=NULL)
    len += strlen(info);

  // Make space for the new complete data
  char* ret = new char[len+2];
  
  // Copy the original data over
  if(one!=NULL)
    strcpy(ret,one);
  else
    strcpy(ret,"");
    
  // Copy the new data over
  if(info!=NULL)
    strcat(ret,info);

  // Cleanup the old
  if(one!=NULL)
    delete one;

  // Put the Data back in
  *stuff = ret;
}


