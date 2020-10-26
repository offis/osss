#ifndef VORBIS_CODEC_H
#define VORBIS_CODEC_H

#include "Codec.h"

class VorbisCodec : public Codec
{
  public:
  VorbisCodec();

  virtual
  ~VorbisCodec();

  virtual
  unsigned int
  decode();
};

class VorbisSample : public EncodedSample
{
  public:
  VorbisSample()
  {
    OSSS_BASE_CLASS( EncodedSample )
  }

  virtual
  osss::osss_polymorphic< Codec >
  getCodec() const
  {
    return VorbisCodec();
  }
};

#endif // VORBIS_CODEC_H
