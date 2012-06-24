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

 v_color.h - Color scheme related classes
             by Doug Eleveld

*/

#ifndef V_COLOR_H_INCLUDED
#define V_COLOR_H_INCLUDED


#include "v_gfx.h"


// Default RGB values for default color scheme
#define RGB_BACK   128, 128, 128
#define RGB_FORE   255, 255, 255
#define RGB_LIGHT  192, 192, 192
#define RGB_DARK    64,  64,  64
#define RGB_SELECT 128, 175, 128


// Color variables
enum colSystemColor { colBack = 0,   // Background (normal) color - should be zero (0)
                colFore,       // Foreground (text) color
                colDark,       // 2 colors for shading
                colLight,
                colSelect,     // Background color of selected objects in lists etc.

                colLastColor   // Marker for the last color so we can count the colors
              };


// Wrapper class for abstract base class for interfacing all the color
// schemes.  It works like a reseatable reference to a ColorScheme.
// All color requests go through it and are passed to a CColorScheme
// object that actually defines the colors
class CColorInterface
{
  public:
    class rep
    {
      private:
        // Getting color out
        virtual CColor Color(const colSystemColor) = 0;

        // Setting a color of the color scheme
        virtual void SetColor(const colSystemColor, int r, int g, int b) = 0;

        // Resetting the color scheme to default
        virtual void Reset() = 0;

      protected:
        // Constructors
        rep (void)
        { };
        virtual ~rep (void)
        { };

      // Let the color interface call the private functions
      // directly.  Derived classes can still overload the
      // virtual functions to extend the functionality
      friend class CColorInterface;
    };

  private:
    rep* interface;

  public:
    // Exemplar for default construction
    static CColorInterface Default;

    // Constructors
    CColorInterface ()
      :interface(Default.interface)
    { };
    CColorInterface (rep& r)
      :interface(&r)
    { };

    // Setting a color scheme
    void operator=(const CColorInterface& aInterface)
    {
      interface = aInterface.interface;
    };

    // The interface that the user will see
    CColor Color(const colSystemColor c)
    {
      return interface->Color(c);
    };
    void SetColor(const colSystemColor c, int r, int g, int b)
    {
      interface->SetColor(c,r,g,b);
    };
    void Reset()
    {
      interface->Reset();
    };
};


//=========================================


// Reference counted color scheme template
template <int FORE_R, int FORE_G, int FORE_B,
          int BACK_R, int BACK_G, int BACK_B,
          int LIGHT_R, int LIGHT_G, int LIGHT_B,
          int DARK_R, int DARK_G, int DARK_B,
          int SELECT_R, int SELECT_G, int SELECT_B>
class CColorScheme
  :public CColorInterface::rep
{
  private:
    // The internal palette
    CColor pal[colLastColor-colBack+1];

  public:
    // The internal interface to the CColorInterface::rep abstract base
    CColor Color(const colSystemColor c)
    {
      return pal[c];
    };
    void SetColor(const colSystemColor c, int r, int g, int b)
    {
      pal[c] = Cgfx::MakeColor(r,g,b);
    };
    void Reset();

  public:
    // Constructors
    CColorScheme (void)
    { };

    CColorScheme (const CColorScheme& rhs)
    {
    for(int i=0;i<colLastColor-colBack+1;i++)
      pal[i] = rhs.pal[i];
    };

    virtual ~CColorScheme (void)
    { };
};


//=========================================


// The forward declaration of the default color interface
//extern CColorInterface ColorInterface;


//=========================================


// The forward declaration of the default color scheme
extern CColorScheme<RGB_FORE,RGB_BACK,RGB_LIGHT,RGB_DARK,RGB_SELECT> DefaultColorScheme;


//=========================================

template <int FORE_R, int FORE_G, int FORE_B,
          int BACK_R, int BACK_G, int BACK_B,
          int LIGHT_R, int LIGHT_G, int LIGHT_B,
          int DARK_R, int DARK_G, int DARK_B,
          int SELECT_R, int SELECT_G, int SELECT_B>
void CColorScheme<FORE_R,FORE_G,FORE_B,
                 BACK_R,BACK_G,BACK_B,
                 LIGHT_R,LIGHT_G,LIGHT_B,
                 DARK_R,DARK_G,DARK_B,
                 SELECT_R,SELECT_G,SELECT_B>::Reset()
{
  pal[colFore] = Cgfx::MakeColor(FORE_R,FORE_G,FORE_B);
  pal[colBack] = Cgfx::MakeColor(BACK_R,BACK_G,BACK_B);
  pal[colLight] = Cgfx::MakeColor(LIGHT_R,LIGHT_G,LIGHT_B);
  pal[colDark] = Cgfx::MakeColor(DARK_R,DARK_G,DARK_B);
  pal[colSelect] = Cgfx::MakeColor(SELECT_R,SELECT_G,SELECT_B);
};

#endif

