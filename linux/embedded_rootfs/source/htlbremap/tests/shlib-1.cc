#include <iostream>

#include "test.h"

void
f()
{
  throw 1;
}

int
main ()
{
  try {
    f();
  }
  catch (...) {
    printmap();
    return 0;
  }
  return 1;
}
