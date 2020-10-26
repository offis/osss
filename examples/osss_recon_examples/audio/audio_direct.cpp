#include <osss>

#ifndef SC_SYNTHESIS
#include <systemc.h> // bring SystemC symbols into scope
#include <iostream>
#endif

/* Very simple benchmark:
   This example demonstrates use of an osss_recon
   object without using any osss_context instances.
   All accesses are executed directly on the reconfigurable
   object, so attribute saving and restauration will never
   occur. This is no problem since we have just one
   single process working on the osss_recon object. */

/* ------------------------------------------------------ */

/* Here we define a memory class. It simply allows
   read() and write(). We provide two datatypes for
   better readability. */

#define MEMSIZE 1024
#define ADDR_WIDTH 10
#define DATA_WIDTH 16

typedef sc_dt::sc_uint<ADDR_WIDTH> Address;
typedef sc_dt::sc_uint<DATA_WIDTH> Dataword;

class Memory
{
  private:
  Dataword storage[MEMSIZE];

  public:
  Dataword read(Address addr)
  {
    return storage[addr];
  }

  void write(Address addr, Dataword value)
  {
    storage[addr] = value;
  }
};

/* ------------------------------------------------------ */

/* Now we define an hierarchy of audio codecs.
   They operate on two buffers. Note that we
   simply pass them by reference. This is not
   the way you would synthesise it but since
   it is not the focus for this example, we
   do not care. */

/* This is an base class for all codecs.
   Note, that we derived it from osss_object.
   This is required since we want to stuff it
   into an osss_recon container. But see below for this ... */
template<class MemClass>
class PlayerBase : public osss::osss_object
{
public:

  PlayerBase()
  {
    // This line is important for all classes
    // that are to be used within an polymorphic
    // container. Pass the base class name here.
    OSSS_BASE_CLASS( osss::osss_object );
  }

  // Our workhorse method. No operation here...
  virtual void decode(MemClass & /*compressed*/,
                      MemClass & /*decompressed*/)
    { OSSS_PURE_VIRTUAL_VOID(); }
};

// One player class which may be instantiated
template<class MemClass>
class MP3Player : public PlayerBase<MemClass>
{
  using osss::osss_object::wait;
public:

  MP3Player()
  {
    OSSS_BASE_CLASS(PlayerBase<MemClass>);
  }

  // We implement the method declared in our
  // base class
  virtual void decode(MemClass & compressed,
                      MemClass & decompressed)
  {
    // Very meaningful codec... we just do some
    // silly things for user information and
    // time consumption. The real code would go here.
    OSSS_MESSAGE(true, "MP3 decode");
    for (int addr = 0; addr < MEMSIZE; ++addr)
    {
      decompressed.write(addr, compressed.read(addr) + 1);
      // strange behaviour with g++ 4.1.1
      // using wait() without 'this->' results in a compilation error:
      // 'no match for call to '(osss::osss_object) ()'
      // with g++ 4.1.2 it works even without a 'this->' prefix
      this->wait();
    }
  }

};

template<class MemClass>
class OggVorbisPlayer : public PlayerBase<MemClass>
{
  using osss::osss_object::wait;
public:

  OggVorbisPlayer()
  {
    OSSS_BASE_CLASS(PlayerBase<MemClass>);
  }

  virtual void decode(MemClass & compressed,
                      MemClass & decompressed)
  {
    OSSS_MESSAGE(true, "OGG decode");
    for (int addr = 0; addr < MEMSIZE; ++addr)
    {
      decompressed.write(addr, compressed.read(addr) + 2);
      this->wait();
    }
  }
};

template<class MemClass>
class AACPlayer : public PlayerBase<MemClass>
{
  using osss::osss_object::wait;
public:

  AACPlayer()
  {
    OSSS_BASE_CLASS(PlayerBase<MemClass>);
  }

  virtual void decode(MemClass & compressed,
                      MemClass & decompressed)
  {
    OSSS_MESSAGE(true, "AAC decode");
    for (int addr = 0; addr < MEMSIZE; ++addr)
    {
      decompressed.write(addr, compressed.read(addr) + 2);
      this->wait();
    }
  }
};

/* ------------------------------------------------------ */

SC_MODULE(DUT)
{
  Memory input_buffer;
  Memory output_buffer;
  
  // We want one reconfigurable area in our design. It shall provide
  // the interface "PlayerBasey<Memory>".
  osss::osss_recon< PlayerBase<Memory> > decoder;

  // The one-and-only process in this simple benchmark
  void core()
  {
    // The OSSS_MESSAGE statements have no meaning for synthesis, please ignore
    OSSS_MESSAGE(true, "core process (re-)started");
    while (true)
    {
      // We just make use of three codecs one after another.
      // Just demonstrate the direct use of the osss_recon
      // object.

      // This assignment will cause a reconfiguration
      // if the previous object in the osss_recon container
      // was not an MP3Player. This is always the case.
      // This is a blocking call, returning after configuration
      // and constructor execution.
      OSSS_MESSAGE(true, "Switching to MP3");
      decoder = MP3Player<Memory>();

      // No other process in this design, so no-one can
      // interfer with us. Therefore we know, that the MP3 Codec
      // must be present. We make a procedure call (-> ..._PC).
      OSSS_MESSAGE(true, "Decoding MP3");
      decoder->decode(input_buffer, output_buffer);

      OSSS_MESSAGE(true, "Switching to OGG");
      decoder = OggVorbisPlayer<Memory>();
      OSSS_MESSAGE(true, "Decoding OGG");
      decoder->decode(input_buffer, output_buffer);

      OSSS_MESSAGE(true, "Switching to AAC");
      decoder = AACPlayer<Memory>();
      OSSS_MESSAGE(true, "Decoding AAC");
      decoder->decode(input_buffer, output_buffer);

      // Just for demonstration: We can re-use the contents of
      // the osss_recon container. This doesn't cause any reconfiguration.
      OSSS_MESSAGE(true, "Decoding AAC (2nd time)");
      decoder->decode(input_buffer, output_buffer);
    };
  }

  sc_in< bool > clock;
  sc_in< bool > reset;

  SC_CTOR(DUT) : decoder("decoder_module")
  {
    // Think of the osss_recon<> module member as a submodule.
    // It requires a clock and a reset signal. Attach it to
    // its "clock_port" and "reset_port" members of type
    // sc_in<bool>
    decoder.clock_port(clock);
    decoder.reset_port(reset);

    // We used one single thread in this example
    SC_CTHREAD(core, clock);
    reset_signal_is(reset, true);
    // We need to declare, that this thread uses the decoder.
    uses(decoder);
  }
};

/* ------------------------------------------------------ */

int sc_main(int /*argc*/, char * /*argv*/[])
{
  // First the well-known ordinary SystemC-stuff...
  sc_clock clock("main_clock", sc_time(10, SC_NS));
  sc_signal< bool > reset;
  DUT dut("design_under_test");
  dut.clock(clock);
  dut.reset(reset);

  // ... and here are the OSSS+R statements:

  // We declare that we have a special type of FPGA. This is
  // a type declaration (like "Xilinx Virtex II 1000-4").
  osss::osss_device_type my_device_type("My FPGA device type");

  // Specify, how the logic copy times (unused in this example,
  // so please ignore the 26us entries) and reconfiguration times
  // are, if a certain class (e.g. OggVorbisPlayer<Memory> is to
  // be configured to the FPGA.
  OSSS_DECLARE_TIME(my_device_type, PlayerBase<Memory>, sc_time(26, SC_US), sc_time(200, SC_US));
  OSSS_DECLARE_TIME(my_device_type, MP3Player<Memory>, sc_time(26, SC_US), sc_time(200, SC_US));
  OSSS_DECLARE_TIME(my_device_type, OggVorbisPlayer<Memory>, sc_time(26, SC_US), sc_time(200, SC_US));
  OSSS_DECLARE_TIME(my_device_type, AACPlayer<Memory>, sc_time(26, SC_US), sc_time(200, SC_US));

  // Instantiate the device. Now "my_device" is one specific chip instance on the PCB.
  osss::osss_device my_device(my_device_type, "my_device");

  // The rest is ordinary SystemC stuff again  
  my_device.clock_port(clock);
  my_device.reset_port(reset);
  dut.decoder( my_device );
  OSSS_MESSAGE(true, "Starting simulation");
  sc_start(sc_time(2, SC_MS));

  return EXIT_SUCCESS;
}
