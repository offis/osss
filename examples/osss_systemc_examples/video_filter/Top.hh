#ifndef TOP_INCLUDED
#define TOP_INCLUDED

#include <systemc.h>

#include "Video_Source.hh"

#include "filter.hh"
#include "Video_Sink.hh"

SC_MODULE(Top)
{

  sc_in<bool> clock;
  sc_in<bool> reset;

 protected:

  sc_signal<bool> Ho_444_sig;
  sc_signal<sc_biguint<11> > write_address_sig;

  sc_signal<sc_biguint<8> > red_in_sig;
  sc_signal<sc_biguint<8> > green_in_sig;
  sc_signal<sc_biguint<8> > blue_in_sig;

  sc_signal<sc_biguint<8> > red_out_sig;
  sc_signal<sc_biguint<8> > green_out_sig;
  sc_signal<sc_biguint<8> > blue_out_sig;

 public:

  SC_CTOR(Top) :
    clock("clock"),
    reset("reset"),
    m_video_source("m_video_source"),
    m_filter("m_filter"),
    m_video_sink("m_video_sink")
  {
    m_video_source.clock(clock);
    m_video_source.reset(reset);

    m_video_source.Ho_444(Ho_444_sig);
    m_video_source.write_address(write_address_sig);

    m_video_source.red_out(red_in_sig);
    m_video_source.green_out(green_in_sig);
    m_video_source.blue_out(blue_in_sig);

    m_filter.rst(reset);
    m_filter.Ho_444(Ho_444_sig);
    m_filter.write_clk(clock);
    m_filter.write_address(write_address_sig);
  
    m_filter.red_in(red_in_sig);
    m_filter.green_in(green_in_sig);
    m_filter.blue_in(blue_in_sig);
		
    m_filter.red_out(red_out_sig);
    m_filter.green_out(green_out_sig);
    m_filter.blue_out(blue_out_sig);

    m_video_sink.Ho_444(Ho_444_sig);
    m_video_sink.rst(reset);
    m_video_sink.write_clk(clock);

    m_video_sink.red_in(red_out_sig);
    m_video_sink.green_in(green_out_sig);
    m_video_sink.blue_in(blue_out_sig);
  }

 protected:

  Video_Source m_video_source;
 public: // for tracing
  filter       m_filter;
 protected:
  Video_Sink   m_video_sink;

};

#endif
