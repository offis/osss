#ifndef PLAYER_MODULE_H
#define PLAYER_MODULE_H

#include "Defines.h"
#include "GuardedRingBuffer.h"
#include "Codec.h"

#include <osss>

template< unsigned int OUTPUT_BUFFER_SIZE >
SC_MODULE( PlayerModule )
{
  // Fast system clock
  sc_core::sc_in< bool > pi_clock;
  sc_core::sc_in< bool > pi_reset;

  // Here we fetch music to be decoded.
  // It may be encoded my different codecs which we don't know.
  sc_core::sc_in< osss::osss_polymorphic< EncodedSample > > pi_enc_sample;
  // For handshaking with the music producer. This time
  // we are having requests and the music storage answers
  // with ack.
  sc_core::sc_in< bool >  pi_req;
  sc_core::sc_out< bool > po_ack;

  // We see "true" if keyword is ok: system unlocked
  sc_core::sc_in< bool > pi_unlocked;

  // One of three possible objects (contexts) to live in "recon"
  osss::osss_context< Codec > codec_context;

  // Buffer for plain wave data
  osss::osss_port_to_shared< GuardedRingBuffer< WaveSample, OUTPUT_BUFFER_SIZE > > port_to_buffer;

  // compressed data
  RingBuffer< osss::osss_polymorphic< EncodedSample >, ENCODED_BLOCK_SIZE > eb;

  // internal helper methods
  void fetchEncodedData();

  void playbackThread();

  SC_CTOR( PlayerModule ) : port_to_buffer("port_to_buffer")
  {
    SC_CTHREAD( playbackThread, pi_clock );
    set_stack_size( 1 << 20 );
    reset_signal_is( pi_reset, true );
    uses(codec_context);
  }

};

#include "PlayerModule.icc"

#endif // PLAYER_MODULE_H
