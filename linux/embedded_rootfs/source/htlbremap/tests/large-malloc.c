#include <stdio.h>
#include <stdlib.h>

const unsigned n = 67346432 / sizeof (int);

int
main ()
{
  volatile int *p = malloc (n * sizeof (int));
  fprintf (stderr, "%p - %p\n", p, &p[n - 1]);
  p[0] = 0x12345678;
  p[n - 1] = 0x87654321;
  puts ("PASSED");
  return 0;
}
