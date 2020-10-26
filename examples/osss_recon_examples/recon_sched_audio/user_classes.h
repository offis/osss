#include <osss>
#include <systemc.h> // bring SystemC symbols into scope
#include <iostream>

/* We assume that you inspected the first audio and
   and "two process audio" benchmarks before looking into this file.
   
   We extend the "two process audio" benchmark and show one way
   how thrashing can be avoided.
 */

/* ------------------------------------------------------ */

/* No changes to this section. */

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

/* No changes to this section. */

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

