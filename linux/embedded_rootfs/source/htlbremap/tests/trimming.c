#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

int errors;

#define PAGESIZE (2 * 1024 * 1024)
#define MAXMEM (30 * PAGESIZE)

static int
new_unlinked_fd (const char * prefix)
{
  int fd;
  char *name;

  name = tempnam ("/mnth", prefix);
  fd = open (name, O_CREAT | O_RDWR, 0755);
  if (fd < 0)
    {
      puts ("Failed creating hugetlbfs file");
      exit (1);
    }

  unlink (name);

  return fd;
}

int
main (void)
{
  volatile char *p;
  unsigned i;

  for (i = 0; i < 10; i++)
    {
      p = malloc (MAXMEM);
      p[0] = 1;
      p[MAXMEM - 1] = 2;
      free ((char *) p);

      p = mmap (0, MAXMEM, PROT_READ | PROT_WRITE, MAP_PRIVATE,
		new_unlinked_fd ("test"), 0);
      if (p == MAP_FAILED)
	{
	  puts ("mmap failed");
	  exit (1);
	}
      p[0] = 1;
      p[MAXMEM - 1] = 2;
      munmap (p, MAXMEM);
    }
  puts ("PASSED");
  return 0;
}
