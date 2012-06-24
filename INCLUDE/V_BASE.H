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

 v_base.h - Base classes

*/


#ifndef V_BASE_H_INCLUDED
#define V_BASE_H_INCLUDED


#include <stdlib.h>
#include "v_gfx.h"
#include "v_const.h"
#include "v_color.h"
#include "v_comman.h"


class CView
{
 public:
  CView(int apx, int apy, int asx, int asy);
  virtual ~CView();

  virtual void HandleMouse(int mx, int my, int buttons);
  virtual void MouseLost(int mx, int my, int buttons);
  virtual void MouseGot(int mx, int my, int buttons);
  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  virtual void* HandleKey(char Char, char Scancode);
  virtual void* HandleAccel(char Char, char Scancode);

  virtual void PrepareDraw();
  virtual void Draw(); // TODO: replace all draw methods with new version
  virtual void Draw(int ax, int ay, int asx, int asy)
    { (void)ax; (void)ay; (void)asx; (void)asy; Draw(); }
  virtual void Redraw();
  virtual void Action();

  virtual int Focus();
  virtual int DeFocus();
  void Drag(int mx=0, int my=0);
  void Drop();
  void Kill();
  virtual int CanClose();

  CView *Next, *Prev;

  void Move(int x,int y);

  void Scale(int x, int y);
  virtual void DefaultScale();
  virtual void MoveTo(int x, int y);
  virtual void ScaleTo(int x, int y);
  void SetScaleMode(sclScaling aScaleMode, int offset = CView::DefaultSpacing);
  inline void SetValue(cmCommand val) { Value = val; };
  void GetScaleMode(sclScaling &aScaleMode, int &offset) { aScaleMode = ScaleMode; offset = ScaleModeOffset; };
  
  virtual int IsIn(int x, int y);
  virtual int IsIn(int ax, int ay, int bx, int by);
  inline int Height() { return sy; }
  inline int Width() { return sx; }
  inline int Posx() { return px; }
  inline int Posy() { return py; }
  inline int IsTransparent() { return Transparent; }
  inline void SetTransparent(int aTransparent) { Transparent=aTransparent; }
  inline cmCommand GetValue() { return Value; };

  virtual CColor Color(colSystemColor Num);
  void MakeLocal(int &x, int &y, CView *Origin);
  void MakeGlobal(int &x, int &y, CView *Target);
  virtual BITMAP *GetMouse(int &rx, int &ry);
  void SetMouse(BITMAP *aMouse, int rx=0, int ry=0);

  // Default spacing for adding objects etc...
  static int DefaultSpacing;

  // The color interface in use i.e. color scheme
  // The object is small, and only contains a pointer
  CColorInterface ColorInterface;

 protected:
  friend class CGroup;

  class CGroup *Owner;

  int px, py; // position
  int sx, sy; // size

  cmCommand Value;
  sclScaling ScaleMode;
  int ScaleModeOffset;
  int Oldmx, Oldmy, Oldbuttons;
  int Dragging;
  BITMAP *Mouse;
  int Mouserx, Mousery;
  int SentencedToDeath;

  void Adjust(int dx, int dy);
  virtual void RedrawOwner();
  CView* FindPrevsclRef();
  virtual void CreateBuffer();
  virtual void DestroyBuffer();

 private:
  int Transparent; // first draw background, then this view
};



class CBufView:public CView
{
 public:
  CBufView(int apx, int apy, int asx, int asy);
  virtual ~CBufView();

  virtual void Draw(int ax, int ay, int asx, int asy);
  virtual void Redraw();
  virtual void MoveTo(int x, int y);
  virtual void ScaleTo(int x, int y);

  virtual void* HandleEvent(cmCommand Event, void* Data=0);

  inline BITMAP *GetBuffer() {if (!Buffer) CreateBuffer(); return Buffer;}

  enum BUFFER_T { BufferFull,
                  BufferSub,
                  BufferExternal
                };
                
 protected:
  int RedrawNextTime;

  virtual void CreateBuffer();
  virtual void DestroyBuffer();

  void SetBuffer(BITMAP *Buf);
  void SetBuffer(BUFFER_T B);
  
  inline BUFFER_T GetBufferType() {return BufferType;}

 private:
  BITMAP *Buffer;
  BUFFER_T BufferType;

//  bool ExternalBitmap;

 friend class CBitmap;
};



class CGroup:public CBufView
{
 public:
  CGroup(int apx, int apy, int asx, int asy);
  virtual ~CGroup();

  virtual void HandleMouse(int mx, int my, int buttons);
  virtual void MouseLost(int mx, int my, int buttons);
  virtual void MouseGot(int mx, int my, int buttons);
  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  virtual void* HandleAccel(char Char, char Scancode);

  virtual void PrepareDraw();
  virtual void Draw(int ax, int ay, int asx, int asy);
  virtual void Update(CView *aClient, int ax, int ay, int asx, int asy);
  inline void Update(int ax, int ay, int asx, int asy)
    { Update(NULL, ax, ay, asx, asy); }

  virtual void Action();

  virtual void Insert(CView *View);
  virtual void Remove(CView *View);

  // Move an object to the front or back of the group
  // It must already be in the list
  virtual void Front(CView *View);
  virtual void Back(CView *View);

  virtual void ScaleTo(int x, int y);
  virtual BITMAP *GetMouse(int &rx, int &ry);

  CView *GetFirstView() { return First; }
  CView *GetLastView() { return Last; }

  virtual int CanClose();

  virtual void Enclose(const int border = CView::DefaultSpacing);

 protected:
  virtual void CreateBuffer();
  virtual void DestroyBuffer();

  enum DRAWING_T { DrawingPainter,
                   DrawingRecursive,
                 };
                 
  inline void SetDrawing(DRAWING_T B) {DrawingType=B; Redraw();}
  inline DRAWING_T GetDrawingType() {return DrawingType;}

  CView *First;
  CView *Last;
  CView *OldMouseIn;
  int Adjusting;
  int Running;

 private:
  friend class CProgram;

  virtual void DrawRect(int ax, int ay, int bx, int by);
  void DrawRecursive(int ax, int ay, int bx, int by, CView* v);

  DRAWING_T DrawingType;
};


class CWindow:public CGroup
{
 public:
  CWindow(int apx, int apy, int asx, int asy);
  virtual ~CWindow();

  virtual void* HandleEvent(cmCommand Event, void* Data=0);
  virtual void* HandleKey(char Char, char Scancode);

  virtual void MoveTo(int x, int y);
  virtual void ScaleTo(int x, int y);

  cmCommand Run(int aForeground=0);
  int Stop(const cmCommand aValue=cmOk);
  void ToFront();

 protected:
  virtual void RedrawOwner();

  virtual void GetUserInput(int& Ascii, int& Scancode, int& mx, int& my, int& but);
};


#endif
