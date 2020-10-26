#include "AAC.h"

#define AAC_DECODE_TIME sc_time(230, SC_US)
#define AAC_LEFT_CH 1
#define AAC_RIGHT_CH 2
#define AAC_COMPRESSION_RATIO 8

#include <osss.h>

unsigned int
AACCodec::decode()
{
   //OSSS_MESSAGE(true, "decoding start");

   // dummy code
   unsigned int count = 0;
   while (! encoded.isEmpty())
   {
     osss_polymorphic< EncodedSample > tmp = encoded.get();
     ++count;
   }

   while ((count > 0) && (! wave.isFull()))
   {
     for (int i = 0; i < AAC_COMPRESSION_RATIO; ++i)
     {
       if (! wave.isFull())
       {
         wave.put(WaveSample(AAC_LEFT_CH, AAC_RIGHT_CH));
       }
       else
       {
         OSSS_MESSAGE(true, "wave buffer overflow, skipping samples");
       }
     }
     --count;
   }

   wait( AAC_DECODE_TIME );
   //OSSS_MESSAGE(true, "decoding stop");
   return WAVE_BLOCK_SIZE;
}
