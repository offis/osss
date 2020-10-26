#ifndef MIXER_MODULE_H
#define MIXER_MODULE_H

#include "Defines.h"
#include "WaveSample.h"
#include "GuardedRingBuffer.h"

#include <osss>

SC_MODULE( MixerModule )
{
  // Fast system clock
  sc_core::sc_in< bool > pi_clock;
  sc_core::sc_in< bool > pi_reset;

  // This clock is close to 44100 Hz
  sc_core::sc_in< bool >        pi_audio_clock;
  // The environment expects one sample
  // per rising audio clock edge
  sc_core::sc_out< WaveSample > po_audio_data;

  // true if there is demand to play music
  //sc_core::sc_in< bool > pi_music_play;

  // Buffers for plain wave data
  osss::osss_port_to_shared< GuardedRingBuffer< WaveSample, MUSIC_BUFFER_SIZE > > port_to_music_buffer;
  osss::osss_port_to_shared< GuardedRingBuffer< WaveSample, SPEECH_BUFFER_SIZE > > port_to_speech_buffer;

  sc_core::sc_in< bool > pi_music_buffer_empty;
  sc_core::sc_in< bool > pi_music_buffer_full;
  sc_core::sc_in< bool > pi_speech_buffer_empty;
  sc_core::sc_in< bool > pi_speech_buffer_full;

  void mixerThread();

  SC_CTOR( MixerModule ) : port_to_music_buffer("port_to_music_buffer"),
                           port_to_speech_buffer("port_to_speech_buffer")
  {
    SC_CTHREAD( mixerThread, pi_clock );
    reset_signal_is(pi_reset, true);
  }

};

#endif // MIXER_MODULE_H
