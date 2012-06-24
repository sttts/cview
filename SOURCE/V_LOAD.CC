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

 v_load.cc - Code for loading objects from cmSerialized objects

*/


#include <stdlib.h>
#include <stdio.h>
#include <stream.h>
#include "v_load.h"

#include "v_base.h"


//=====================================================


CView* LoadCView(char** data, CView* obj);
CView* LoadCBufView(char** data, CView* obj);
CView* LoadCGroup(char** data, CView* obj);
CView* LoadCWindow(char** data, CView* obj);
CView* LoadCDialog(char** data, CView* obj);
CView* LoadCButton(char** data, CView* obj);
CView* LoadCTextButton(char** data, CView* obj);


TypeEntry ObjectType[] = {
                           { "CView",        LoadCView,        },
                           { "CBufView",     LoadCBufView,     },
                           { "CGroup",       LoadCGroup,       },
                           { "CWindow",      LoadCWindow,      },
                           { "CDialog",      LoadCDialog,      },
                           { "CButton",      LoadCButton,      },
                           { "CTextButton",  LoadCTextButton,  },
                           
                           { "CMenu",        NULL,             },
                           { "CMenuBar",     NULL,             },
                           { "CPopupMenu",   NULL,             },
                           { "CPicture",     NULL,             },
                           
                           { NULL,           NULL              }
                         };

// Read a tag (next important word) from the input stream
// Here comments are ignored
char* ReadTag (char** dataptr)
{
  bool comment = false;
  bool instring = false;

  char* start = NULL;

  if(dataptr==NULL)
    return NULL;
    
  char* data = *dataptr;

  while(*data)
  {
    switch(*data)
    {
      case -1:
      case '\0':
        *dataptr = NULL;
        return start;

      case '\n':
        if(!instring)
          comment = false;
        /* Fall through */

      case ' ':
      case '\t':
      case '=':
        if(!comment && start && !instring)
        {
          *data = '\0';
          *dataptr = data+1;

          return start;
        }
        break;

      // Comment starting stuff
      case '/':
        if(*(data+1)=='/' && !instring)
        {
          comment = true;
          data++;
        }
        break;

      default:
        // Check if we start the string
        if(!comment && !start)
        {
          start = data;

          // Check if we started a string
          if(*start=='\"')
            instring = true;
        }
        // Otherwise just add another char
        else
        {
          // Check if we stop a string
          if((*data=='\"')&&(*(data-1)!='\\'))
            instring = false;
        }
        break;
    }

    data++;
  }
  // We ran out of stuff to parse
  return start;
}


int ReadTagInt (char** dataptr)
{
  char* out = ReadTag(dataptr);
  return atoi(out);
}

                         

TypeEntry* IsTypeName (TypeEntry* ptr, const char* name)
{
  while(ptr->TypeName!=NULL)
  {
    // Does the name match a Typename
    if(strcmp(ptr->TypeName,name)==0)
      return ptr;

    ptr++;
  }

  return NULL;
}

CView* LoadObject(char** input)
{
  // Try to read in the type name
  char* data = ReadTag(input);

  // Make sure it's a type name
  TypeEntry* index = IsTypeName(ObjectType,data);
  if(index!=NULL)
  {
    // Call the related load function
    return index->Func(input,NULL);
  }
  return NULL;
}


//=====================


CView* CViewSetpx(char** data, CView* obj)
{
  obj->MoveTo(ReadTagInt(data),obj->Posy());
  return obj;
}


CView* CViewSetpy(char** data, CView* obj)
{
  obj->MoveTo(obj->Posx(),ReadTagInt(data));
  return obj;
}


CView* CViewSetsx(char** data, CView* obj)
{
  obj->ScaleTo(ReadTagInt(data),obj->Height());
  return obj;
}


CView* CViewSetsy(char** data, CView* obj)
{
  obj->ScaleTo(obj->Width(),ReadTagInt(data));
  return obj;
}



CView* CViewSetValue(char** data, CView* obj)
{
  obj->SetValue(cmCommand(ReadTagInt(data)));
  return obj;
}


CView* CViewSetScaleMode(char** data, CView* obj)
{
  sclScaling Scale;
  int Spacing;
  obj->GetScaleMode(Scale,Spacing);
  
  Scale = (sclScaling)ReadTagInt(data);

  obj->SetScaleMode(Scale,Spacing);
  return obj;
}


CView* CViewSetScaleModeOffset(char** data, CView* obj)
{
  sclScaling Scale;
  int Spacing;
  obj->GetScaleMode(Scale,Spacing);
  
  Spacing = ReadTagInt(data);

  obj->SetScaleMode(Scale,Spacing);
  return obj;
}


CView* CViewSetTransparent(char** data, CView* obj)
{
  obj->SetTransparent(ReadTagInt(data));
  return obj;
}



TypeEntry CViewKeywords[] = {
                              { "px"              ,CViewSetpx              },
                              { "py"              ,CViewSetpy              },
                              { "sx"              ,CViewSetsx              },
                              { "sy"              ,CViewSetsy              },
                              { "Value"           ,CViewSetValue           },
                              { "ScaleMode"       ,CViewSetScaleMode       },
                              { "ScaleModeOffset" ,CViewSetScaleModeOffset },
                              { "Transparent"     ,CViewSetTransparent     },
                              { NULL              ,NULL                    }
                            };


CView* LoadCView(char** data, CView* obj)
{
  // Maybe we have to allocate a new object
  if(obj==NULL)
    obj = new CView(100,100,100,100);

  while(1)
  {
    char* Tag = ReadTag(data);

    TypeEntry* index = IsTypeName(CViewKeywords,Tag);
    if(index!=NULL)
    {
      // Call the related load function
      obj = index->Func(data,obj);
    }
    else
      break;
  }
  return obj;
}


CView* LoadCBufView(char** data, CView* obj) { };
CView* LoadCGroup(char** data, CView* obj) { };
CView* LoadCWindow(char** data, CView* obj) { };
CView* LoadCDialog(char** data, CView* obj) { };
CView* LoadTitlebar(char** data, CView* obj) { };
CView* LoadClient(char** data, CView* obj) { };
CView* LoadCButton(char** data, CView* obj) { };
CView* LoadCTextButton(char** data, CView* obj) { };
CView* LoadCCloseButton(char** data, CView* obj) { };


