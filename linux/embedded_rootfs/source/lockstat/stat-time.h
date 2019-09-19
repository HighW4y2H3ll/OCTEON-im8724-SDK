/* -*-C++-*- */

#ifndef STAT_TIME_H
#define STAT_TIME_H

#include <sstream>
#include <iomanip>
#include <stdint.h>

class Stat_time
{
public:
  Stat_time(uint64_t t = 0)
    : t_(t)
  {}

  operator uint64_t () const
  { return t_; }

  std::string
  str(const Stat_time &total) const
  {
    std::ostringstream os;
    os.precision(1);
    if (width)
      os.width(width);
    os.setf(std::ios_base::fixed, std::ios_base::floatfield);
    os << t_ << "(" << std::setw(5) << float(t_)/total.t_*100 << "%)";
    return os.str();
  }

  Stat_time&
  operator+=(const Stat_time& rhs)
  {
    t_ += rhs.t_;
    return *this;
  }

  static Stat_time
  now(void)
  {
    uint64_t cy;
    asm volatile ("rdhwr %0,$31" : "=r"(cy));
    return Stat_time(cy);
  }

  static int width;

private:
  uint64_t t_;
};

#endif
