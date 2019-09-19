#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#define PAGESIZE (2 * 1024 * 1024)
#define MAXMEM (60 * PAGESIZE)

volatile char *p;
int exhausted;
int errors;
int try_to_exhaust;

static void
handler (int n)
{
  exhausted = 1;
}

void
alloc_test (unsigned block)
{
  unsigned n = MAXMEM/block * 1.25;
  unsigned i;
  volatile char *r = 0;

  for (i = 0; i < n && (p = malloc (block)) && !exhausted; i++)
    {
      p[0] = 1;
      p[block - 1] = 2;
      //      fprintf (stderr,  "  %2d: %p\n", i, p);
      if (i % 4 == 0)
	free ((char *) r);
      r = p;
    }
  exhausted = 0;
  if (i < n * 0.60 || (try_to_exhaust && i == n))
    errors++;
  fprintf (stderr, "Allocation of %d bytes in %d-byte blocks (errors: %d)\n",
	   block * i, block, errors);
}

int
main (int argc, char **argv, char **envp)
{
  unsigned block = PAGESIZE;

  while (envp && *envp)
    {
      printf ("%p main %p %s\n", envp, *envp, *envp);
      envp++;
    }
  try_to_exhaust = getenv ("HTLBREMAP_SIGNAL_ON_ERROR") != 0;
/*   char s[64]; */
/*   sprintf (s, "cat /proc/%d/maps", getpid ()); */
/*   errors |= system (s); */
  if (try_to_exhaust)
    {
      signal (SIGUSR1, handler);
      puts ("signal handler ready");
      if (argc != 2)
	{
	  puts ("I take block as an arg");
	  return 1;
	}
      block = strtod (argv[1], 0);
    }

  alloc_test (block);

  puts (errors ? "FAILED" : "PASSED");
  return errors;
}
