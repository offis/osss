#include <systemc.h>

#define OSSS_BLUE
#include <osss.h>

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#define MEMSIZE 3
// must cover MEMSIZE+1
#define ADDR_WIDTH 3
#define DATA_WIDTH 16

#include "audio3_config.h"

#define MP3_FACTOR 2
#define AAC_FACTOR 10

class Buffer
{
  private:
  Dataword storage[MEMSIZE];
  Address  fill;

  public:
  Buffer()
  {
    fill = 0;
  }

  Dataword read()
  {
    return storage[fill--];
  }

  void write( Dataword d )
  {
    storage[fill++] = d;
  }

  bool empty()
  {
    return (fill == 0);
  }

  bool full()
  {
    return (fill == MEMSIZE);
  }
};

template<class Buffer>
class AudioCodec : public osss::osss_object
{
public:
  Buffer compressed, decompressed;
  bool   mode_compress;

  AudioCodec()
  {
    OSSS_BASE_CLASS( osss::osss_object );
    mode_compress = false; // playback
  }

  virtual void code()
    { OSSS_PURE_VIRTUAL_VOID(); }

  void switchToCompressionMode()
  {
    mode_compress = true;
  }

  void switchToDecompressionMode()
  {
    mode_compress = false;
  }

  Dataword getOutput()
  {
    if (mode_compress)
    { return compressed.read(); }
    else
    { return decompressed.read(); }
  }

  void putInput(Dataword d)
  {
    if (mode_compress)
    { return decompressed.write(d); }
    else
    { return compressed.write(d); }
  }

  bool inputBufferEmpty()
  {
    if (mode_compress)
    { return decompressed.empty(); }
    else
    { return compressed.empty(); }
  }

  bool inputBufferFull()
  {
    if (mode_compress)
    { return decompressed.full(); }
    else
    { return compressed.full(); }
  }

  bool outputBufferEmpty()
  {
    if (mode_compress)
    { return compressed.empty(); }
    else
    { return decompressed.empty(); }
  }

  bool outputBufferFull()
  {
    if (mode_compress)
    { return compressed.empty(); }
    else
    { return decompressed.empty(); }
  }

  protected:
  Dataword getInput()
  {
    if (mode_compress)
    { return decompressed.read(); }
    else
    { return compressed.read(); }
  }

  void putOutput(Dataword d)
  {
    if (mode_compress)
    { return compressed.write(d); }
    else
    { return decompressed.write(d); }
  }

};

template<class Buffer>
class MP3Codec : public AudioCodec<Buffer>
{
public:

  MP3Codec()
  {
    OSSS_BASE_CLASS(AudioCodec< Buffer >);
  }

  virtual void code()
  {
    while (  ! AudioCodec<Buffer>::inputBufferEmpty()
          && ! AudioCodec<Buffer>::outputBufferFull())
    {
      putOutput( AudioCodec<Buffer>::getInput() * MP3_FACTOR ); // dummy decoding...
    }
  }
};

template<class Buffer>
class AACCodec : public AudioCodec<Buffer>
{
public:

  AACCodec()
  {
    OSSS_BASE_CLASS(AudioCodec< Buffer >);
  }

  virtual void code()
  {
    while (  ! AudioCodec<Buffer>::inputBufferEmpty()
          && ! AudioCodec<Buffer>::outputBufferFull())
    {
      putOutput( AudioCodec<Buffer>::getInput() * AAC_FACTOR ); // dummy decoding...
    }
  }
};


/* ------------------------------------------------------ */

SC_MODULE(xModule)
{
  osss::osss_recon< AudioCodec<Buffer> > codec;

  sc_in< int >       pi_codec_id;
  sc_in< Dataword >  pi_data;
  sc_in< bool >      pi_data_valid;
  sc_out< bool >     po_stall_input;

  sc_out< Dataword > po_data;
  sc_out< bool >     po_data_valid;

  int current_codec_id;

  void flush()
  {
    if (current_codec_id != NO_ID)
    {
      codec->code();
      while (! codec->outputBufferEmpty() )
      {
        po_data.write( codec->getOutput() );
        po_data_valid.write( true );
        wait();
        po_data_valid.write( false );
      }
    }
  }

  void xProc()
  {
    current_codec_id = NO_ID;
    po_stall_input.write( true );
    po_data_valid.write( false );
    while (true)
    {
      po_stall_input.write( true );

      bool codec_change = (pi_codec_id.read() != current_codec_id);
      bool no_input     = (pi_data_valid.read() == false);
      if (codec_change || no_input)
      {
        flush();

        // New codec
        if (codec_change && (pi_codec_id.read() != NO_ID))
        {
          switch( pi_codec_id.read() )
          { case MP3_ID : codec = MP3Codec<Buffer>(); break;
            case AAC_ID : codec = AACCodec<Buffer>(); break;
          }
        }
        wait();
        current_codec_id = pi_codec_id.read();
      }
      else // no codec change and valid input
      {
        if ( ! codec->inputBufferFull() )
        {
          po_stall_input.write( false );
          Dataword d = po_data.read();
          wait();
          po_stall_input.write( true );
          codec->putInput( d );
        }
        else
        {
          wait();
        }
      }
    } // while
  }

  sc_in< bool > clock;
  sc_in< bool > reset;

  SC_CTOR(xModule) : codec("xModule"), pi_codec_id("pi_codec_id"), pi_data("pi_data"),
                     pi_data_valid("pi_data_valid"), po_stall_input("po_stall_input"),
                     po_data("po_data"), po_data_valid("po_data_valid")
  {
    codec.clock_port(clock);
    codec.reset_port(reset);

    SC_CTHREAD(xProc, clock.pos());
    reset_signal_is(reset, true);
    uses(codec, 42);
  }
};


