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

 v_load.h - Code for loading objects from cmSerialized objects

*/

#ifndef V_LOAD_H_INCLUDED
#define V_LOAD_H_INCLUDED



class CView;


typedef CView*(*LoadFunc)(char** data, CView* obj);


struct TypeEntry { char* TypeName;
                   LoadFunc Func;
                 };


extern TypeEntry ObjectType[];


TypeEntry* IsTypeName (TypeEntry* ptr, const char* name);


// Read a tag (next important word) from the input stream
// Here comments are ignored
char* ReadTag (char** data);


#endif
