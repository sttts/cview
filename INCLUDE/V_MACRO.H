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

 v_macro.h - Gui macro classes by Doug Eleveld

*/

//#include <time.h>

//============================================================

class MacroElement
{
  private:
    int Ascii;
    int Scancode;
    int mx;
    int my;
    int mb;

    MacroElement* next;

    MacroElement ();
    MacroElement (int Ascii, int Scancode, int mx, int my, int mb);
    MacroElement (const MacroElement& other);

    void operator=(const MacroElement& other);

    bool operator==(const MacroElement& other);

  friend class CMacro;
};


class CMacro
{
  private:
    MacroElement base;
    MacroElement* last;

    MacroElement* current;

  public:
    CMacro ();
    ~CMacro ();

    bool Empty ()
       {
       if (last==NULL)
          return true;
       return false;
       }

    void Reset ();
    bool Append (int Ascii, int Scancode, int mx, int my, int mb);
    bool Playback (int& Ascii, int& Scancode, int& mx, int& my, int& mb);
};



