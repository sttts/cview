//----------------------------------------------------------------------------
// 	Memory fortification header
//----------------------------------------------------------------------------
#ifndef ZFORTIFY_H
#define ZFORTIFY_H
//----------------------------------------------------------------------------
#include <stdlib.h>
#include <new.h>
//----------------------------------------------------------------------------
// Our wonderful new operators
void *operator new(size_t size);
void *operator new(size_t size, const char *file, unsigned long line);
void  operator delete(void *pointer);

// Some compilers use a different new operator for newing arrays.
// This includes GNU G++ (2.6.0) and Borland C++ (4.02)
#ifdef __GNUC__
void *operator new[](size_t size);
void *operator new[](size_t size, char *file, unsigned long line);
#endif

// Prototypes - for the compiler's benefit only. The user should never
// need to call these functions with these arguments. Use the macros
// defined a little further down in the file.

int ZFortify_EnterScope(char *file, unsigned long line);
int ZFortify_LeaveScope(char *file, unsigned long line);
int ZFortify_OutputAllMemory(char *file, unsigned long line);
int ZFortify_DumpAllMemory(int scope, char *file, unsigned long line);
int ZFortify_CheckAllMemory(const char *file, unsigned long line);
int ZFortify_CheckPointer(void *uptr, char *file, unsigned long line);
int ZFortify_Disable(char *file, unsigned long line);
int ZFortify_PreDelete(char *file, unsigned long line);

typedef void (*ZFortify_OutputFuncPtr)(const char *);
ZFortify_OutputFuncPtr ZFortify_SetOutputFunc(ZFortify_OutputFuncPtr Output);
int ZFortify_SetNewFailRate(int Percent);

#ifndef ZFORTIFY_CPP /* Only define the macros if we're NOT in ZFortify.cpp */

// Add  soucecode information to the new and delete calls.  Note that these
// macros  will  create  syntax  errors  when a piece of code is defining a
// custom  new or delete operator.  If this happens, you will need to place
// #undef's and #define's around the offending code (sorry).
//
// eg.
// #undef new
// void *X::operator new(size_t) { return malloc(size_t); }
// #define new ZFortify_New
//
#define ZFortify_New                    new(__FILE__, __LINE__)
#define ZFortify_Delete                 ZFortify_PreDelete(__FILE__, __LINE__), delete
#define new                             ZFortify_New
#define delete                          ZFortify_Delete

#define ZFortify_EnterScope()           ZFortify_EnterScope(__FILE__, __LINE__)
#define ZFortify_LeaveScope()           ZFortify_LeaveScope(__FILE__, __LINE__)
#define ZFortify_OutputAllMemory()      ZFortify_OutputAllMemory(__FILE__, __LINE__)
#define ZFortify_DumpAllMemory(s)       ZFortify_DumpAllMemory(s, __FILE__, __LINE__)
#define ZFortify_CheckAllMemory()       ZFortify_CheckAllMemory(__FILE__, __LINE__)
#define ZFortify_CheckPointer(ptr)      ZFortify_CheckPointer(ptr, __FILE__, __LINE__)
#define ZFortify_Disable()              ZFortify_Disable(__FILE__, __LINE__)

#endif // ZFORTIFY_CPP

#endif // ZFORTIFY_H
