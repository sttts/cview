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

 v_bord1.h - Various Dialog border classes
 
*/


#ifndef V_BORDER_H_INCLUDED
#define V_BORDER_H_INCLUDED


class CDialog;
class CBorder;
class CTitlebar;
class CClient;
class CCloseButton;


// Wrapper class for abstract base class for interfacing all the border
// drawing.  It works like a reseatable reference.
class CDialogBorderStyle
{
 public:
  // ABC (abstract base class) for border drawing interface
  class rep
  {
   private:
     // This function must do the actual border drawing
     virtual void Draw(CDialog& aTarget, CBorder& aBorder) = 0;

     // These function position the subobjects correctly
     virtual void PositionTitle(CDialog& aTarget, CTitlebar& aTitle) = 0;
     virtual void PositionClient(CDialog& aTarget, CClient& aClient) = 0;
     virtual void PositionClose(CDialog& aTarget, CCloseButton& aClose) = 0;

   // Only the CBorder will interface these functions
   friend class CDialogBorderStyle;
  };

  // The default border type
  class CDefault:public rep
  {
   private:
     virtual void Draw(CDialog& aTarget, CBorder& aBorder);
     
     virtual void PositionTitle(CDialog& aTarget, CTitlebar& aTitle);
     virtual void PositionClient(CDialog& aTarget, CClient& aClient);
     virtual void PositionClose(CDialog& aTarget, CCloseButton& aClose);
  };

 private:
  rep* interface;

 public:
  // Constructors
  CDialogBorderStyle ()
    :interface(&DefaultDrawer)
  { };
  CDialogBorderStyle (const CDialogBorderStyle& d)
    :interface(d.interface)
  { };
  CDialogBorderStyle (rep& r)
    :interface(&r)
  { };
  virtual ~CDialogBorderStyle (void)
  { };

  // Setting a new drawer
  void operator=(rep& r)
  {
    interface = &r;
  };

  // Interface to internal drawing code
  void Draw(CDialog& aTarget, CBorder& aBorder)
  {
    interface->Draw(aTarget,aBorder);
  };
  void PositionTitle(CDialog& aTarget, CTitlebar& aTitle)
  {
    interface->PositionTitle(aTarget,aTitle);
  };
  void PositionClient(CDialog& aTarget, CClient& aClient)
  {
    interface->PositionClient(aTarget,aClient);
  };
  void PositionClose(CDialog& aTarget, CCloseButton& aClose)
  {
    interface->PositionClose(aTarget,aClose);
  };

  // The default border drawing style
  static CDefault DefaultDrawer;

  // The default border drawing style
  static CDialogBorderStyle Default;
};


//---------------------------------------------------------------------------
// Exchangable dialog border drawers
//---------------------------------------------------------------------------

// Flat style dialog border type
class CDialogBorderFlat:public CDialogBorderStyle::rep
{
 private:
   virtual void Draw(CDialog& aTarget, CBorder& aBorder);
     
   virtual void PositionTitle(CDialog& aTarget, CTitlebar& aTitle);
   virtual void PositionClient(CDialog& aTarget, CClient& aClient);
   virtual void PositionClose(CDialog& aTarget, CCloseButton& aClose);
};


// Flat style dialog border type
class CDialogBorderWin95:public CDialogBorderStyle::rep
{
 private:
   virtual void Draw(CDialog& aTarget, CBorder& aBorder);
     
   virtual void PositionTitle(CDialog& aTarget, CTitlebar& aTitle);
   virtual void PositionClient(CDialog& aTarget, CClient& aClient);
   virtual void PositionClose(CDialog& aTarget, CCloseButton& aClose);
};


// Rather pitiful Windows 3.1 look-alike dialog border type
class CDialogBorderWin31:public CDialogBorderStyle::rep
{
 private:
   virtual void Draw(CDialog& aTarget, CBorder& aBorder);
     
   virtual void PositionTitle(CDialog& aTarget, CTitlebar& aTitle);
   virtual void PositionClient(CDialog& aTarget, CClient& aClient);
   virtual void PositionClose(CDialog& aTarget, CCloseButton& aClose);
};


// Completely empty border drawing - Minimalistic
class CDialogBorderNone:public CDialogBorderStyle::rep
{
 private:
   virtual void Draw(CDialog& aTarget, CBorder& aBorder);
     
   virtual void PositionTitle(CDialog& aTarget, CTitlebar& aTitle);
   virtual void PositionClient(CDialog& aTarget, CClient& aClient);
   virtual void PositionClose(CDialog& aTarget, CCloseButton& aClose);
};


#endif
