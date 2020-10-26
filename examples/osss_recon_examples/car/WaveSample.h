#ifndef WAVE_SAMPLE_H
#define WAVE_SAMPLE_H

#include "stdint.h"
#include <string>
#include "RingBuffer.h"

#include <systemc>


class WaveSample
{
  public:
  WaveSample()
  {
    left = 0;
    right = 0;
  }
  WaveSample( sc_dt::sc_int< 16 > l, sc_dt::sc_int< 16 > r)
  {
    left = l;
    right = r;
  }
  sc_dt::sc_int< 16 > left;
  sc_dt::sc_int< 16 > right;

  ~WaveSample(){}

  bool
  operator==(const WaveSample & other)
  {
    return (other.left == left) && (other.right == right);
  }
};

#ifndef SYNTHESIS
namespace sc_core {
inline
void sc_trace ( sc_core::sc_trace_file *&, const WaveSample &, const std::string &)
{
}
} // namsepace sc_core

inline
std::ostream & operator<<(std::ostream & os, const WaveSample & object)
{
  os << " l=" << object.left << "/r=" << object.right << " ";
  return os;
}
#endif // ! SYNTHESIS

#endif // WAVE_SAMPLE_H
