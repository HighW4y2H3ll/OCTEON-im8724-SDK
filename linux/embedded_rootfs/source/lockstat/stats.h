/* -*-C++-*- */

#ifndef STATS_H
#define STATS_H

#include <utility>
#include <ext/hash_map>
#include <cmath>
#include <algorithm>
#include "utils.h"

// Add some hash functions we need.

namespace __gnu_cxx 
{
  template<typename T>
  struct hash<T*>
  {
    size_t
    operator()(T* p) const
    { return reinterpret_cast<size_t>(p); }
  };
};

template<typename Stat, typename Lock = typename Stat::lock_type>
class Stats
{
public:
  typedef typename Stat::lock_type lock_type;
  typedef std::pair<lock_type*, Stat*> Lock_stat_pair;
  typedef __gnu_cxx::hash_map<Lock*, Stat*> Stats_map;
  typedef __gnu_cxx::hash_map<int, Stats_map*> Thread_stats_map;
  typedef typename Thread_stats_map::iterator Thread_stats_iter;

  Stat&
  lookup(Lock *lock)
  {
    if (!thread_local_map)
      {
	thread_local_map = new Stats_map;
	Wrapper::pthread_mutex_lock(&all_maps_mutex_);
	thread_stats_map_[gettid()] = thread_local_map;
	Wrapper::pthread_mutex_unlock(&all_maps_mutex_);
      }

    Stat*& stat = (*thread_local_map)[lock];
    if (!stat)
      stat = new Stat(lock);
    return *stat;
  }

  struct Sort_stats
  {
    bool
    operator()(const Lock_stat_pair& a, const Lock_stat_pair& b)
    { return *a.second < *b.second; }
  };

  void
  print_all(Stat_time total)
  {
    typedef __gnu_cxx::hash_map<int, Stat*> Tid_to_stat;
    typedef __gnu_cxx::hash_map<lock_type*, Tid_to_stat> Lock_to_tid_to_stat;
    typedef __gnu_cxx::hash_map<lock_type*, Stat*> Lock_to_sum_stat;
    typedef std::vector<std::pair<lock_type*, Stat*> > Lock_stat_vector;
    
    Stat_time::width = (int)std::log10((uint64_t)total) + 1;

    Lock_to_tid_to_stat lock_to_tid_to_stat;
    Lock_to_sum_stat lock_to_sum_stat;

    // Create lock->tid->stat and sorted lock->stat_sum for
    // tid->lock->stat entries.
    for (Thread_stats_iter thread_stats_iter = thread_stats_map_.begin();
	 thread_stats_iter != thread_stats_map_.end();
	 ++thread_stats_iter)
      for (typename Stats_map::iterator stat =
	     thread_stats_iter->second->begin();
	   stat != thread_stats_iter->second->end();
	   ++stat)
	{
	  lock_to_tid_to_stat[stat->first][thread_stats_iter->first] =
	    stat->second;
	  Stat*& s = lock_to_sum_stat[stat->first];
	  if (!s)
	    s = new Stat(stat->first);
	  *s += *stat->second;
	}

    Lock_stat_vector lock_to_sum_sort (lock_to_sum_stat.begin(),
				       lock_to_sum_stat.end());
    std::sort(lock_to_sum_sort.begin(), lock_to_sum_sort.end(), Sort_stats());
    for (typename Lock_stat_vector::reverse_iterator lock_iter =
	   lock_to_sum_sort.rbegin();
	 lock_iter != lock_to_sum_sort.rend();
	 ++lock_iter)
      {
	Stat& sum = *lock_iter->second;

	pp() << "  " << std::setw(16) << sum.name() << " " << sum.print(total)
	     << sum.misc() << std::endl;
	
	pp() << std::setw(18) << "per-thread:" << std::endl;
	for (typename Tid_to_stat::iterator tid_iter =
	       lock_to_tid_to_stat[lock_iter->first].begin();
	     tid_iter != lock_to_tid_to_stat[lock_iter->first].end();
	     ++tid_iter)
	  pp() << "    " << std::setw(14) << tid_iter->first << " "
	       << tid_iter->second->print(total) << std::endl;
      }
  }

private:
  Thread_stats_map thread_stats_map_;
  pthread_mutex_t all_maps_mutex_;

  static __thread Stats_map *thread_local_map;
};

template<typename Lock_type>
class Stat
{
public:
  typedef Lock_type lock_type;

  Stat(lock_type* lock)
    : lock_(lock)
  {}

  std::string
  name(void) const
  {
    std::ostringstream os;
    os << (void*) lock_;
    return os.str();
  }

  std::string
  misc(void) const
  { return ""; }

  static void
  set_filter(const char *varname)
  {
    char* var = ::getenv(varname);
    if (var)
      {
	filter = (lock_type*) ::strtoul(var, 0, 0);
	filter_set = true;
      }
    else
      filter_set = false;
  }

  static bool
  filter_out(lock_type *lock)
  { return filter_set && filter != lock; }

protected:
  lock_type *lock_;

  static lock_type *filter;
  static bool filter_set;
};

#endif
