#ifndef MP3_CODEC_H
#define MP3_CODEC_H

#include "Codec.h"

class MP3Codec: public Codec
{
public:
  MP3Codec();

  virtual
  ~MP3Codec(){}

  virtual
  unsigned int
  decode();

};

class MP3Sample : public EncodedSample
{
public:
  MP3Sample()
  {
    OSSS_BASE_CLASS( EncodedSample )
  }

  virtual
  osss::osss_polymorphic< Codec >
  getCodec() const
  {
    return MP3Codec();
  }
};

#endif // MP3_CODEC_H
