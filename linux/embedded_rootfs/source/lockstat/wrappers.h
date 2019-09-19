/* -*-C++-*- */

#include <pthread.h>
#include <dlfcn.h>
#include <cassert>

template<typename T>
T*
pthread_symbol(const char *name, T*)
{
  // dlsym seems to broken that it does not return the symbol with the
  // default version number.
  T* glibc_sym = (T*) ::dlvsym(RTLD_NEXT, name, "GLIBC_2.3.2");
  if (glibc_sym)
    return glibc_sym;
  return (T*) ::dlsym(RTLD_NEXT, name);
}

#define PTHREAD_SYMBOL(S) pthread_symbol(#S, ::S)

struct Wrapper
{
  static int
  pthread_mutex_lock(pthread_mutex_t *m)
  {
    static int (*p)(pthread_mutex_t*);
    // Initialize manually to avoid mutex from libstdc++.
    if (!p)
      p = PTHREAD_SYMBOL(pthread_mutex_lock);
    assert(p);
    return p(m);
  }
    
  static int
  pthread_mutex_trylock(pthread_mutex_t *m)
  {
    static int (*p)(pthread_mutex_t*);
    if (!p)
      p = PTHREAD_SYMBOL(pthread_mutex_trylock);
    assert(p);
    return p(m);
  }

  static int
  pthread_mutex_unlock(pthread_mutex_t *m)
  {
    static int (*p)(pthread_mutex_t *);
    if (!p)
      p = PTHREAD_SYMBOL(pthread_mutex_unlock);
    assert(p);
    return p(m);
  }

  static int
  pthread_cond_wait(pthread_cond_t* c, pthread_mutex_t* m)
  {
    static int (*p)(pthread_cond_t*, pthread_mutex_t*);
    if (!p)
      p = PTHREAD_SYMBOL(pthread_cond_wait);
    return p(c, m);
  }

  static int
  pthread_cond_timedwait(pthread_cond_t* c, pthread_mutex_t* m,
			 const struct timespec *t)
  {
    static int (*p)(pthread_cond_t*, pthread_mutex_t*, const struct timespec*);
    if (!p)
      p = PTHREAD_SYMBOL(pthread_cond_timedwait);
    return p(c, m, t);
  }

  static int
  pthread_cond_signal(pthread_cond_t* c)
  {
    static int (*p)(pthread_cond_t*);
    if (!p)
      p = PTHREAD_SYMBOL(pthread_cond_signal);
    return p(c);
  }

  static int
  pthread_cond_broadcast(pthread_cond_t* c)
  {
    static int (*p)(pthread_cond_t*);
    if (!p)
      p = PTHREAD_SYMBOL(pthread_cond_broadcast);
    return p(c);
  }

  static int
  pthread_spin_lock(pthread_spinlock_t *m)
  {
    static int (*p)(pthread_spinlock_t*);
    if (!p)
      p = PTHREAD_SYMBOL(pthread_spin_lock);
    assert(p);
    return p(m);
  }
    
  static int
  pthread_spin_trylock(pthread_spinlock_t *m)
  {
    static int (*p)(pthread_spinlock_t*);
    if (!p)
      p = PTHREAD_SYMBOL(pthread_spin_trylock);
    assert(p);
    return p(m);
  }

  static int
  pthread_spin_unlock(pthread_spinlock_t *m)
  {
    static int (*p)(pthread_spinlock_t *);
    if (!p)
      p = PTHREAD_SYMBOL(pthread_spin_unlock);
    assert(p);
    return p(m);
  }

};
