class singelton
{
 private:
   singelton() { };
   ~singelton() { };

 friend class wrapper;
};


class wrapper
{
 public:
   singelton nonstatic;
   static singelton isstatic;
};

singelton wrapper::isstatic;




