#include <stddef.h>
#include <sys/mman.h>
#include <stdio.h>
#include <signal.h>
#include <malloc.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "htlbremap.h"

static int heap_fd;
static char *heap_base;
static char *heap_next;
static void *(*default_morecore) ();
static ptrdiff_t heap_left;

/* libc's malloc supports non-contiguous morecore however for better
   performance we try to keep things contiguous as much as we can.  */

static void *
htlb_morecore (ptrdiff_t n)
{
  char *p;

  dprintf ("morecore: called requesting %ld bytes\n", (long)n);
  /* We can only trim from the last contiguous block.  */
  if (n < heap_base - heap_next)
    n = heap_base - heap_next;
  else if (n > heap_left)
    {
      size_t size;
      void *new;

      size = ALIGN (n, hugepagesize);
      new = mmap (0, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE,
		  heap_fd, 0);
      if (new == MAP_FAILED)
	{
	  static int out_of_heap_reported;
	  if (!out_of_heap_reported)
	    {
	      fprintf
		(stderr,
		 "htlbremap: morecore: Can't allocate %zd bytes of heap\n",
		 size);
	      out_of_heap_reported = 1;
	    }
	  /* Send a notification to the tests when we switch from
	     hugetlb-based heap to regular mmap-base.  */
	  if (signal_on_error)
	    kill (getpid (), SIGUSR1);
	  return 0;
	}
      dprintf ("morecore:   allocated %ld bytes at %p\n", (long) size, new);

      /* If contiguous merge it with the previous one.  */
      if (heap_next + heap_left == new)
	  heap_left += size;
      else
	{
	  if (new < (void *) heap_base)
	    fprintf (stderr,
		     "htlbremap: morecore: allocated below current base\n");
	  heap_next = heap_base = new;
	  heap_left = size;
	}
    }

  if (n >= 0)
    p = heap_next;
  heap_left -= n;
  heap_next += n;
  if (n < 0)
    p = heap_next;

  if (heap_left >= (ptrdiff_t) hugepagesize)
    {
      size_t size = ALIGN_DOWN (heap_left, hugepagesize);
      if (munmap (heap_next + heap_left - size, size) == 0)
	{
	  dprintf ("morecore:   deallocated %ld bytes\n", (long) size);
	  heap_left -= size;
	}
      else
	perror ("htlbremap: morecore: unmapping htlb pages");
    }

  if (n > 0)
    memset (p, 0, n);

  dprintf ("morecore: returning: %p, %d bytes (base %p, top %p, left %d)\n",
	   p, (int) n, heap_base, heap_next, (int) heap_left);
  return p;
}

void
register_morecore (void)
{
  heap_fd = new_unlinked_fd ("heap-");
  default_morecore = __morecore;
  __morecore = htlb_morecore;
  dprintf ("Installed htlb morecore (base: %p)\n", heap_base);

  /* Basically we should not do any mmap'ing outside of our hugetlb
     mapping of the heap.  The problem is that since the heap has to
     be contiguous we need to use MAP_FIXED.  If something was mmap'ed
     underneath and the heap needs to grow because of the semanitics
     of MAP_FIXED we woud implicitly unmap any overlapping region.  We
     can disable malloc's mmaping but not the user program's of
     course.  See the HTLB_HEAP_SIZE environment variable for more on
     this.  */
  mallopt (M_MMAP_MAX, 0);
}

