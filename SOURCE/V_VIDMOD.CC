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

 v_vidmod.cc - Video Mode selector dialog class
               by Doug Eleveld

*/


#include <string.h>
#include "v_vidmod.h"
#include "v_text.h"
#include "v_combo.h"
#include "v_button.h"
#include "v_common.h"
#include "v_msg.h"
#include "v_save.h"


//===================================================


CVideoModeDlg::CVideoModeDlg()
 : CDialog(1, 1, 20, 10, "Video Mode", dlgClose|
                                       dlgDrag|
                                       dlgEscape),
   Size(new CText(CView::DefaultSpacing,CView::DefaultSpacing,"Resolution")),
   Depth(new CText(1,1,"Depth")),
   OkButton(new CTextButton(1, 1, 1, 1, "Ok",cmOk)),
   CancelButton(new CTextButton(1, 1, 1, 1, "Cancel",cmCancel)),
   SizeList(new CCombobox(1, 1, 100, 100, "", comboPopup)),
   DepthList(new CCombobox(1, 1, 100, 100, "", comboPopup))
{
  Size->DefaultScale();
  Depth->DefaultScale();
  CancelButton->DefaultScale();
  OkButton->ScaleTo(CancelButton->Width(),CancelButton->Height());
//  SizeList->DefaultScale();
//  DepthList->DefaultScale();

  Client->Insert(Size);
  Client->Insert(SizeList);
  Client->Insert(DepthList);
  Client->Insert(Depth);
  Client->Insert(OkButton);
  Client->Insert(CancelButton);

  Size->SetScaleMode(sclRef);
  SizeList->SetScaleMode(sclSideRight);
  DepthList->SetScaleMode(sclSideBottom);
  Depth->SetScaleMode(sclSideLeft);
  
  OkButton->MoveTo(CView::DefaultSpacing,DepthList->Posy()+DepthList->Height()+CView::DefaultSpacing);
  CancelButton->SetScaleMode(sclSideRight);

  // Add the elements of the mode list
  int i = 0;
  char* ptr = Cgfx::Modes[i];
  while(ptr!=NULL)
  {
    SizeList->Insert(ptr);
    ++i;
    ptr = Cgfx::Modes[i];
  }
  
  // Add the elements of the depth list
  i = 0;
  ptr = Cgfx::Depths[i];
  while(ptr!=NULL)
  {
    DepthList->Insert(ptr);
    ++i;
    ptr = Cgfx::Depths[i];
  }

  Enclose();
  Center();
}


CVideoModeDlg::~CVideoModeDlg()
{
}


void *CVideoModeDlg::HandleEvent(cmCommand Event, void* Data)
{
  // We were OK'd so go ahead and change the mode
  if(Event==cmOk)
  {
    // Extract the width and height from the string
    char* ModeString = strdup(SizeList->GetInput());
  
    int Width = Cgfx::Width();
    int NewWidth = 0;
    
    int Height = Cgfx::Height();
    int NewHeight = 0;

    char *ptr = strtok(ModeString,"x-, ");
    if(ptr!=NULL)
      NewWidth = atoi(ptr);

    if(NewWidth>0)
      Width = NewWidth;
      
    ptr = strtok(NULL,"x-, ");
    if(ptr!=NULL)
      NewHeight = atoi(ptr);

    if(NewHeight>0)
      Height = NewHeight;

    free(ModeString);

    // Extract the depth
    int Depth = Cgfx::Depth();
    int NewDepth = atoi(DepthList->GetInput());

    if(NewDepth>0)
      Depth = NewDepth;

    // Actually change the graphic mode
    if (View_SetVideoMode(Width,Height,Depth)==false)
    {
      // Inform the user that the video mode would not change
      MessageBox(msgCancel, msgIconExclamation, "Error", "Could not set video mode");
    }

    // Stop for the video mode change
    Stop(cmVideoMode);
  }
  else if(Event==cmSerialize)
  {
    char info[1000];
    sprintf(info,"{ CVideoModeDlg P%p }\n",this);
    AddSerializeInfo(Data,info);
  }
  
  return CDialog::HandleEvent(Event,Data);
}

