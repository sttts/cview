#include <stdio.h>

// This is a design pattern the I use often.  I'm not sure if it has a name,
// but I call it 'Wrapped ABCs with defaults'.  It allows different units it
// be used like 'plug-ins' and allows default behavior to be changed at
// run-time.  All the objects are very small and there are no pointers to
// leave dangling.  It even allows add/extend the functionality without
// recompiling, and you only need to relink.

// Interface definition in .H file

// This defines the internal interface for shapes
// i.e. things shapes should know about
class ShapeInterface
{
  private:
    virtual void Draw(int x, int y) = 0;  // draw the shape at (x,y)

  friend class ShapeDrawer;
};


// This is one kind of shape
class Square
  :public ShapeInterface
{
  private:
    virtual void Draw(int x, int y)
    {
      printf("Square at %i,%i\n",x,y);
    };
};


// This is another kind of shape
class Circle
  :public ShapeInterface
{
  private:
    virtual void Draw(int x, int y)
    {
      printf("Circle at %i,%i\n",x,y);
    };
};


// This is a tool to draw shapes
// i.e. public interface has things that the user has to know about
class ShapeDrawer
{
  private:
    ShapeInterface* interface;

  public:
    // Initial default drawer draws a square (small, contains no data!)
    static Square DefaultShape;

    // Default global shape drawer (small, contains 1 pointer!)
    static ShapeDrawer Default;

    // Constructors
    ShapeDrawer()
      :interface(Default.interface)
      { };
    ShapeDrawer(const ShapeDrawer& aDrawer)
      :interface(aDrawer.interface)
      { };
    ShapeDrawer(ShapeInterface& aInterface)
      :interface(&aInterface)
      { };

    // The public interface to draw things!! - non-virtual!!
    void Draw(int x, int y)
    {
      interface->Draw(x,y);
    };
    void operator=(const ShapeDrawer& aDrawer)
    {
      interface = aDrawer.interface;
    };
};


// Global objects

// Initial default drawer draws a square (small, contains no data!)
Square ShapeDrawer::DefaultShape;

// Default global shape drawer (small, contains 1 pointer!)
// This must NOT be default constructed!!!
ShapeDrawer ShapeDrawer::Default(ShapeDrawer::DefaultShape);


// Now you can make ojects containing a ShapeDrawer.  It's small (only 1
// pointer) so you can pass it by value and copy without any problems.
// The copy constructor will work, and even = will work!

int main (void)
{
  printf("\n\n");

  // Make a default drawer
  ShapeDrawer Drawer;

  // Draw the default shape
  Drawer.Draw(0,0);           // Will draw a square

  // Make a square drawer
  Square ExampleSquare;
  ShapeDrawer SquareDrawer(ExampleSquare);

  // Draw the square
  SquareDrawer.Draw(0,0);     // Will draw a square

  // Make the default drawer draw circles instead of squares
  Circle ExampleCircle;
  ShapeDrawer::Default = ExampleCircle;

  // Now the default drawer will draw circles and so will all
  // ShapeDrawers default constructed after this point
  ShapeDrawer Drawer2;

  // Draw the default shape
  Drawer2.Draw(0,0);           // Will draw a circle

  // Now force the SquareDrawer to draw a circle
  SquareDrawer = ExampleCircle;

  SquareDrawer.Draw(0,0);      // Will draw a circle

  return 0;
}

/*
   Normally I make the ShapeInterface class and the DefaultShape to be
   nested inside ShapeDrawer to reduce namespace pollution.  I left them
   seperate here to make it easier to understand the design pattern.
   
   What I find appealing is that the shape objects have no internal data and
   the drawer objects have only a pointer inside which can never be NULL.
   All the objects here can be copied by value with no performance penalty.
   This means the user does not need to use pointers anywhere.  This reduces 
   the risk of dangling pointers significantly.

   The ONLY thing the user can do with a ShapeInterface is derive from it
   because it has only private virtual function.  Since the interface
   functions are pure virtual the user is forced to create correct objects.

   The ONLY thing that you can do with shapes is construct them, and pass
   them to drawers.  There is no internal data so the objects are fast and
   easy to copy.

   The ShapeDrawer has the complete user interface of all non-virtual
   functions.  The public interface _can_ be different than the internal
   interface.  i.e. the user doesn't need to know everything to draw a shape.
   
   If the code is compiled into a library and someone makes a new shape, then
   the shape can be added _without_ recompiling the whole thing.  Only the new
   code has to be compiled and then linked into the library.  Users can extend
   the functionality of the library without having to recompile the library.

*/