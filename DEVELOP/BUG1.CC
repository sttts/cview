/* BUG: The calling of destructors for (class) statically declared classes
   is done in global scope and not class scope like it should. */

class singelton
{
 private:
   singelton() { };
   ~singelton() { };

 public:
   static singelton instance;
};

singelton singelton::instance;

/*
I get these errors:

bug1.cc:12: warning: all member functions in class `singelton' are private
bug1.cc:12: warning: `class singelton' only defines a private destructor and has no friends
bug1.cc: In function `(static destructors for bug1.cc)':
bug1.cc:8: `singelton::~singelton()' is private
bug1.cc:33: within this context

Small problem:
The warning about all member functions being private and the class having no
friends is not useful when there are static instances of the class.

Bug:
The static singleton is within the scope of class and should be able to be
destructed, but DJGPP is trying to destroy it at global scope.
*/

