#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "test.h"

void fail (char *alloc, size_t size)
{
  fprintf (stderr,
	   "FAILED allocating %d bytes with %s\n", size, alloc);
  exit (1);
}

void *heap (size_t size)
{
  void *p = malloc (size);
  if (!p)
    fail ("heap", size);
  printf ("heap: %p\n", p);
  return p;
}

void *map (size_t size)
{
  void *p;
  p = mmap (0, size, PROT_READ|PROT_WRITE|PROT_EXEC,
	    MAP_SHARED|MAP_ANONYMOUS, 0, 0);
  if (p == MAP_FAILED)
    fail ("mmap", size);
  printf ("mmap: %p\n", p);
  return p;
}

const size_t M = 1024*1024;

main ()
{
  char *p;
  char s[64];

  heap (4);
  p = map (4*M);
  heap (2*M);
  munmap (p, 4*M);
  printf ("unmmaped: %p\n", p);
  p = heap (2*M);
  p[5] = 1;
  printmap ();
  return 0;
}
