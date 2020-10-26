#include "Testbench.h"
#include "AAC.h"
#include "Vorbis.h"
#include "MP3.h"

#include <osss.h>

#define DEBUG_TESTBENCH false
#define FPL06 true

void Testbench::music_drv()
{
  while (pi_reset.read() == false){wait();}
  OSSS_MESSAGE(true, "reset high seen");
  po_music_req.write(false);
  while (pi_reset.read() == true){ wait(); }
  OSSS_MESSAGE(true, "reset low seen");

#if FPL06 == false
  short int count = 0;
#endif
  osss_polymorphic< EncodedSample > es;

  while (true)
  {
#if FPL06 == true
    es = VorbisSample();
#else
    switch (count)
    {
    case 0:
       OSSS_MESSAGE(true, "generating aac");
       es = AACSample();
       break;
    case 1:
       OSSS_MESSAGE(true, "generating mp3");
       es = MP3Sample();
       break;
    case 2:
       OSSS_MESSAGE(true, "generating vorbis");
       es = VorbisSample();
       break;
    }
#endif

    for (int i = 0; i < ENCODED_BLOCK_SIZE; ++i)
    {
      OSSS_MESSAGE(DEBUG_TESTBENCH, "waiting for ack low");
      while (pi_music_ack.read() == true) { wait(); }
      po_music_enc_sample.write(es);
      po_music_req.write(true);
      wait();
      OSSS_MESSAGE(DEBUG_TESTBENCH, "waiting for ack high");
      while (pi_music_ack.read() == false) { wait(); }
      po_music_req.write(false);
      wait();
    }

#if FPL06 == false
    // Determine next codec
    count = (count+1) % 3;
#endif
  }
}


void Testbench::speech_drv()
{
  while (pi_reset.read() == false){wait();}
  OSSS_MESSAGE(true, "reset high seen");
  po_speech_req.write(false);
  while (pi_reset.read() == true) { wait(); }
  OSSS_MESSAGE(true, "reset low seen");

  osss_polymorphic< EncodedSample > es = MP3Sample();

  while (sc_time_stamp() < sc_time(1250, SC_MS)){ wait(); } // FPL '06 time
  // while (sc_time_stamp() < sc_time(50, SC_US)){ wait(); }
  OSSS_MESSAGE(true, "starting 5 second navigation speech");

  for (int todo = 926 ; todo > 0 ; --todo)
  {
    for (int i = 0; i < ENCODED_BLOCK_SIZE; ++i)
    {
      OSSS_MESSAGE(DEBUG_TESTBENCH, "waiting for ack low");
      while (pi_speech_ack.read() == true) { wait(); }
      po_speech_enc_sample.write(es);
      po_speech_req.write(true);
      wait();
      OSSS_MESSAGE(DEBUG_TESTBENCH, "waiting for ack high");
      while (pi_speech_ack.read() == false) { wait(); }
      po_speech_req.write(false);
      wait();
    }
  }
  OSSS_MESSAGE(true, "delivered 926 frames");
}

void Testbench::security_drv()
{
  while (pi_reset.read() == true)
  {
    po_keyword.write(0);
    OSSS_MESSAGE(true, "resetting");
    wait();
  }
  OSSS_MESSAGE(true, "reset low seen");
  sc_uint< 64 > inv_keyword = 0x08154711;
  po_keyword.write(~ inv_keyword);
}
