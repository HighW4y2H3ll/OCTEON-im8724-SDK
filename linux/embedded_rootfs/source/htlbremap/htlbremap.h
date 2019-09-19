#ifndef HTLBREMAP_H
#define HTLBREMAP_H

#include <sys/types.h>
#include <errno.h>
#include <link.h>
#include <string.h>

/* Note ALIGNMENT evaluated multiple times.  */
#define ALIGN(addr, alignment) \
  (((addr) + (alignment) - 1) & -((typeof (addr)) (alignment)))
#define ALIGN_DOWN(addr, alignment) \
  ((addr) & -((typeof (addr)) (alignment)))

#ifdef NDEBUG
# define DEBUG 0
#else
extern int DEBUG;
#endif

#define UNUSED __attribute__ ((unused))

#define dperror(S) \
  if (DEBUG) dprintf ("%s: %s (%d)\n", (S), strerror (errno), errno)

#define dprintf(...)				\
  do {						\
    if (DEBUG)					\
      {						\
	local_printf ("htlbremap: ");		\
	local_printf (__VA_ARGS__);		\
      }						\
  } while (0)

#define I "  "

typedef unsigned long addr_t;

extern size_t pagesize;
extern size_t hugepagesize;

extern int remap_exe, remap_shlibs;
extern int signal_on_error;

struct vma {
  addr_t start;
  addr_t end;
  int flags;
#define VMA_F_PRIVATE	(1 << 0)
#define VMA_F_READ	(1 << 1)
  struct vma *next;
  char name[20];
};

void remap_regions (struct vma *);
void remap_stack (struct vma *);
void register_morecore (void);
void switch_stack_and_call (void *, void (*) (void *), void *);
long local_syscall(int, ...);
addr_t get_mapping (struct vma *, const char *, addr_t *);
void print_error (char *, ...);

void * alloc_save_area (int, size_t);
void free_save_area (void *, size_t);
int new_unlinked_fd (const char *);

ssize_t local_write(int, const void *, size_t);
void local_printf (const char *, ...) __attribute__ ((format (printf, 1, 2)));

#endif /* HTLBREMAP_H */
