#ifndef TESTBENCH_H
#define TESTBENCH_H

#include "WaveSample.h"
#include "EncodedSample.h"

#include <systemc>

// This module provides stimuli and receives
// output data. It is the system's environment.

SC_MODULE(Testbench)
{
  sc_core::sc_in< bool > pi_clock;
  sc_core::sc_in< bool > pi_reset;

  sc_core::sc_out< bool >           po_music_req;
  sc_core::sc_in< bool >            pi_music_ack;
  sc_core::sc_out< osss::osss_polymorphic< EncodedSample > >  po_music_enc_sample;

  sc_core::sc_out< sc_dt::sc_uint< 64 > >  po_keyword;
  sc_core::sc_in< WaveSample >      pi_audio_data;

  sc_core::sc_out< bool >           po_speech_req;
  sc_core::sc_in< bool >            pi_speech_ack;
  sc_core::sc_out< osss::osss_polymorphic< EncodedSample > >  po_speech_enc_sample;

  // provides some music
  void music_drv();

  // provides text to be spoken
  void speech_drv();

  // provides a user key (e.g. from keycard)
  void security_drv();

  SC_CTOR( Testbench )
  {
    SC_CTHREAD(music_drv, pi_clock);
    reset_signal_is(pi_reset, true);
    SC_CTHREAD(speech_drv, pi_clock);
    reset_signal_is(pi_reset, true);
    SC_CTHREAD(security_drv, pi_clock);
    reset_signal_is(pi_reset, true);
  }
};

#endif // TESTBENCH_H

