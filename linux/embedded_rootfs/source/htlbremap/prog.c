#define _GNU_SOURCE
#include <link.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>

#include "htlbremap.h"

#define FOR_EACH_LOAD_PHDR(index, phdr, phdrs, phnum) \
  for (index = 0, phdr = phdrs; index < phnum; index++, phdr++) \
    if (phdr->p_type == PT_LOAD)

/* Just wrap a phdr_info and provide linking of shobjs that will be
   remapped together.  */

struct shobj
{
  addr_t bottom;
  size_t size;
  struct dl_phdr_info *phdr_info;
  struct shobj *next_same_region;
  int nr_in_same_region;
};

static struct shobj *libhtlbremap_so, *libhtlbremap_region;

static const char *
shobj_name (struct dl_phdr_info *info)
{
  return info->dlpi_addr ? info->dlpi_name : "main-executable";
}

struct compute_shobjs_memsize_cb_args
{
  size_t size;
  size_t nr_shobjs;
};

static int
compute_shobjs_memsize_cb (struct dl_phdr_info *info, size_t siz, void *data)
{
  struct compute_shobjs_memsize_cb_args *a = data;
  size_t len;

  len = sizeof (struct shobj);
  len += sizeof (struct dl_phdr_info);
  len += sizeof (ElfW(Phdr)) * info->dlpi_phnum;
  len += strlen (info->dlpi_name) + 1;
  dprintf (I "%s phdr info memsize: %d (%d)\n", shobj_name (info), (int) len,
	   (int) siz);
  a->size += len;
  a->nr_shobjs++;

  return 0;
}

static size_t
compute_shobjs_memsize (size_t *nr_shobjs)
{
  struct compute_shobjs_memsize_cb_args a = { 0, 0 };
  dl_iterate_phdr (compute_shobjs_memsize_cb, &a);

  *nr_shobjs = a.nr_shobjs;
  return a.size;
}

struct create_shobjs_cb_args
{
  char *mem;
  size_t memleft;
  struct shobj **shobjs;
  size_t nr_shobjs;
};

static void *
myalloc (size_t size, char **mem, size_t *memleft)
{
  void *p = *mem;
  *mem += size;
  if (*memleft < size)
    abort ();
  *memleft -= size;
  return p;
}

static addr_t
compute_shobj_region (struct dl_phdr_info *info, size_t *size)
{
  int i;
  const ElfW(Phdr) *phdr;
  ElfW(Addr) highest_vaddr = 0;
  ElfW(Addr) lowest_vaddr = 0;
  int first;

  first = 1;
  FOR_EACH_LOAD_PHDR (i, phdr, info->dlpi_phdr, info->dlpi_phnum)
    {
      if (phdr->p_vaddr > highest_vaddr || first)
	highest_vaddr = phdr->p_vaddr + phdr->p_memsz;
      if (phdr->p_vaddr < lowest_vaddr || first)
	lowest_vaddr = phdr->p_vaddr;
      first = 0;
    }

  *size = highest_vaddr - lowest_vaddr;
  return lowest_vaddr + info->dlpi_addr;
}

static int
in_htlbremap (struct dl_phdr_info *info)
{
  int i;
  const ElfW(Phdr) *phdr;
  void *addr = &in_htlbremap;

  FOR_EACH_LOAD_PHDR (i, phdr, info->dlpi_phdr, info->dlpi_phnum)
    if ((void *) (info->dlpi_addr + phdr->p_vaddr) <= addr
	&& addr < (void *) (info->dlpi_addr + phdr->p_vaddr + phdr->p_memsz))
      return 1;
  return 0;
}

static int
create_shobjs_cb (struct dl_phdr_info *info, size_t siz UNUSED, void *data)
{
  struct create_shobjs_cb_args *a = data;
  
  struct shobj *shobj =
    myalloc (sizeof (struct shobj), &a->mem, &a->memleft);
  a->shobjs[a->nr_shobjs++] = shobj;
  shobj->bottom = compute_shobj_region (info, &shobj->size);

  shobj->phdr_info =
    myalloc (sizeof (struct dl_phdr_info), &a->mem, &a->memleft);
  memcpy (shobj->phdr_info, info, sizeof (struct dl_phdr_info));
  shobj->phdr_info->dlpi_phdr =
    myalloc (info->dlpi_phnum * sizeof (ElfW(Phdr)), &a->mem, &a->memleft);
  memcpy ((void *) shobj->phdr_info->dlpi_phdr, info->dlpi_phdr,
	  info->dlpi_phnum * sizeof (ElfW(Phdr)));
  shobj->phdr_info->dlpi_name =
    myalloc (strlen (info->dlpi_name) + 1, &a->mem, &a->memleft);
  strcpy ((char *) shobj->phdr_info->dlpi_name, info->dlpi_name);

  if (in_htlbremap (info))
    libhtlbremap_so = shobj;

  shobj->next_same_region = NULL;
  shobj->nr_in_same_region = -1;

  return 0;
}

static void
create_shobjs (struct shobj **shobjs, void *mem, size_t memleft)
{
  struct create_shobjs_cb_args args = { mem, memleft, shobjs, 0 };
  dl_iterate_phdr (create_shobjs_cb, &args);
}

static size_t
create_regions (struct shobj *shobjs[], size_t nr_shobjs,
		struct shobj **regions)
{
  size_t i, j, nr_regions;

  for (i = 0, nr_regions = 0; i < nr_shobjs; i = j + 1, nr_regions++)
    {
      regions[nr_regions] = shobjs[i];

      for (j = i;
	   shobjs[j]->next_same_region;
	   j++)
	;
      regions[nr_regions]->nr_in_same_region = j + 1 - i;
    }

  return nr_regions;
}

static void
print_region (struct shobj *region)
{
  struct shobj *shobj;
      
  for (shobj = region; shobj; shobj = shobj->next_same_region)
    {
      size_t k;
      const ElfW(Phdr) *phdr;
      struct dl_phdr_info *info = shobj->phdr_info;

      dprintf (I I "Shared object: %s\n", shobj_name (info));
      dprintf (I I "%lx-%lx %d\n",
	       shobj->bottom, shobj->bottom + shobj->size,
	       (int) shobj->size);
      FOR_EACH_LOAD_PHDR (k, phdr, info->dlpi_phdr, info->dlpi_phnum)
	{
	  dprintf (I I I "%lx-%lx %d\n",
		   (unsigned long) (phdr->p_vaddr + info->dlpi_addr),
		   (unsigned long) (phdr->p_vaddr + info->dlpi_addr
				    + phdr->p_memsz),
		   phdr->p_flags);
	}
    }
}

static void
print_regions (struct shobj *regions[], size_t nr_regions)
{
  size_t i;

  dprintf ("Printing regions\n");
  for (i = 0; i < nr_regions; i++)
    {
      dprintf (I "Region %d (%d shobjs%s)\n", (int) i,
	       regions[i]->nr_in_same_region,
	       regions[i] == libhtlbremap_region ? ", ourselves!" : "");
      print_region (regions[i]);
    }
}

static int
shlibp (struct shobj *shobj)
{
  return shobj->phdr_info->dlpi_addr != 0;
}

static addr_t
compute_region_address (struct shobj *shobj, addr_t *bottom)
{
  addr_t top;

  *bottom = shobj->bottom;
  top = *bottom + shobj->size;
  for (shobj = shobj->next_same_region ;shobj; shobj = shobj->next_same_region)
    {
      if (shobj->bottom < *bottom)
	*bottom = shobj->bottom;
      if (top < shobj->bottom + shobj->size)
	top = shobj->bottom + shobj->size;
    }
  return top;
}

/* Before we get a chance to change the morecore handler the heap has
   already been used.  Also relocate this intial heap.  */

static void
add_heap (struct vma *areas, struct shobj *heap, struct dl_phdr_info *info, ElfW(Phdr) *phdr)
{
  addr_t top;

  heap->bottom = get_mapping (areas, "[heap]", &top);
  heap->size = top - heap->bottom;
  dprintf (I "heap: %lx-%lx (%d)\n", heap->bottom, top, (int) heap->size);
  heap->phdr_info = info;
  heap->next_same_region = NULL;

  info->dlpi_addr = heap->bottom;
  info->dlpi_name = "fake shobj for the heap";
  info->dlpi_phnum = 1;
  info->dlpi_phdr = phdr;

  memset (phdr, 0, sizeof (ElfW(Phdr)));
  phdr->p_memsz = heap->size;
  phdr->p_type = PT_LOAD;
}

struct vma_overlap
{
  addr_t bottom;
  addr_t top;
};

static struct vma_overlap
compute_overlap (struct vma *area, addr_t huge_bottom, addr_t huge_top)
{
  struct vma_overlap r = {.bottom = 0, .top = 0};

  if (area->end <= huge_bottom)
    return r;
  if (area->start >= huge_top)
    return r;

  r.bottom = (huge_bottom < area->start) ? area->start : huge_bottom;
  r.top = (huge_top > area->end) ? area->end : huge_top;

  return r;
}

static int
can_remap(struct vma *areas, addr_t huge_bottom, addr_t huge_top)
{
  while (areas)
    {
      struct vma_overlap ol = compute_overlap(areas, huge_bottom, huge_top);

      /* If there is an overlap that is readable but not private we can't do the remap. */
      if (ol.bottom && (areas->flags & VMA_F_READ) != 0 && (areas->flags & VMA_F_PRIVATE) == 0)
	return 0;

      areas = areas->next;
    }
  return 1;
}

static void
save (struct vma *areas, addr_t huge_bottom, addr_t huge_top, void *to)
{
  int found_offset = 0;
  long offset = 0;

  dprintf (I "Saving.\n");

  while (areas)
    {
      struct vma_overlap ol = compute_overlap(areas, huge_bottom, huge_top);

      if (ol.bottom && (areas->flags & VMA_F_READ) != 0)
	{
	  if (!found_offset)
	    {
	      offset = ((long) to) - (long) ALIGN_DOWN (ol.bottom, hugepagesize);
	      found_offset = 1;
	    }
	  dprintf (I I "%p -> %p (%d bytes) (%p - %p)\n",
		   (void *) ol.bottom,
		   (void *) (ol.bottom + offset),
		   (int) (ol.top - ol.bottom), (void *) ol.bottom, (void *) ol.top);

	  memcpy ((void *) (ol.bottom + offset),
		  (void *) ol.bottom,
		  ol.top - ol.bottom);
	}
      areas = areas-> next;
    }
}

static void
remap_region (struct vma *areas, struct shobj *shobj, int region)
{
  int fd;
  int r;
  addr_t bottom, top;
  void *save_area;
  addr_t huge_bottom, huge_top;
  char *p;

  top = compute_region_address (shobj, &bottom);

  huge_bottom = ALIGN_DOWN (bottom, hugepagesize);
  huge_top = ALIGN (top, hugepagesize);

  if (!can_remap (areas, huge_bottom, huge_top))
    return;

  fd = new_unlinked_fd ("prog-save-");
  save_area = alloc_save_area (fd, huge_top - huge_bottom);
  if (!save_area)
    {
      print_error (
"htlbremap: Can't allocate memory for save area (region %d: %zd bytes)\n",
         region, top - bottom);
      return;
    }

  save (areas, huge_bottom, huge_top, save_area);
  
  r = msync(save_area, huge_top - huge_bottom, MS_SYNC);
  if (r < 0)
    {
      print_error (
"htlbremap: Can't msync save area (region %d: %zd bytes)\n",
         region, top - bottom);
      return;
    }
  free_save_area (save_area, huge_top - huge_bottom);

  p = mmap ((void *) (long) huge_bottom, huge_top - huge_bottom,
	    PROT_READ | PROT_WRITE | PROT_EXEC,
	    MAP_PRIVATE | MAP_FIXED, fd, 0);

  if (p == NULL)
    {
        print_error ("htblremap: mmap failed\n");
        exit (errno);
    }
  
  close (fd);
}

static int
so_compare (const void *a, const void *b)
{
  struct shobj **so1 = (void *) a;
  struct shobj **so2 = (void *) b;
  if ((*so1)->phdr_info->dlpi_addr > (*so2)->phdr_info->dlpi_addr)
    return 1;
  else if  ((*so1)->phdr_info->dlpi_addr < (*so2)->phdr_info->dlpi_addr)
    return -1;
  return 0;
}

static void
sort_shobjs (struct shobj *shobjs[], size_t nr_shobjs)
{
  qsort (shobjs, nr_shobjs, sizeof (shobjs[0]), so_compare);
}

static void
compute_regions (struct shobj *shobjs[], size_t nr_shobjs)
{
  size_t i;

  for (i = 1; i < nr_shobjs; i++)
    {
      struct shobj *shobj = shobjs[i], *prev = shobjs[i - 1];
      addr_t prev_huge_top = ALIGN (prev->bottom + prev->size, hugepagesize);
      addr_t huge_bottom = ALIGN_DOWN (shobj->bottom, hugepagesize);

      if (huge_bottom < prev_huge_top)
	prev->next_same_region = shobj;
    }
}

static void
find_libhtlbremap_region (struct shobj *regions[], size_t nr_regions)
{
  size_t i;
  struct shobj *so;

  for (i = 0; i < nr_regions; i++)
    for (so = regions[i]; so; so = so->next_same_region)
      if (so == libhtlbremap_so)
	{
	  libhtlbremap_region = regions[i];
	  return ;
	}
}

void
remap_regions (struct vma *areas)
{
  size_t memsize;
  char *mem;
  struct shobj **shobjs, **regions;
  size_t nr_shobjs, nr_regions;
  size_t i;

  /* While creating the regions also make a local copy of phdr_info.
     The program headers are always in the text segment so when we
     unmap text we lose this data.  Copy it to the stack.  The heap is
     right after the main executable and we will implicitly unmap it
     once we map in the hugetbl page over the same virtual address
     region.  */
  memsize = compute_shobjs_memsize (&nr_shobjs);
  mem = alloca (memsize);
  /* Count the heap "shlib".  */
  nr_shobjs++;
  shobjs = alloca (nr_shobjs * sizeof (struct shobjs *));
  create_shobjs (shobjs, mem, memsize);
  shobjs[nr_shobjs - 1] = alloca (sizeof (struct shobj));
  add_heap (areas, shobjs[nr_shobjs - 1],
	    alloca (sizeof (struct dl_phdr_info)),
	    alloca (sizeof (ElfW(Phdr))));

  sort_shobjs (shobjs, nr_shobjs);
  compute_regions (shobjs, nr_shobjs);

  regions = alloca (nr_shobjs * sizeof (struct shobjs *));
  nr_regions = create_regions (shobjs, nr_shobjs, regions);
  find_libhtlbremap_region (regions, nr_regions);
  print_regions (regions, nr_regions);

  for (i = 0; i < nr_regions; i++)
    {
      dprintf ("Remapping region %d\n", (int) i);
      print_region (regions[i]);
      if ((shlibp (regions[i]) && !remap_shlibs)
	  || (!shlibp (regions[i]) && !remap_exe))
	dprintf (I I "Ignoring\n");
      else
	remap_region (areas, regions[i], i);
    }
}
