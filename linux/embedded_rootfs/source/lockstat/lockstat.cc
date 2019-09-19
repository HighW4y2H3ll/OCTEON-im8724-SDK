/*

TODO
* tls seems to call __tls_get_addr

*/

#include <iostream>
#include "wrappers.h"
#include "cond.h"
#include "mutex.h"
#include "stats.h"
#include "spin.h"
#include "utils.h"

int Stat_time::width;

template<typename Stat, typename Lock>
__thread __gnu_cxx::hash_map<Lock*, Stat*>*
Stats<Stat, Lock>::thread_local_map;

template<typename Lock>
Lock*
Stat<Lock>::filter;

template<typename Lock>
bool
Stat<Lock>::filter_set;

Stats<Cond_stat> Cond_stat::stats;
Stats<Mutex_stat> Mutex_stat::stats;
Stats<Spinlock_stat> Spinlock_stat::stats;

// Print stats upon exit.  NOTE that static objects are constructed in
// the order they are defined in the file and destructed in reverse
// order so we want Print_stat at the end of the file.

class Print_stat
{
public:
  Print_stat()
    : t0_(Stat_time::now())
  {
    Cond_stat::set_filter("LOCKSTAT_FILTER_COND");
    Mutex_stat::set_filter("LOCKSTAT_FILTER_MUTEX");
    Spinlock_stat::set_filter("LOCKSTAT_FILTER_SPINLOCK");
  }

  ~Print_stat()
  {
    Wrapper::pthread_mutex_lock(&m_);
    Stat_time t1 = Stat_time::now();
    Stat_time total =  t1 - t0_;
    std::cout << std::endl;
    pp() << "Total cycles: " << total << std::endl;

    // Locks for I/O stream give inconsitent summaries as they get
    // accounted even as we print them.
    initialized.unset();

    pp() << "Spinlock statistics:" << std::endl;
    Spinlock_stat::stats.print_all(total);
    pp() << std::endl;
    pp() << "Mutex statistics:" << std::endl;
    Mutex_stat::stats.print_all(total);
    pp() << std::endl;
    pp() << "Condition variable statistics:" << std::endl;
    Cond_stat::stats.print_all(total);

    Wrapper::pthread_mutex_unlock(&m_);
  }

private:
  pthread_mutex_t m_;
  Stat_time t0_;
} print_stat;

Initialized initialized;
