#include "Vorbis.h"

#define VORBIS_DECODE_TIME sc_time(230, SC_US)
#define VORBIS_LEFT_CH 1
#define VORBIS_RIGHT_CH 2
#define VORBIS_COMPRESSION_RATIO 8

#include <osss.h> //bring library symbols to scope

VorbisCodec::VorbisCodec()
{
  OSSS_BASE_CLASS( Codec );
}

VorbisCodec::~VorbisCodec()
{
}

unsigned int
VorbisCodec::decode()
{
   //OSSS_MESSAGE(true, "decoding start");

   // Fetch && count elements
   unsigned int count = 0;
   while (! encoded.isEmpty())
   {
     osss_polymorphic< EncodedSample > tmp = encoded.get();
     ++count;
   }

   while ((count > 0) && (! wave.isFull()))
   {
     for (int i = 0; i < VORBIS_COMPRESSION_RATIO; ++i)
     {
       if (! wave.isFull())
       {
         wave.put(WaveSample(VORBIS_LEFT_CH, VORBIS_RIGHT_CH));
       }
       else
       {
         OSSS_MESSAGE(true, "wave buffer overflow, skipping samples");
       }
     }
     --count;
   }

   wait( VORBIS_DECODE_TIME );
   //OSSS_MESSAGE(true, "decoding stop");

   return WAVE_BLOCK_SIZE;
}
