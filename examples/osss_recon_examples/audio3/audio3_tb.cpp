#include "audio3_config.h"

//#define USE_SYNTHESISED_DUT 1

#if USE_SYNTHESISED_DUT
#include "synthesised_xModule.cpp"
#else
#include "audio3_dut.cpp"
#endif

#define OSSS_BLUE
#include <osss.h>

int sc_main(int /*argc*/, char * /*argv*/[])
{
  // First the well-known ordinary SystemC-stuff...
#ifndef SC_SYNTHESIS
  sc_clock clock("main_clock", sc_time(10, SC_NS));
  sc_signal< bool > reset;
#endif
  xModule dut("design_under_test");
#ifndef SC_SYNTHESIS
  dut.clock(clock);
  dut.reset(reset);
#endif

  sc_signal< int >      so_codec_id;     dut.pi_codec_id( so_codec_id );
  sc_signal< Dataword > so_data;         dut.pi_data( so_data );
  sc_signal< bool >     so_data_valid;   dut.pi_data_valid( so_data_valid );
  sc_signal< bool >     si_stall_input;  dut.po_stall_input( si_stall_input );
  sc_signal< Dataword > si_data;         dut.po_data( si_data );
  sc_signal< bool >     si_data_valid;   dut.po_data_valid( si_data_valid );

  // ... and here are the OSSS+R statements:

  // We declare that we have a special type of FPGA. This is
  // a type declaration (like "Xilinx Virtex II 1000-4").
  osss::osss_device_type my_device_type("My FPGA device type");

  // Specify, how the logic copy times (unused in this example,
  // so please ignore the 26us entries) and reconfiguration times
  // are, if a certain class (e.g. OggVorbisPlayer<Memory> is to
  // be configured to the FPGA.
  OSSS_DECLARE_TIME(my_device_type, AudioCodec<Buffer>, sc_time(26, SC_US), sc_time(200, SC_US));
  OSSS_DECLARE_TIME(my_device_type, MP3Codec<Buffer>, sc_time(26, SC_US), sc_time(200, SC_US));
  OSSS_DECLARE_TIME(my_device_type, AACCodec<Buffer>, sc_time(26, SC_US), sc_time(200, SC_US));
  osss::osss_device my_device(my_device_type, "my_device");

  // The rest is ordinary SystemC stuff again
#ifndef SC_SYNTHESIS
  my_device.clock_port(clock);
  my_device.reset_port(reset);
#endif
  dut.codec( my_device );
  OSSS_MESSAGE(true, "Starting simulation");
#ifndef SC_SYNTHESIS
  sc_start(sc_time(2, SC_MS));
#endif
  OSSS_MESSAGE(true, "Stopped simulation");
  return EXIT_SUCCESS;
}
