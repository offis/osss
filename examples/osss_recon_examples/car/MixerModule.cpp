#include "MixerModule.h"

// Mix the two audio buffers into an output
// stream with 44,1 kHz
void MixerModule::mixerThread()
{
  po_audio_data.write(WaveSample(0,0));
#ifndef SYNTHESIS
  OSSS_MESSAGE(true, "waiting for reset");
#endif
  while (pi_reset.read() == false){ wait(); }
#ifndef SYNTHESIS
  OSSS_MESSAGE(true, "waiting for reset low");
#endif
  wait();
  while (pi_reset.read() == true){ wait(); }
  OSSS_MESSAGE(true, "reset low seen, working");

#ifndef SYNTHESIS
  bool data_seen        = false;
  bool underrun_message = false;
  bool speech_message   = false;
#endif

  WaveSample speech_sample, music_sample;
  while(true)
  {
    // one sample per raising wave clock edge

    // fetch data
    if (false == pi_music_buffer_empty.read())
    {
#ifndef SYNTHESIS
      data_seen = true;
      OSSS_MESSAGE(underrun_message, "there are new music samples to fetch again");
      // if underrun occurs: speak up!
      underrun_message = false;
#endif

      // talk to the guarded buffer
      music_sample = port_to_music_buffer->get();
      //OSSS_MESSAGE(true, "just fetched music sample");
    }
    else
    {
      // silence
      music_sample = WaveSample();
      if (data_seen && (!underrun_message))
      {
#ifndef SYNTHESIS
        OSSS_MESSAGE( true, "music buffer underrun");
        // if underrun continues: shut up!
        underrun_message = true;
#endif
      }
    }
    // Is there something to say?
    if (false == pi_speech_buffer_empty.read())
    {
#ifndef SYNTHESIS
      OSSS_MESSAGE(!speech_message, "next: fetch speech sample");
      speech_message = true;
#endif
      speech_sample = port_to_music_buffer->get();
    }
    else
    {
      // empty sample
      speech_sample = WaveSample();
#ifndef SYNTHESIS
      OSSS_MESSAGE( speech_message, "no more speech samples available");
      speech_message = false;
#endif
    }
    // look for edge
#ifndef SYNTHESIS
    //OSSS_MESSAGE( true, "waiting for rising audio clock edge");
#endif
    while (pi_audio_clock.read() == true) { wait(); }
    while (pi_audio_clock.read() == false) { wait(); }
    // it is here. Write mixed data.
#ifndef SYNTHESIS
    /*
    OSSS_MESSAGE( true, "new sample:"
                     << WaveSample(   (speech_sample.left >> 1)
                                    + (music_sample.left >> 1),
                                      (speech_sample.right >> 1)
                                    + (music_sample.right >> 1) )
                );
    */
#endif
    po_audio_data.write(WaveSample(   (speech_sample.left >> 1)
                                    + (music_sample.left >> 1),
                                      (speech_sample.right >> 1)
                                    + (music_sample.right >> 1) ));
    wait();
  }
}
