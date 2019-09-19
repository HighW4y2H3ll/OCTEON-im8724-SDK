/* -*-C++-*- */

#include "stat-time.h"
#include "stats.h"
#include "mutex.h"
#include "errno.h"

class Cond_stat : public Stat<pthread_cond_t>
{
  typedef __gnu_cxx::hash_map<pthread_mutex_t*, int> pthread_mutex_map;
public:
  Cond_stat(lock_type* lock)
    : Stat<pthread_cond_t>(lock), current_mutex_(0)
  {}

  std::string
  misc(void) const
  {
    std::ostringstream os;
    os << " mutex";
    for (pthread_mutex_map::const_iterator i = mutexes_.begin();
	 i != mutexes_.end();
	 ++i)
      os << " " << i->first;
    return os.str();
  }  

  std::string
  print(Stat_time total) const
  {
    std::ostringstream os;
    os << "wait " << wait_time_.str(total)
       << " signal " << signal_time_.str(total);
    return os.str();
  }

  void
  operator+=(const Cond_stat& other)
  {
    wait_time_ += other.wait_time_;
    signal_time_ += other.signal_time_;
    if (!current_mutex_)
      current_mutex_ = other.current_mutex_;
    mutexes_.insert(other.mutexes_.begin(), other.mutexes_.end());
  }

  friend bool
  operator<(const Cond_stat& a, const Cond_stat& b)
  { return a.signal_time_ < b.signal_time_; }

  void
  used_with(pthread_mutex_t *m)
  {
    current_mutex_ = m;
    mutexes_[m] = 1;
  }

  enum signal_type { signal, broadcast };

private:
  Stat_time wait_time_;
  Stat_time signal_time_;
  pthread_mutex_t *current_mutex_;
  pthread_mutex_map mutexes_;

public:
  static int
  common_wait(pthread_cond_t*, pthread_mutex_t*, const struct timespec*);
  static int
  common_signal(pthread_cond_t*, signal_type);

  static Stats<Cond_stat> stats;
};

int
Cond_stat::common_wait(pthread_cond_t* c, pthread_mutex_t* m,
		       const struct timespec* t)
{
  if (!initialized || filter_out(c))
    {
      if (t)
	return Wrapper::pthread_cond_timedwait(c, m, t);
      else
	return Wrapper::pthread_cond_wait(c, m);
    }

  Cond_stat& cs = Cond_stat::stats.lookup(c);
  Mutex_stat* ms;

  cs.used_with(m);
  if (!Mutex_stat::filter_out(m))
    ms = &Mutex_stat::stats.lookup(m);

  Stat_time t0 = Stat_time::now();

  int ret;
  if (t)
    ret = Wrapper::pthread_cond_timedwait(c, m, t);
  else
    ret = Wrapper::pthread_cond_wait(c, m);
  if (ret != 0
      && ret != ETIMEDOUT)
    return ret;

  Stat_time waited = Stat_time::now() - t0;
  cs.wait_time_ += waited;
  // Mutex is unlocked while waiting for the condition.
  if (ms)
    ms->remove_hold_time(waited);

  return ret;
}

extern "C" int
pthread_cond_wait(pthread_cond_t *c, pthread_mutex_t *m)
{ return Cond_stat::common_wait(c, m, 0); }

extern "C" int
pthread_cond_timedwait(pthread_cond_t *c, pthread_mutex_t *m,
		       const struct timespec *t)
{ return Cond_stat::common_wait(c, m, t); }

int
Cond_stat::common_signal(pthread_cond_t *c, signal_type s)
{
  if (!initialized || filter_out(c))
    {
      if (s == signal)
	return Wrapper::pthread_cond_signal(c);
      else
	return Wrapper::pthread_cond_broadcast(c);
    }

  Cond_stat& cs = Cond_stat::stats.lookup(c);
  Stat_time t0 = Stat_time::now();

  int ret;
  if (s == signal)
    ret = Wrapper::pthread_cond_signal(c);
  else
    ret = Wrapper::pthread_cond_broadcast(c);    

  cs.signal_time_ += Stat_time::now() - t0;
  return ret;
}

extern "C" int
pthread_cond_signal(pthread_cond_t *c)
{ return Cond_stat::common_signal(c, Cond_stat::signal); }

extern "C" int
pthread_cond_broadcast(pthread_cond_t *c)
{ return Cond_stat::common_signal(c, Cond_stat::broadcast); }
