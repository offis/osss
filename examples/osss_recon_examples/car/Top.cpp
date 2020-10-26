
#include "Defines.h"

#define CAR_RECONFIGURATION_TIMES_FPL06 false

#include "PlayerModule.h"
#include "SecurityModule.h"
#include "Testbench.h"
#include "MixerModule.h"
#include "Vorbis.h"
#include "AAC.h"
#include "MP3.h"

// Include the simulation libraries
#include <osss.h>

SC_MODULE( TopLevelModule )
{
  sc_in< bool > pi_clock;
  sc_in< bool > pi_audio_clock;

  sc_in< bool > pi_reset;

  sc_signal< WaveSample > s_mixed_audio_signal;
  sc_signal< sc_uint< 64 > > s_keyword;

  sc_signal< osss_polymorphic< EncodedSample > > s_music_enc_sample;
  sc_signal< bool > s_music_ack;
  sc_signal< bool > s_music_req;

  sc_signal< osss_polymorphic< EncodedSample > > s_speech_enc_sample;
  sc_signal< bool > s_speech_ack;
  sc_signal< bool > s_speech_req;

  sc_signal< bool > s_unlocked;

  Testbench * tb;
  SecurityModule  * secm;
  // Music player and navigation speech module share the
  // same implementation
  PlayerModule<MUSIC_BUFFER_SIZE> * playm;
  PlayerModule<SPEECH_BUFFER_SIZE> * navm;
  MixerModule * mixm;

  // This is the recon object which stands for one
  // configurable part of the circuit
  osss_recon< osss_object > recon;
  osss::osss_socket< osss::osss_shared< GuardedRingBuffer< WaveSample, MUSIC_BUFFER_SIZE > > > music_buffer;
  osss::osss_socket< osss::osss_shared< GuardedRingBuffer< WaveSample, SPEECH_BUFFER_SIZE > > > speech_buffer;

  sc_signal< bool > s_music_buffer_empty;
  sc_signal< bool > s_music_buffer_full;
  sc_signal< bool > s_speech_buffer_empty;
  sc_signal< bool > s_speech_buffer_full;


  SC_CTOR( TopLevelModule ) : recon("TopLevelModule")
  {
    // Normal SystemC module instantiation, nothing special here...
    playm = new PlayerModule<MUSIC_BUFFER_SIZE>("player_module");
    playm->pi_clock(pi_clock);
    playm->pi_reset(pi_reset);
    playm->pi_enc_sample(s_music_enc_sample);
    playm->po_ack(s_music_ack);
    playm->pi_req(s_music_req);
    playm->pi_unlocked(s_unlocked);
    playm->codec_context(recon);

    mixm = new MixerModule("mixer_module");
    mixm->pi_audio_clock(pi_audio_clock);
    mixm->po_audio_data(s_mixed_audio_signal);
    mixm->pi_clock(pi_clock);
    mixm->pi_reset(pi_reset);
    mixm->pi_music_buffer_empty(s_music_buffer_empty);
    mixm->pi_music_buffer_full(s_music_buffer_full);
    mixm->pi_speech_buffer_empty(s_speech_buffer_empty);
    mixm->pi_speech_buffer_full(s_speech_buffer_full);


    navm = new PlayerModule<SPEECH_BUFFER_SIZE>("navigation_module");
    navm->pi_clock(pi_clock);
    navm->pi_reset(pi_reset);
    navm->pi_enc_sample(s_speech_enc_sample);
    navm->po_ack(s_speech_ack);
    navm->pi_req(s_speech_req);
    navm->pi_unlocked(s_unlocked);
    // bind context to recon object
    navm->codec_context(recon);

    secm = new SecurityModule("security_module");
    secm->pi_keyword(s_keyword);
    secm->po_unlocked(s_unlocked);
    secm->pi_clock(pi_clock);
    secm->pi_reset(pi_reset);
    // bind context to recon object
    secm->crypto_context(recon);

    tb  = new Testbench("testbench_module");
    tb->pi_clock(pi_clock);
    tb->pi_audio_data(s_mixed_audio_signal);
    tb->po_keyword(s_keyword);
    tb->pi_reset(pi_reset);
    tb->po_music_enc_sample(s_music_enc_sample);
    tb->pi_music_ack(s_music_ack);
    tb->po_music_req(s_music_req);
    tb->po_speech_enc_sample(s_speech_enc_sample);
    tb->pi_speech_ack(s_speech_ack);
    tb->po_speech_req(s_speech_req);

    // connect shared objects living in sockets to
    // their outside ports
    navm->port_to_buffer(speech_buffer.plugged_object());
    playm->port_to_buffer(music_buffer.plugged_object());
    mixm->port_to_music_buffer(music_buffer.plugged_object());
    mixm->port_to_speech_buffer(speech_buffer.plugged_object());
    // Connect clock and reset to the shared objects
    music_buffer.plugged_object().reset_port(pi_reset);
    music_buffer.plugged_object().clock_port(pi_clock);
    speech_buffer.plugged_object().reset_port(pi_reset);
    speech_buffer.plugged_object().clock_port(pi_clock);
    // Connect clock and reset to the reconfigurable object
    recon.reset_port(pi_reset);
    recon.clock_port(pi_clock);

    // init
    s_music_buffer_empty.write(true);
    s_speech_buffer_empty.write(true);

    // make the buffers broadcast some info on their status
    music_buffer.get_interface().po_empty(  s_music_buffer_empty );
    music_buffer.get_interface().po_full(   s_music_buffer_full );
    speech_buffer.get_interface().po_empty( s_speech_buffer_empty );
    speech_buffer.get_interface().po_full(  s_speech_buffer_full );

  }

#ifndef SYNTHESIS
  ~TopLevelModule()
  {
    delete playm;
    delete tb;
    delete secm;
    delete navm;
    delete mixm;
  }
#endif
};

int sc_main(int , char * [])
{
  sc_clock clock("clock", sc_time(39736, SC_PS));
  sc_clock audio_clock("audio_clock", sc_time(22675, SC_NS));

  sc_signal<bool> reset;

  osss_device_type my_type("Virtex II");
  osss_device my_dev(my_type, "my_dev");

  my_dev.clock_port(clock);
  my_dev.reset_port(reset);

  TopLevelModule tlm("TopLevelModule");
  tlm.pi_clock(clock);
  tlm.pi_audio_clock(audio_clock);
  tlm.pi_reset(reset);

  // Put that reconfigurable object onto a device
  tlm.recon(my_dev);

#if CAR_RECONFIGURATION_TIMES_FPL06
  // Say, how costly the context switch for a class
  // on a device type is: attribute save/restore and configuration time
  /* real times, for FPL'06 paper */
  OSSS_DECLARE_TIME(my_type, Crypto,      sc_time(20, SC_US), sc_time(500, SC_MS));
  OSSS_DECLARE_TIME(my_type, VorbisCodec, sc_time(3, SC_US),  sc_time(1100, SC_MS));
  OSSS_DECLARE_TIME(my_type, AACCodec,    sc_time(4, SC_US),  sc_time(1200, SC_MS));
  OSSS_DECLARE_TIME(my_type, MP3Codec,    sc_time(2, SC_US),  sc_time(1, SC_SEC));
  OSSS_DECLARE_TIME(my_type, Codec,       sc_time(0, SC_US), sc_time(0, SC_SEC));
#else

  // smaller times for debugging
  OSSS_DECLARE_TIME(my_type, Crypto,      sc_time(20, SC_US), sc_time(500, SC_NS));
  OSSS_DECLARE_TIME(my_type, VorbisCodec, sc_time(3, SC_US),  sc_time(1100, SC_NS));
  OSSS_DECLARE_TIME(my_type, AACCodec,    sc_time(4, SC_US),  sc_time(1200, SC_NS));
  OSSS_DECLARE_TIME(my_type, MP3Codec,    sc_time(2, SC_US),  sc_time(1000, SC_NS));
  OSSS_DECLARE_TIME(my_type, Codec,       sc_time(0, SC_US), sc_time(0, SC_SEC));
#endif
  // This would open the log-file
  // osss_event_log.open("car.xml");

#ifndef SYNTHESIS
  std::cout << "End of elab phase, simulation starts" << std::endl;
#endif
  reset.write(false);
  sc_start(sc_time(10,SC_NS));

#ifndef SYNTHESIS
  std::cout << "reset rise" << std::endl;
#endif
  reset.write(true);
  sc_start(sc_time(100,SC_NS));

#ifndef SYNTHESIS
  std::cout << "reset fall" << std::endl;
#endif
  reset.write(false);
  sc_start(sc_time(8, SC_SEC));
  exit(EXIT_SUCCESS);
}
