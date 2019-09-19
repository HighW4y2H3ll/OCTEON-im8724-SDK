#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#define PAGESIZE (2 * 1024 * 1024)
#define MAXMEM (60 * PAGESIZE)

volatile char *p;
int errors;
volatile char *hist[4];

void
alloc_test (unsigned block)
{
  unsigned i;

  for (i = 0; i < MAXMEM/block*10; i++)
    {
      p = (volatile char *) malloc (block);
      p[0] = 1;
      p[block - 1] = 2;
      hist[i % 4] = p;
      free ((char *) hist[(i + 1) % 4]);
    }
}

int
main (int argc, char **argv)
{
  unsigned block;

  if (argc != 2)
    {
      puts ("I take block as an arg");
      return 1;
    }
  block = strtod (argv[1], 0);

  alloc_test (block);

  puts (errors ? "FAILED" : "PASSED");
  return errors;
}
