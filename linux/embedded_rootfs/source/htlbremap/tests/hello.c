#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#include "test.h"

char cmd[64];

int
main ()
{
  register long sp asm ("sp");
  fprintf (stderr,
	   "main pid: %d sp: 0x%lx heap: %p\n", getpid (), sp, malloc (1));
  printmap ();
  return 0;
}
