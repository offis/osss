#include "Codec.h"

unsigned int
Codec::decode()
{
  OSSS_PURE_VIRTUAL_RETURN( 0 );
}

bool
Codec::put( EncodedSample raw )
{
  encoded.put(raw);
  return encoded.isFull();
}

WaveSample
Codec::grabWave()
{
  return wave.get();
}
