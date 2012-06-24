/*

    _______   _    __ __
   /\  ___ \/\ \  /| |\_\
   \ \ \_/\_\ \ \ || |/_/
    \ \ \\/_/\ \ \|| |  __     __  __        __
     \ \ \   _\ \ \| | /\ \  /'__'\\ \  /\  / /
      \ \ \__\ \ \   | \ \ \/\  __/ \ \/  \/ /
       \ \______\ \__|  \ \_\ \____\ \__/\__/
        \/______/\/__/   \/_/\/____/\/__\/_/ WIP 1

           A window library for Allegro

 (c) By Stefan Schimanski & Janko Heilgeist '1997/1998


                  1Stein@gmx.de
              Janko.Heilgeist@gmx.de

            http://1Stein.home.pages.de

 -----------------------------------------------------

 v_const.h - Constants

*/

#ifndef V_CONST_H_INCLUDED
#define V_CONST_H_INCLUDED


// Scaling constants
enum sclScaling { sclNone           = 0,
                  sclLeft           = 1,
                  sclRight          = 1<<1,
                  sclTop            = 1<<2,
                  sclBottom         = 1<<3,

                  // Relative to last object scaling variables
                  sclXOnLeft        = 1<<4,  // Sets the X value on the left/right of previous
                  sclXOnRight       = 1<<5,
                  sclYOnTop         = 1<<6,  // Sets the Y value on the top/bottom of previous
                  sclYOnBottom      = 1<<7,
                  sclSameX          = 1<<8,  // Object to track other objects position
                  sclSameY          = 1<<9,
                  sclSameHeight     = 1<<10, // Object to track other objects sizes
                  sclSameWidth      = 1<<11,

                  // Relative to last sclRef object scaling variables
                  sclRef            = 1<<12, // Object as a reference for later objects to position themselves
                  sclRefRelative    = 1<<13, // Position object w.r.t last sclRef object

                  sclLastScaling    = 0xffffffff, // Ensure 32 bit enum

                  // Composite scaling flags
                  sclSameSize       = sclSameWidth | sclSameHeight,
                  sclSideRight      = sclXOnRight | sclSameY,
                  sclSideLeft       = sclXOnLeft | sclSameY,
                  sclSideTop        = sclSameX | sclYOnTop,
                  sclSideBottom     = sclSameX | sclYOnBottom
                };


const strOk                     = 0;
const strCancel                 = 1;
const strYes                    = 2;
const strNo                     = 3;
const strRetry                  = 4;
const strIgnore                 = 5;
const strInvalidPath            = 6;
const strFileNeeded             = 7;
const strFileMustExist          = 8;
const strDriveChangeForbidden   = 9;
const strInvalidDrive           = 10;
const strDirChangeForbidden     = 11;
const strMaskChangeForbidden    = 12;
const strMkDirFailed            = 13;
const strInformation            = 14;
const strMakeDir                = 15;
const strDirName                = 16;
const strConfirmation           = 17;
const strDeleteFile             = 18;
const strDeleteFailed           = 19;


const butLeft                   = 0x0001;
const butRight                  = 0x0002;
const butDoubleClick            = 0x1000;
const butDoubleClickLeft        = 0x1001;
const butDoubleClickRight       = 0x1002;

#endif
