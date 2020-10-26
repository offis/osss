#ifndef AAC_CODEC_H
#define AAC_CODEC_H

#include "Codec.h"

class AACCodec : public Codec
{
  public:
  AACCodec()
  {
    OSSS_BASE_CLASS( Codec );
  }

  virtual
  ~AACCodec(){}

  virtual
  unsigned int
  decode();
};

class AACSample : public EncodedSample
{
  public:
  AACSample()
  {
    OSSS_BASE_CLASS( EncodedSample )
  }

  virtual
  osss::osss_polymorphic< Codec >
  getCodec() const
  {
    return AACCodec();
  }
};

#endif // AAC_CODEC_H
