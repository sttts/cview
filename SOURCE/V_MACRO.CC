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

 v_macro.cc - Gui macro classes by Doug Eleveld

*/


#include <stdlib.h>
#include <time.h>
#include "v_macro.h"

#include <stream.h>

#define MINIMAL_MACRO   // Don't record non-essential mouse movements

//============================================================


MacroElement::MacroElement ()
  :Ascii(0),
  Scancode(0),
  mx(0),
  my(0),
  mb(0),
  next(NULL)
{
}


MacroElement::MacroElement (int aAscii, int aScancode, int amx, int amy, int amb)
  :Ascii(aAscii),
  Scancode(aScancode),
  mx(amx),
  my(amy),
  mb(amb),
  next(NULL)
{
}


MacroElement::MacroElement (const MacroElement& other)
  :Ascii(other.Ascii),
  Scancode(other.Scancode),
  mx(other.mx),
  my(other.my),
  mb(other.mb),
  next(NULL)
{
}


void MacroElement::operator=(const MacroElement& other)
{
  Ascii = other.Ascii;
  Scancode = other.Scancode;
  mx = other.mx;
  my = other.my;
  mb = other.mb;
}


bool MacroElement::operator==(const MacroElement& other)
{
  if ((Ascii==other.Ascii)&&
      (Scancode==other.Scancode)&&

#ifdef MINIMAL_MACRO
      (mx==other.mx)&&
      (my==other.my)&&
#endif

      (mb==other.mb))
      return true;
  return false;
}


//============================================================


CMacro::CMacro ()
  :last(NULL)
{
}


CMacro::~CMacro ()
{
  Reset();
}


void CMacro::Reset ()
{
  MacroElement* ptr = base.next;

  while(ptr!=NULL)
  {
    MacroElement* elem = ptr->next;
    delete ptr;
    ptr = elem;
  }
  last = NULL;
}


// Record a step for a macro.  If the step is the same as the last,
// don't record anything.
bool CMacro::Append (int Ascii, int Scancode, int mx, int my, int mb)
{
  // Check if it's the first element
  if (last==NULL)
  {
    base.Ascii = Ascii;
    base.Scancode = Scancode;
    base.mx = mx;
    base.my = my;
    base.mb = mb;
    last = &base;

    return true;
  }
  // Check if it is different than the last element
  else
  {
    MacroElement temp(Ascii,Scancode,mx,my,mb);

    if (temp==*last)
      return false;

    // It is different, so make a new element and link it in
    MacroElement* newone = new MacroElement(temp);

    // Set the links for the Element and the last element pointer
    last->next = newone;
    last = newone;

    return true;
  }
}

// Playback a macro by adjusting the key and mouse values
// return false when playback is completed
bool CMacro::Playback (int& Ascii, int& Scancode, int& mx, int& my, int& mb)
{
  // Check to see if there is a macro there
  if (base.next==NULL)
    return false;

  // Check to see if we just started playing back
  if (current==NULL)
  {
    current = &base;

    Ascii = base.Ascii;
    Scancode = base.Ascii;
    mx = base.mx;
    my = base.my;
    mb = base.mb;

    current = base.next;

    return true;
  }

  // Replay the data
  Ascii = current->Ascii;
  Scancode = current->Ascii;
  mx = current->mx;
  my = current->my;
  mb = current->mb;

  // Check if it is time to go to the next element
  current = current->next;

  // Check to see if it's our last element
  if (current==NULL)
    return false;

  return true;
}

