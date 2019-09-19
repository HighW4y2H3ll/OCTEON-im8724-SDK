#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <asm/unistd.h>
#include <sys/cachectl.h>
#include <stdlib.h>

#include <sys/mman.h>

struct _sig_ucontext {
    unsigned long         uc_flags;
    struct _sig_ucontext  *uc_link;
    stack_t               uc_stack;
    struct sigcontext uc_mcontext;
    sigset_t      uc_sigmask;
};

static void *xi_mem;
static void *buffer_mem;
static void *ri_mem;

static sigjmp_buf env;

int d;

void handle_xi1(int sig, siginfo_t *siginfo, void *info)
{
  struct _sig_ucontext *ucontext = info;

  /* When we get here, the PC should be equal to &d.  */
  if ((unsigned long long)&d == ucontext->uc_mcontext.sc_pc) {
    siglongjmp(env, 1);
  }

  printf("XI-1 test didn't stop in the right place.\n");
  abort();
}

void handle_xi2(int sig, siginfo_t *siginfo, void *info)
{
  struct _sig_ucontext *ucontext = info;

  /* When we get here, the PC should be equal to xi_mem.  */
  if ((unsigned long long)((unsigned long)xi_mem) == ucontext->uc_mcontext.sc_pc) {
    siglongjmp(env, 1);
  }

  printf("XI-2 test didn't stop in the right place.\n");
  abort();
}

void handle_ri1(int sig, siginfo_t *siginfo, void *info)
{
  printf("RI execute test shouldn't fault.\n");
  abort();
}

void handle_ri2(int sig, siginfo_t *siginfo, void *info)
{
  printf("RI succeeded.\n");
  exit(0);
}

static void (*try_xi)(void);
static void (*try_ri)(void);

static void insert_return(void *mem)
{
  uint32_t *p = mem;

  *p = 0x03e00008; /* jr ra */
  p++;
  *p = 0; /* nop */
  cacheflush(mem, 8, BCACHE);
}

int main(int argc, char *argv[])
{
  int pagesize, i;
  uint32_t u;
  struct sigaction sa;
  void (*try_xi1)() = (void *)&d;

  pagesize = getpagesize();

  xi_mem = mmap(NULL, 2 * pagesize, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (xi_mem == MAP_FAILED) {
    perror("mmap xi_mem failed.");
    abort();
  }
  insert_return(xi_mem);
  try_xi = xi_mem;

  buffer_mem = xi_mem + pagesize;

  i = mprotect(buffer_mem, pagesize, PROT_NONE);
  if (i != 0) {
    perror("mprotect failed.");
    abort();
  }

  ri_mem = mmap(NULL, pagesize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (ri_mem == MAP_FAILED) {
    perror("mmap ri_mem failed.");
    abort();
  }
  insert_return(ri_mem);
  try_ri = ri_mem;

  i = mprotect(ri_mem, pagesize, PROT_EXEC);
  if (i != 0) {
    perror("mprotect failed.");
    abort();
  }

  memset(&sa, 0, sizeof(sa));

  sa.sa_sigaction = handle_xi1;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_SIGINFO;

  i = sigaction(SIGSEGV, &sa, NULL);
  if (i != 0) {
    perror("sigaction failed.");
    abort();
  }

  if (sigsetjmp(env, 1)) {
    printf("XI-1 succeeded.\n");
  } else {
    printf("Trying XI-1\n");
    try_xi1();
    printf("XI-1 failed (it executed).\n");
    abort();
  }

  memset(&sa, 0, sizeof(sa));

  sa.sa_sigaction = handle_xi2;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_SIGINFO;

  i = sigaction(SIGSEGV, &sa, NULL);
  if (i != 0) {
    perror("sigaction failed.");
    abort();
  }

  if (sigsetjmp(env, 1)) {
    printf("XI-2 succeeded.\n");
  } else {
    printf("Trying XI-2\n");
    try_xi();
    printf("XI-2 failed (it executed).\n");
    abort();
  }

  printf("Trying RI\n");

  memset(&sa, 0, sizeof(sa));

  sa.sa_sigaction = handle_ri1;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_SIGINFO;

  i = sigaction(SIGSEGV, &sa, NULL);
  if (i != 0) {
    perror("sigaction failed.");
    abort();
  }

  asm volatile ( "" : : : "memory");

  try_ri();

  memset(&sa, 0, sizeof(sa));

  sa.sa_sigaction = handle_ri2;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_SIGINFO;

  i = sigaction(SIGSEGV, &sa, NULL);
  if (i != 0) {
    perror("sigaction failed.");
    abort();
  }
  
  u = *(uint32_t *)ri_mem;

  printf("RI failed, it should not be able to read %x\n", u);

  abort();
}
