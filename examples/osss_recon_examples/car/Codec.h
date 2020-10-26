#ifndef CODEC_H
#define CODEC_H

#include "EncodedSample.h"
#include "WaveSample.h"

#ifndef SYNTHESIS
#include <iostream>
#endif

#define MAX_COMPRESSION_RATIO 8
#define ENCODED_BLOCK_SIZE 72

#define WAVE_BLOCK_SIZE (MAX_COMPRESSION_RATIO * ENCODED_BLOCK_SIZE)
//27ms == 576 samples
//#define WAVE_BLOCK_SIZE 576

class Codec : public osss::osss_object
{
  protected:
  RingBuffer< osss::osss_polymorphic< EncodedSample >,
              ENCODED_BLOCK_SIZE > encoded;
  RingBuffer< WaveSample, WAVE_BLOCK_SIZE > wave;

  public:
  // OSSS_TAG( Codec )

  virtual
  WaveSample
  grabWave();

  virtual
  ~Codec()
  { }

  Codec()
  { }

  // returns number of available decoded samples
  virtual
  unsigned int
  decode();

  bool
  put( EncodedSample raw );
};

#endif // CODEC_H
