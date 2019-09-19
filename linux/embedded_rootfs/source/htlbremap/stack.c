#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>

#include "htlbremap.h"

/* This can be overridden in ulimit.  */
const addr_t default_huge_stack_size = 8*1024*1024;

static addr_t
compute_huge_stack_bottom (addr_t stack_bottom UNUSED,
			   addr_t huge_stack_top UNUSED)
{
  addr_t huge_stack_bottom;
  struct rlimit rlim;
  getrlimit (RLIMIT_STACK, &rlim);

  if (rlim.rlim_cur != RLIM_INFINITY)
    return huge_stack_top - ALIGN (rlim.rlim_cur, hugepagesize);

  dprintf (I "Stack limit infinite, using %ld bytes\n",
	   ALIGN (default_huge_stack_size, hugepagesize));
  huge_stack_bottom
    = huge_stack_top - ALIGN (default_huge_stack_size, hugepagesize);
  /* Touch this so that if you exceed the huge stack the kernel
     starts allocating regular stack.  */
  ((char *)huge_stack_bottom)[-1] = 1;
  return huge_stack_bottom;
}

static void
remap_stack_1 (void *arg)
{
  addr_t stack_bottom, stack_top, stack_size;
  addr_t huge_stack_bottom, huge_stack_top, huge_stack_size;
  void *save_area;
  long offset;
  int fd;
  int r;
  register addr_t spreg asm ("sp");
  void *huge_stack;
  struct vma *areas = arg;

  dprintf (I "Using new stack 0x%lx\n", spreg);

  stack_bottom = get_mapping (areas, "[stack]", &stack_top);
  if (!stack_bottom)
    {
      print_error ("Unable to determine stack mapping\n");
      return;
    }
  stack_size = stack_top - stack_bottom;

  dprintf (I "Original stack 0x%lx-0x%lx\n", stack_bottom, stack_top);

  huge_stack_top = ALIGN (stack_top, hugepagesize);
  huge_stack_bottom = compute_huge_stack_bottom (stack_bottom, huge_stack_top);
  huge_stack_size = huge_stack_top - huge_stack_bottom;

  fd = new_unlinked_fd ("stack-save-");
  save_area = alloc_save_area (fd, huge_stack_size);
  if (!save_area)
    {
      print_error ("htlbremap: failed to allocate %lu bytes for stack save area\n",
		   huge_stack_size);
      return;
    }

  offset = ((long) stack_bottom) - (long) huge_stack_bottom;

  memcpy ((void *)(((long) save_area) + offset), (void *) stack_bottom, stack_size);

  r = msync(save_area, huge_stack_size, MS_SYNC);
  if (r < 0)
    {
      print_error ("htlbremap: Can't msync stack save area (%zd bytes)\n",
		   huge_stack_size);
      free_save_area (save_area, huge_stack_size);
      close (fd);
      return;
    }

  dprintf (I "Unmapping save area.\n");
  free_save_area (save_area, huge_stack_size);

  dprintf (I "mmaping new stack %lx-%lx\n",
	   huge_stack_bottom, huge_stack_top);
  huge_stack = mmap ((void *)huge_stack_bottom, huge_stack_size,
		     PROT_READ | PROT_WRITE | PROT_EXEC,
		     MAP_PRIVATE | MAP_FIXED, fd, 0);
  if (huge_stack == MAP_FAILED)
    {
      perror ("mmap");
      print_error ("Failed to mmap new stack\n");
    }
  close (fd);
}

void
remap_stack (struct vma *areas)
{
  char *temp_stack, *temp_stack_top;
  size_t temp_stack_size = 40 * pagesize;
  int fd;

  dprintf ("Processing stack\n");
  fd = new_unlinked_fd ("temp-stack-");
  temp_stack = alloc_save_area (fd, temp_stack_size);
  temp_stack_top = temp_stack + temp_stack_size;
  if (!temp_stack)
    {
      print_error ("Failed to allocate temporary stack\n");
      return;
    }

  dprintf (I "Temporary stack at 0x%lx-0x%lx\n", (addr_t) temp_stack,
	   (addr_t) temp_stack_top);
  switch_stack_and_call (temp_stack_top, remap_stack_1, areas);

  dprintf (I "Unmapping temp stack.\n");

  free_save_area (temp_stack, temp_stack_size);
  close (fd);
  dprintf (I "Completed.\n");
}

