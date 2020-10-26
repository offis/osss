#ifndef ENCODED_SAMPLE_H
#define ENCODED_SAMPLE_H

#ifndef SYNTHESIS
#include <iostream>
#endif
#include "stdint.h"

#include "Defines.h"
#include "RingBuffer.h"

#include <osss>

// forward declaration
class Codec;

class EncodedSample : public osss::osss_object
{
  uint8_t data;

  public:
  EncodedSample()
  {
    OSSS_BASE_CLASS( osss::osss_object );
  }
  // OSSS_TAG( EncodedSample )

  virtual
  ~EncodedSample();

  virtual
  osss::osss_polymorphic< Codec >
  getCodec() const;

  inline
  bool
  operator == (const EncodedSample & right)
  {
    return data == right.data;
  }

  inline
  void
  set(const uint8_t i)
  {
    data = i;
  }
};


inline
void sc_trace ( sc_core::sc_trace_file *&, const EncodedSample &, const std::string & )
{
}

inline
std::ostream &
operator << ( std::ostream & os , const EncodedSample &)
{ return os;}

#endif // ENCODED_SAMPLE_H
