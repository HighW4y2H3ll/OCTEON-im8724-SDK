/* -*-C++-*- */

#include "stat-time.h"
#include "stats.h"

class Spinlock_stat : public Stat<pthread_spinlock_t>
{
public:
  Spinlock_stat(lock_type* lock)
    : Stat<pthread_spinlock_t>(lock)
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
  operator+=(const Spinlock_stat& other)
  {
    acquire_time_ += other.acquire_time_;
    hold_time_ += other.hold_time_;
    release_time_ += other.release_time_;
  }

  friend bool
  operator<(const Spinlock_stat& a, const Spinlock_stat& b)
  { return a.acquire_time_ < b.acquire_time_; }

  enum type { lock, trylock };

private:
  Stat_time acquire_time_;
  Stat_time hold_time_;
  Stat_time release_time_;

  Stat_time hold_begin_;

public:
  static int
  common_lock(pthread_spinlock_t*, type);
  friend int
  pthread_spin_unlock(pthread_spinlock_t*);

  static Stats<Spinlock_stat> stats;
};

int
Spinlock_stat::common_lock(pthread_spinlock_t* s, type t)
{
  if (!initialized || filter_out(s))
    {
      if (__builtin_expect(t == lock, 1))
	return Wrapper::pthread_spin_lock(s);
      else
	return Wrapper::pthread_spin_trylock(s);
    }

  Spinlock_stat& ss = Spinlock_stat::stats.lookup(s);
  Stat_time t0 = Stat_time::now();

  int ret;
  if (__builtin_expect(t == lock, 1))
    ret = Wrapper::pthread_spin_lock(s);
  else
    ret = Wrapper::pthread_spin_trylock(s);
  if (ret != 0)
    return ret;

  ss.hold_begin_ = Stat_time::now();
  ss.acquire_time_ += ss.hold_begin_ - t0;
  return ret;
}

extern "C" int
pthread_spin_lock(pthread_spinlock_t* s)
{ return Spinlock_stat::common_lock(s, Spinlock_stat::lock); }

extern "C" int
pthread_spin_trylock(pthread_spinlock_t* s)
{ return Spinlock_stat::common_lock(s, Spinlock_stat::trylock); }

extern "C" int
pthread_spin_unlock(pthread_spinlock_t* s) throw()
{
  if (!initialized || Spinlock_stat::filter_out(s))
    return Wrapper::pthread_spin_unlock(s);

  Spinlock_stat& ss = Spinlock_stat::stats.lookup(s);
  Stat_time t0 = Stat_time::now();

  int ret = Wrapper::pthread_spin_unlock(s);
  if (ret != 0)
    return ret;
  // Apparently you can have unlock without a lock.  Ignore these.
  if (!ss.hold_begin_)
    return ret;

  ss.hold_time_ += t0 - ss.hold_begin_;
  ss.hold_begin_ = 0;
  ss.release_time_ += Stat_time::now() - t0;

  return ret;
}
