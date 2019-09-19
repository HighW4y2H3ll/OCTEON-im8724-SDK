/* Test if overlapping mmap and malloc regions work.  */

#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

int ps;
int errors;

void *heap (void)
{
  return malloc (ps);
}

void *map (void)
{
  void *p;
  p = mmap (0, ps, PROT_READ|PROT_WRITE|PROT_EXEC,
	    MAP_SHARED|MAP_ANONYMOUS, 0, 0);
  return p == MAP_FAILED ? 0 : p;
}

void
recurse (char *hp, char *mp, int ncrossing, char *start)
{
  char *b;
  uint64_t *p;
  int i;

  if (hp < mp)
    {
      b = heap ();
      if (b + ps > mp)
	{
	  fprintf (stderr,
		   "heap region: %p -> %p: %d pages (next at %p)\n",
		   start, hp + ps, (hp + ps - start + ps - 1) / ps, b);
	  start = mp;
	  ncrossing--;
	}
      hp = b;
    }
  else
    {
      b = map ();
      if (b + ps > hp)
	{
	  fprintf (stderr,
		   "mmap region: %p -> %p: %d pages (next at %p)\n",
		   start, mp + ps, (mp + ps - start + ps - 1) / ps, b);
	  start = hp;
	  ncrossing--;
	}
      mp = b;
    }

  if (!b)
    {
      fprintf (stderr,
	       "Run out of memory; was supposed to cross %d-more times\n",
	       ncrossing);
      exit (1);
    }

  for (p = (uint64_t *) b; (char *) p - b < ps; p++)
    *p = (uint64_t) (unsigned long) b;

  if (ncrossing)
    recurse (hp, mp, ncrossing, start);

  for (p = (uint64_t *) b; (char *) p - b < ps; p++)
    if (*p != (uint64_t) (unsigned long) b)
      {
	fprintf
	  (stderr,
	   "corruption in %s-allocated %p (expected %llx found %llx)\n",
	   b == hp ? "heap" : "mmap", p,
	   (unsigned long long) (unsigned long) b, *p);
	errors++;
	break;
      }
  if (errors > 55)
    exit (errors);

}

int
main ()
{
  void *h, *m;
  char buf[100];
  ps = getpagesize ();
  h = heap ();
  m = map ();
  fprintf (stderr, "starting from heap: %p, mmap: %p, pagesize: %d\n",
	   h, m, ps);
  recurse (h, m, 5, h < m  ? h : m);
/*   sprintf (buf, "cat /proc/%d/maps", getpid ()); */
/*   errors |= system (buf); */
  puts (errors ? "FAILED" : "PASSED");
  return errors;
}
