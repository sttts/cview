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

 v_color.cc - Color scheme default set
              by Doug Eleveld

*/

#include "v_color.h"


//=========================================


// The instantiation of the default color set
CColorScheme<RGB_FORE,
             RGB_BACK,
             RGB_LIGHT,
             RGB_DARK,
             RGB_SELECT> DefaultColorScheme;


//=========================================


// The interface to the default color scheme
CColorInterface CColorInterface::Default(DefaultColorScheme);


//=========================================


