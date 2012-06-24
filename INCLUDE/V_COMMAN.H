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

 v_comman.h - Constants and classes for command handling

*/

#ifndef V_COMMAND_H_INCLUDED
#define V_COMMAND_H_INCLUDED


enum cmCommand { cmNone = 0,
                 cmStop,          // Data = Value
                 cmOk,
                 cmCancel,
                 cmClose,
                 cmDrag,          // Data = Dragx + Dragy << 16
                 cmLeft,
                 cmRight,
                 cmUp,
                 cmDown,
                 cmYes,
                 cmNo,
                 cmRetry,
                 cmIgnore,
                 cmVideoMode,
                 cmWindowBufferMode, // Data = &CBufView::BUFFER_T
                 cmScrolled,      // Data = &CScrollbar
                 cmListSelect,    // Data = &CListbox
                 cmRadioSelect,   // Data = &CRadioButton
                 cmButtonSelect,  // Data = &CButton
                 cmCheckSelect,   // Data = &CCheckbox
                 cmInputChanged,  // Data = &CInputLine
                 cmComboChanged,  // Data = &CCombobox
                 cmComboPopup,
                 cmFileMkDir,
                 cmFileDirUp,
                 cmFileDel,
                 cmListSelectDbl, // Data = &CListbox
                 cmDragger,       // Data = Pos
                 cmTopWindow,
                 cmNotTopWindow,
                 cmQuit,
                 cmEscape,        // When a user escapes a window

                 cmColorScheme,   // Use a color scheme: Data = &CColorInterface::rep

                 cmDialogStyle,   // Dialog change border style: Data = &CDialogBorderStyle::rep
                 cmMenuStyle,     // Menu change style: Data = &CMenuStyle::rep

                 cmSerialize,     // Write description of object

                 cmUser,          // User may define commands higher than this
                 
                 cmLastCommand = 0xffffffff // Forces the enum to 32 bits
                                            // so that the user can place
                                            // some after the cmUser command
               };

/*
// Helper class for making new commands
class Command
{
 private:
   static int index;

   Command() { };
   
   // unimplemented and private
   Command(const Command&);
   operator=(const Command&);

 public:
   ~Command() { };
  
   operator cmCommand()
   {
     index++;
     return (cmCommand)index;
   }

   static Command Declare;

   // Avoid a djgpp warning that is wrong
   class stuff { };
   friend stuff;
};

*/


#endif
