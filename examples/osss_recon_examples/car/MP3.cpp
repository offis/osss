#include "MP3.h"

#define MP3_DECODE_TIME sc_time(2300, SC_US)
#define MP3_LEFT_CH 1
#define MP3_RIGHT_CH 2
#define MP3_COMPRESSION_RATIO 8

#include <osss.h> // bring library symbols into scope

MP3Codec::MP3Codec()
{
  OSSS_BASE_CLASS( Codec );
}

unsigned int
MP3Codec::decode()
{
   //OSSS_MESSAGE(true, "decoding start");

   // Dummy code, fetch and count the available samples
   unsigned int count = 0;
   while (! encoded.isEmpty())
   {
     osss_polymorphic< EncodedSample > tmp = encoded.get();
     ++count;
   }

   // Simulate a decompression
   while ((count > 0) && (! wave.isFull()))
   {
     for (int i = 0; i < MP3_COMPRESSION_RATIO; ++i)
     {
       if (! wave.isFull())
       {
         wave.put(WaveSample(MP3_LEFT_CH, MP3_RIGHT_CH));
       }
       else
       {
         OSSS_MESSAGE(true, "wave buffer overflow, skipping samples");
       }
     }
     --count;
   }

   wait( MP3_DECODE_TIME );
   //OSSS_MESSAGE(true, "decoding stop");
   return WAVE_BLOCK_SIZE;
}
