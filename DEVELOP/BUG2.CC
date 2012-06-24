/* BUG: Constant enum's arent always handled correctly */

#include <stdio.h>

enum index { zero = 0,
             one,
             two,
             three,
             four
           };

index GetVariable()
{
  return zero;
}

/* This assignment seems to be being skipped */
const index problem = GetVariable();

void Test(index event)
{
  switch(event)
  {
    /* Error occurs here */
    case problem:
      break;
    default:
  }

}

/*
I get these errors:

bug2.cc: In function `void Test(enum index)':
bug2.cc:25: case label `problem' does not reduce to an integer constant
bug2.cc:27: warning: unreachable code at beginning of switch statement
bug2.cc:28: parse error before `}'

I'm declaring a enum that's const so that should be a number, and not
just treated like a define, as it appears to be doing here, otherwise
side effects aren't handled properly.

*/

