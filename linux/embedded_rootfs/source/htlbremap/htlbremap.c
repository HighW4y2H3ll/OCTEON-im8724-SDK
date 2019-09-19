#include <alloca.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

#include "htlbremap.h"

#define HTLB_FS_DIR "/mnth"

#ifndef NDEBUG
int DEBUG;
#endif
size_t pagesize;
size_t hugepagesize;
int signal_on_error;
int remap_exe;
int remap_shlibs;

ssize_t
local_write(int fd, const void *buf, size_t count)
{
  return local_syscall (SYS_write, fd, buf, count);
}

void
local_printf (const char *fmt, ...)
{
  va_list ap;
  int seenl = 0;

  va_start (ap, fmt);

  for (; *fmt; fmt++)
    if (*fmt == '%')
      {
	int zerofill = 0;
	int fillwidth = 0;
      format:
	fmt++;
	if (*fmt == '0')
	  zerofill = 1, fmt++;
	while ('0' <= *fmt && *fmt <= '9')
	  {
	    fillwidth *= 10;
	    fillwidth += *fmt - '0';
	    fmt++;
	  }

	switch (*fmt)
	  {      
	  case '%':
	    local_write (1, "%", 1);
	    break;
	  case 's':
	    local_printf (va_arg (ap, const char *));
	    break;

	  case 'p':
	    local_printf ("0x");
	    zerofill = 1;
	    fillwidth -= 2;
	    seenl =  sizeof (void *) == sizeof (long);
	    goto x;
	  case 'l':
	    seenl = 1;
	    goto format;
	    break;
	  case 'x':
	  x:
	    {
	      long d, i, j, k;
	      if (seenl)
		i = va_arg (ap, long);
	      else
		i = va_arg (ap, int);
	      seenl = 0;

	      for (k = 1, d = 1, j = i / 16; j > 0; j /= 16)
		{
		  d++;
		  k *= 16;
		}
	      for (; fillwidth > d; fillwidth--)
		local_write (1, zerofill ? "0" : " ", 1);
	      for (; k > 0; k /= 16)
		{
		  unsigned digit = i / k;
		  char c = digit > 9 ? 'a' + digit - 10 : '0' + digit;
		  local_write (1, &c, 1);
		  i %= k;
		}
	      break;
	    }
	  case 'd':
	    {
	      long i, d, j, k;

	      if (seenl)
		i = va_arg (ap, long);
	      else
		i = va_arg (ap, int);
	      seenl = 0;

	      if (i < 0)
		{
		  local_write (1, "-", 1);
		  i = -i;
		}
	      for (k = 1, d = 1, j = i / 10; j > 0; j /= 10)
		{
		  d++;
		  k *= 10;
		}
	      for (; fillwidth > d; fillwidth--)
		local_write (1, zerofill ? "0" : " ", 1);
	      for (; k > 0; k /= 10)
		{
		  unsigned digit = i / k;
		  char c = '0' + digit;
		  local_write (1, &c, 1);
		  i %= k;
		}
	      break;
	    }
	  default:
	    local_write (1, "[%", 2);
	    local_write (1, fmt, 1);
	    local_write (1, " not supported]", 16);
	    (void) va_arg (ap, int);
	  }
      }
    else if (*fmt == '\\')
      {
	switch (*++fmt)
	  {
	  case 'n':
	    local_write (1, "\n", 1);
	    break;
	  case 't':
	    local_write (1, "\t", 1);
	    break;
	  case '\\':
	    local_write (1, "\\", 1);
	    break;
	  }
      }
    else
      local_write (1, fmt, 1);

  va_end (ap);
}

void
print_error (char *fmt, ...)
{
  va_list ap;
  va_start (ap, fmt);
  vfprintf (stderr, fmt, ap);
  va_end (ap);
  if (signal_on_error)
    abort ();
}

int
new_unlinked_fd (const char * prefix)
{
  int fd;
  char name[64];
  static int i;

  sprintf (name, HTLB_FS_DIR "/%s%d-%d", prefix, getpid(), i++);
  dprintf (I I "temp: %s\n", name);
  fd = open (name, O_CREAT | O_RDWR, 0755);
  if (fd < 0)
    {
      dperror (I I "open");
      exit (1);
    }
  unlink (name);

  return fd;
}

/* We *have* to allocate hugetlb pages here.  If we don't we can be
   given pages that will be unmapped when we fixed-map the new huge
   pages.  */

void *
alloc_save_area (int fd, size_t nb)
{
  void *p;
  size_t aligned_nb = ALIGN (nb, hugepagesize);

  dprintf (I "Allocating HTLB save area %d, aligned %d.\n",
	   (int) nb, (int) aligned_nb);

  p = mmap (0, aligned_nb, PROT_READ | PROT_WRITE | PROT_EXEC,
	    MAP_SHARED, fd, 0);
  if (p == MAP_FAILED)
    {
      dperror (I I "mmap");
      return NULL;
    }
  dprintf (I I "save area: %p\n", p);
  return p;
}

void
free_save_area (void *p, size_t nb)
{
  size_t aligned_nb = ALIGN (nb, hugepagesize);
  if (munmap (p, aligned_nb))
    {
      dperror (I I "munmap");
    }
}

/* Look for LOOKFOR in file specified by FILENAME.  Return the
   matching line in LINE with max length LEN.  The return value is the
   index in LINE one character after the match.  If LOOKFOR was not
   found 0 is returned.  */

static unsigned
findline (const char *filename, const char *lookfor, char *line, int len)
{
  FILE *inp;
  char *found;

  inp = fopen (filename, "r");
  if (!inp)
    {
      char buf[256];
      sprintf (buf, "htlbremap: Open %s", filename);
      perror (buf);
      exit (1);
    }

  found = NULL;
  while (!feof (inp))
    {

      fgets (line, len, inp);
      line[len - 1] = 0;

      found = strstr (line, lookfor);
      if (found)
	break;
    }

  fclose (inp);

  return found ? found - line + strlen (lookfor) : 0;
}

addr_t
get_mapping (struct vma *areas, const char *name, addr_t *top)
{
  while (areas)
    {
      if (strstr (areas->name, name))
	{
	  *top = areas->end;
	  return areas->start;
	}
      areas = areas->next;
    }
  return 0;
}

static int
gethugepagesize (void)
{
  char line[256];
  unsigned end;
  int hugesize;

  /* Check that there are hugepages.  */
  end = findline ("/proc/meminfo", "HugePages_Free:", line, sizeof (line));
  if (!end || atoi (&line[end]) <= 0)
    {
      fprintf
	(stderr,
	 "htlbremap: no free huge pages according to /proc/meminfo\n");
      exit (1);
    }

  /* Check HTLB_FS_DIR.  */
  end = findline ("/proc/mounts", "none " HTLB_FS_DIR, line, sizeof (line));
  if (!end || strncmp (&line[end], " hugetlbfs ", 11) != 0)
    {
      fprintf
	(stderr,
	 "htlbremap: " HTLB_FS_DIR " is not mounted with type hugetlbfs\n");
      exit (1);
    }

  end = findline ("/proc/meminfo", "Hugepagesize:", line, sizeof (line));
  hugesize = atoi (&line[end]);
  if (!end || hugesize <= 0)
    {
      fprintf
	(stderr,
	 "htlbremap: failed to get the huge page size from /proc/meminfo\n");
      exit (1);
    }

  return hugesize << 10;
}

static int err_no;

int
local_syscall_error (int e)
{
  err_no = e;
  return -1;
}

static int
enabled (char *feature)
{
  char *env = getenv ("HTLBREMAP_DISABLE");
  return !env || !strstr (env, feature);
}
static __attribute__ ((constructor)) void
ctor (void)
{
  FILE *maps;
  char buf[256];
  struct vma *current_mappings = NULL;
  struct vma *current_mappings_end = NULL;
  struct vma *t = NULL;
#ifndef DEBUG
  char *env;
  env = getenv ("HTLBREMAP_DEBUG");
  if (env && strtol (env, 0, 0))
    DEBUG = 1;
#endif
  pagesize = getpagesize ();
  hugepagesize = gethugepagesize ();
  dprintf ("Normal pages are %dKB\n", (int) pagesize >> 10);
  dprintf ("Huge pages are %dMB\n", (int) hugepagesize >> 20);
  signal_on_error = getenv ("HTLBREMAP_SIGNAL_ON_ERROR") != 0;

  if (enabled ("heap"))
    register_morecore ();
  remap_exe = enabled ("executable");
  remap_shlibs = enabled ("shlibs");

  maps = fopen ("/proc/self/maps", "r");
  if (!maps)
    {
      fprintf (stderr, "htlbremap: failed to open /proc/self/maps\n");
      exit (1);
    }

  while (fgets (buf, sizeof(buf), maps))
    {
      char *space_state;
      char *dash_state;
      char *space_token;
      char *dash_token;
      int i;

      t = alloca(sizeof(struct vma));
      t->flags = 0;

      space_token = strtok_r (buf, " ", &space_state);
      if (space_token == NULL)
	{
	  fprintf (stderr, "htlbremap: couldn't parse /proc/self/maps <%s>\n", buf);
	  exit (1);
	}

      dash_token = strtok_r (space_token, "-", &dash_state);
      if (dash_token == NULL)
	{
	  fprintf (stderr, "htlbremap: couldn't parse /proc/self/maps <%s>\n", buf);
	  exit (1);
	}
      t->start = strtoul (dash_token, NULL, 16);

      dash_token = strtok_r (NULL, "-", &dash_state);
      if (dash_token == NULL)
	{
	  fprintf (stderr, "htlbremap: couldn't parse /proc/self/maps <%s>\n", buf);
	  exit (1);
	}
      t->end = strtoul (dash_token, NULL, 16);

      space_token = strtok_r (NULL, " ", &space_state);
      if (space_token == NULL)
	{
	  fprintf (stderr, "htlbremap: couldn't parse /proc/self/maps <%s>\n", buf);
	  exit (1);
	}
      if (space_token[0] == 'r')
	t->flags |= VMA_F_READ;
      if (space_token[3] == 'p')
	t->flags |= VMA_F_PRIVATE;

      for (i = 0; i < 4; i++)
	{
	  space_token = strtok_r (NULL, " ", &space_state);
	  if (space_token == NULL)
	    {
	      fprintf (stderr, "htlbremap: couldn't parse /proc/self/maps <%s>\n", buf);
	      exit (1);
	    }
	}
      strncpy (t->name, space_token, sizeof(t->name));
      t->next = NULL;
      if (current_mappings_end)
	  current_mappings_end->next = t;
      else
	current_mappings = t;
      current_mappings_end = t;
    }

  if (remap_exe || remap_shlibs)
    remap_regions (current_mappings);
  if (enabled ("stack"))
    remap_stack (current_mappings);

  fclose (maps);
}
