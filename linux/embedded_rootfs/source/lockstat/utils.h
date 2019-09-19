/* -*-C++-*- */

#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>
#include <syscall.h>

static int
gettid (void)
{
  return syscall (__NR_gettid);
}

// LD_PRELOAD libs are constructed last.  This class provides a way to
// detect whether the library is already initialized and we are in a
// valid state to run.  Otherwise just call the original functions we
// are wrapping.

class Initialized
{
public:
  Initialized()
    : i_(234523)
  {}

  operator bool () const
  { return i_ == 234523; }

  void
  unset(void)
  { i_ = 0; }

private:
  int i_;
};

extern Initialized initialized;

static inline std::ostream&
pp(void)
{
  return std::cout << "lockstat: ";
}

#endif
