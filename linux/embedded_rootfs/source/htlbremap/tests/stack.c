#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

int
main ()
{
  register unsigned long sp asm ("sp");
  FILE * f = fopen ("/proc/self/maps", "r");
  char *line = 0;
  size_t len;

  printf ("sp: %lx\n", sp);
  while (getline (&line, &len, f) != -1)
    {
      unsigned long bottom, top;
      char *p;

      printf (line);
      bottom = strtoul (line, &p, 16);
      if (bottom && bottom <= sp && sp < strtoul (p + 1, NULL, 16))
	{
	  puts ("Found:");
	  puts (line);
	  if (strstr (line, "/mnth/stack"))
	    {
	      puts ("PASSSED");
	      return 0;
	    }
	  else
	    {
	      puts ("FAILED");
	      return 1;
	    }
	}
    }
  puts ("FAILED: can't find stack mapping");
  return 1;
}
