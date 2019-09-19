/* -*-C++-*- */

#ifndef MUTEX_H
#define MUTEX_H

#include "stat-time.h"

class Mutex_stat : public Stat<pthread_mutex_t>
{
public:
  Mutex_stat(lock_type* lock)
    : Stat<pthread_mutex_t>(lock)
  {}

  std::string
  print(Stat_time total) const
  {
    std::ostringstream os;
    os << "acquire " << acquire_time_.str(total)
       << " hold " << hold_time_.str(total)
       << " release " << release_time_.str(total);
    return os.str();
  }

  void
  operator+=(const Mutex_stat& other)
  {
    acquire_time_ += other.acquire_time_;
    hold_time_ += other.hold_time_;
    release_time_ += other.release_time_;
  }

  friend bool
  operator<(const Mutex_stat& a, const Mutex_stat& b)
  { return a.acquire_time_ < b.acquire_time_; }

  void
  remove_hold_time(Stat_time t)
  { hold_time_ += -t; }

  enum type { lock, trylock };

private:
  Stat_time acquire_time_;
  Stat_time hold_time_;
  Stat_time release_time_;

  Stat_time hold_begin_;

public:
  static int
  common_lock(pthread_mutex_t*, type);
  friend int
  pthread_mutex_unlock(pthread_mutex_t*);

  static Stats<Mutex_stat> stats;
};

int
Mutex_stat::common_lock(pthread_mutex_t* m, type t)
{
  if (!initialized || filter_out(m))
    {
      if (__builtin_expect(t == lock, 1))
	return Wrapper::pthread_mutex_lock(m);
      else
	return Wrapper::pthread_mutex_trylock(m);
    }

  Mutex_stat& ms = Mutex_stat::stats.lookup(m);
  Stat_time t0 = Stat_time::now();

  int ret;
  if (__builtin_expect(t == lock, 1))
    ret = Wrapper::pthread_mutex_lock(m);
  else
    ret = Wrapper::pthread_mutex_trylock(m);
  if (ret != 0)
    return ret;

  ms.hold_begin_ = Stat_time::now();
  ms.acquire_time_ += ms.hold_begin_ - t0;
  return ret;
}

extern "C" int
pthread_mutex_lock(pthread_mutex_t* m)
{ return Mutex_stat::common_lock(m, Mutex_stat::lock); }

extern "C" int
pthread_mutex_trylock(pthread_mutex_t* m)
{ return Mutex_stat::common_lock(m, Mutex_stat::trylock); }

extern "C" int
pthread_mutex_unlock(pthread_mutex_t* m) throw()
{
  if (!initialized || Mutex_stat::filter_out(m))
    return Wrapper::pthread_mutex_unlock(m);

  Mutex_stat& ms = Mutex_stat::stats.lookup(m);
  Stat_time t0 = Stat_time::now();

  int ret = Wrapper::pthread_mutex_unlock(m);
  if (ret != 0)
    return ret;

  assert (ms.hold_begin_ != 0);
  ms.hold_time_ += t0 - ms.hold_begin_;
  ms.hold_begin_ = 0;
  ms.release_time_ += Stat_time::now() - t0;
  
  return ret;
}

#endif
