#include <malloc.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

int user_cnt_locked1;

void *map (unsigned n)
{
  void *p;
  p = mmap (0, n, PROT_READ|PROT_WRITE|PROT_EXEC,
	    MAP_SHARED|MAP_ANONYMOUS, 0, 0);
  return p == MAP_FAILED ? 0 : p;
}

void *mymalloc (unsigned n)
{
  void *p = malloc (n);
  if (!p)
    {
      puts ("FAILED");
      exit (1);
    }
  return p;
}

main ()
{
  char *p, *p1, *p2, *p3;
  do {
    printf ("allocing %u\n", 3871488);
    p = mymalloc (3871488);
    printf ("done: %p\n", p);
  } while ((unsigned long) p < 0x2af22000);
  p3 = map (0x100000);
  printf ("mmaped: %p\n", p3);
  printf ("allocing %u\n", 3871488);
  p = mymalloc (3871488);
  printf ("done: %p\n", p);
#if 1
  p3 = map (0x100000);
  printf ("mmaped: %p\n", p3);
  printf ("allocing %u\n", 3871488);
  p = mymalloc (3871488);
  printf ("done: %p\n", p);
#endif
  printf ("allocing %u\n", 135168);
  p1 = mymalloc (135168);
  printf ("done: %p\n", p1);
  puts ("freeing 3871488");
  free (p);
  puts ("done");
  printf ("allocing %u\n", 3871488);
  p2 = mymalloc (3871488);
  printf ("done: %p\n", p2);

  puts ("PASSED");
  return 0;
}
