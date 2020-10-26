#include <osss>
#include <systemc.h> // bring SystemC symbols into scope
#include <iostream>

/* We assume that you inspected the first audio benchmark
   before looking into this file.

   We make a "slight" modification here: We want to share
   the use of the reconfigurable area.

   First, this introduces problems, since the calls like
   ...
   decoder = OggVorbisPlayer<Memory>();
   decoder->decode(input_buffer, output_buffer);
   ...
   are not atomic. There are several ways to deal with this.
   One way would be to make some explicit handshaking between
   the user processes to avoid mutual interference. We choose
   to use library features. We enable OSSS+R to detect that
   there was interference and "repair" the situation.

   We introduce contexts. Contexts allow you to treat them
   as "virtual recon objects" if you like. To the designer,
   they seem to exist in parallel. In reality they are switched
   automatically as needed. Here we go.
 */

/* ------------------------------------------------------ */

/* This is the memory class you know from the first audio
   example. No changes. */

#define MEMSIZE 1024
#define ADDR_WIDTH 10
#define DATA_WIDTH 16

#if defined( FORCE_THREAD_PROCESSES )

#undef SC_CTHREAD
#define SC_CTHREAD( proc, clock ) \
  SC_THREAD( proc );

#define reset_signal_is( signal, level )

#endif

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

/* Player classes. Same as before, no changes. */

template<class MemClass>
class PlayerBase : public osss::osss_object
{
public:
  // This line is important for all classes
  // that are to be used within an polymorphic
  // container. This is to be replaced by...

  PlayerBase()
  {
    // ...this in future: Simply pass the base class name here.
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
  // The process one uses this one only
  osss::osss_context< PlayerBase< Memory > > view_of_process_one;
  // And this is for process two
  osss::osss_context< PlayerBase< Memory > > view_of_process_two;

  void process_one()
  {
    OSSS_MESSAGE(true, "process one (re-)started");
    while (true)
    {
      // This process uses MP3 and AAC
      OSSS_MESSAGE(true, "Switching to MP3");
      view_of_process_one = MP3Player<Memory>();

      OSSS_MESSAGE(true, "Decoding MP3");
      view_of_process_one->decode(input_buffer, output_buffer);

      OSSS_MESSAGE(true, "Switching to AAC");
      view_of_process_one = AACPlayer<Memory>();

      OSSS_MESSAGE(true, "Decoding AAC");
      view_of_process_one->decode(input_buffer, output_buffer);
    };
  }

  void process_two()
  {
    OSSS_MESSAGE(true, "process two (re-)started");
    while (true)
    {
      // This process uses MP3 and OGG
      OSSS_MESSAGE(true, "Switching to MP3");
      view_of_process_two = MP3Player<Memory>();

      OSSS_MESSAGE(true, "Decoding MP3");
      view_of_process_two->decode(input_buffer, output_buffer);

      OSSS_MESSAGE(true, "Switching to OGG");
      view_of_process_two = OggVorbisPlayer<Memory>();

      OSSS_MESSAGE(true, "Decoding OGG");
      view_of_process_two->decode(input_buffer, output_buffer);
    };
  }


  sc_in< bool > clock;
  sc_in< bool > reset;

  SC_CTOR(DUT) : decoder("decoder_module")
  {
    decoder.clock_port(clock);
    decoder.reset_port(reset);

    SC_CTHREAD(process_one, clock);
    reset_signal_is(reset, true);
    // We could still write "uses(decoder)". But this
    // is more readable to the user.
    uses(view_of_process_one);

    SC_CTHREAD(process_two, clock);
    reset_signal_is(reset, true);
    uses(view_of_process_two);

    // This is new: Since we use contexts as "virtual
    // reconfigurable areas" (so to say), we need to
    // tell the library where they are mapped to.
    // Since we got only one physical reconfigurable
    // area, we quickly decide it to be this way:
    view_of_process_one(decoder);
    view_of_process_two(decoder);
    // ...and now they are both mapped to "decoder"
  }
};

/* ------------------------------------------------------ */

int sc_main(int /*argc*/, char * /*argv*/[])
{
  sc_clock clock("main_clock", sc_time(10, SC_NS));
  sc_signal< bool > reset;
  DUT dut("design_under_test");
  dut.clock(clock);
  dut.reset(reset);

  osss::osss_device_type my_device_type("My FPGA device type");

  // Here we changed nothing. But in this example we use
  // the settings for the copy time (20us to 24us).
  OSSS_DECLARE_TIME(my_device_type, PlayerBase<Memory>, sc_time(20, SC_US), sc_time(200, SC_US));
  OSSS_DECLARE_TIME(my_device_type, MP3Player<Memory>, sc_time(23, SC_US), sc_time(200, SC_US));
  OSSS_DECLARE_TIME(my_device_type, OggVorbisPlayer<Memory>, sc_time(22, SC_US), sc_time(200, SC_US));
  OSSS_DECLARE_TIME(my_device_type, AACPlayer<Memory>, sc_time(24, SC_US), sc_time(200, SC_US));

  osss::osss_device my_device(my_device_type, "my_device");

  my_device.clock_port(clock);
  my_device.reset_port(reset);
  dut.decoder( my_device );

  osss_event_log.open("log.xml");

  OSSS_MESSAGE(true, "Starting simulation");
  sc_start(sc_time(2, SC_MS));

  osss_event_log.close();

  return EXIT_SUCCESS;
}

// see explanation.txt for further information
