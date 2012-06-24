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

 v_menust.h - Various menu drawing style classes
              by Doug Eleveld
 
*/


#ifndef V_MENUST_H_INCLUDED
#define V_MENUST_H_INCLUDED


class CMenu;
class CMenuItem;


// Wrapper class for abstract base class for interfacing all the border
// drawing.  It works like a reseatable reference.
class CMenuStyle
{
 public:
  // ABC (abstract base class) for border drawing interface
  class rep
  {
   private:
     // These functions must do the actual border drawing
     virtual void Back(CMenu& aTarget) = 0;
     virtual void HighlightBar(CMenu& aTarget, int buttons, CMenuItem *Item, int x, int y, CMenuItem *Active) = 0;
     virtual void HighlightPopup(CMenu& aTarget, int buttons, CMenuItem *Item, int x, int y, CMenuItem *Active) = 0;

     // These functions determine drawing style
     virtual int SpaceLeft() = 0;
     virtual int SpaceRight() = 0;
     virtual int SpaceTop() = 0;
     virtual int SpaceBot() = 0;

   // Only the CBorder will interface these functions
   friend class CMenuStyle;
  };

  // The default border type
  class CDefault:public rep
  {
   private:
     virtual void Back(CMenu& aTarget);
     virtual void HighlightBar(CMenu& aTarget, int buttons, CMenuItem *Item, int x, int y, CMenuItem *Active);
     virtual void HighlightPopup(CMenu& aTarget, int buttons, CMenuItem *Item, int x, int y, CMenuItem *Active);
     
     // These functions determine drawing style
     virtual int SpaceLeft() { return 5; };
     virtual int SpaceRight() { return 5; };
     virtual int SpaceTop() { return 0; };
     virtual int SpaceBot() { return 2; };
  };

 private:
  rep* interface;

 public:
  // Constructors
  CMenuStyle ()
    :interface(&DefaultDrawer)
  { };
  CMenuStyle (const CMenuStyle& d)
    :interface(d.interface)
  { };
  CMenuStyle (rep& r)
    :interface(&r)
  { };
  virtual ~CMenuStyle (void)
  { };

  // Setting a new drawer
  void operator=(rep& r)
  {
    interface = &r;
  };

  // Interface to internal drawing code
  void Back(CMenu& aTarget)
  {
    interface->Back(aTarget);
  };
  void HighlightBar(CMenu& aTarget, int buttons, CMenuItem *Item, int x, int y, CMenuItem *Active)
  {
    interface->HighlightBar(aTarget,buttons,Item,x,y,Active);
  };
  void HighlightPopup(CMenu& aTarget, int buttons, CMenuItem *Item, int x, int y, CMenuItem *Active)
  {
    interface->HighlightPopup(aTarget,buttons,Item,x,y,Active);
  };
  int SpaceLeft()
  {
    return interface->SpaceLeft();
  };
  int SpaceRight()
  {
    return interface->SpaceRight();
  };
  int SpaceTop()
  {
    return interface->SpaceTop();
  };
  int SpaceBot()
  {
    return interface->SpaceBot();
  };


  // The default border drawing style
  static CDefault DefaultDrawer;

  // The default border drawing style
  static CMenuStyle Default;
};


//---------------------------------------------------------------------------
// Exchangable dialog border drawers
//---------------------------------------------------------------------------

// Flat style dialog border type
class CMenuStyleBox:public CMenuStyle::rep
{
 private:
   virtual void Back(CMenu& aTarget);
   virtual void HighlightBar(CMenu& aTarget, int buttons, CMenuItem *Item, int x, int y, CMenuItem *Active);
   virtual void HighlightPopup(CMenu& aTarget, int buttons, CMenuItem *Item, int x, int y, CMenuItem *Active);
   
   virtual int SpaceLeft() { return 3; };
   virtual int SpaceRight() { return 3; };
   virtual int SpaceTop() { return 0; };
   virtual int SpaceBot() { return 2; };
};

#endif
