#ifndef FILTER_INCLUDED
#define FILTER_INCLUDED

#include <systemc.h>
#include "chunk_buffer.hh"

#ifdef LOCAL_BUILD_FOR_FOSSY_TESTCASE
  #ifndef FILTER_INNER_CC_FILE
  #define FILTER_INNER_CC_FILE "filter_inner.cc"
  #endif
  #include FILTER_INNER_CC_FILE
#else
  #include "filter_inner.hh"
#endif

SC_MODULE(filter)
{

  //
  // port section
  //
  sc_in< bool >           rst;
  sc_in< bool >           Ho_444;
  sc_in< bool >           write_clk;
  sc_in< sc_biguint<11> > write_address;
  
  sc_in< sc_biguint<8> >  red_in;
  sc_in< sc_biguint<8> >  green_in;
  sc_in< sc_biguint<8> >  blue_in;

  sc_out< sc_biguint<8> > red_out;
  sc_out< sc_biguint<8> > green_out;
  sc_out< sc_biguint<8> > blue_out;

// protected:

  //
  // signals
  //
  sc_signal< sc_biguint<8> > read_red_data_line0;
  sc_signal< sc_biguint<8> > read_red_data_line1;
  sc_signal< sc_biguint<8> > read_red_data_line2;
  sc_signal< sc_biguint<8> > read_green_data_line0;
  sc_signal< sc_biguint<8> > read_green_data_line1;
  sc_signal< sc_biguint<8> > read_green_data_line2;
  sc_signal< sc_biguint<8> > read_blue_data_line0;
  sc_signal< sc_biguint<8> > read_blue_data_line1;
  sc_signal< sc_biguint<8> > read_blue_data_line2;
  sc_signal< bool > read_enable;

public:

  SC_HAS_PROCESS(filter);

  filter(sc_module_name name) :
    sc_module(name),
    rst("rst"),
    Ho_444("Ho_444"),
    write_clk("write_clk"),
    write_address("write_address"),
    red_in("red_in"),
    green_in("green_in"),
    blue_in("blue_in"),
    red_out("red_out"),
    green_out("green_out"),
    blue_out("blue_out"),
    m_chunk_buffer("m_chunk_buffer"),
    m_filter_inner("m_filter_inner")
  {
 
    //
    // m_chunk_buffer port bindings
    //
    m_chunk_buffer.rst(rst);
    m_chunk_buffer.Ho_444(Ho_444);
    m_chunk_buffer.write_clk(write_clk);
    m_chunk_buffer.write_address(write_address);
    m_chunk_buffer.write_red_data(red_in);
    m_chunk_buffer.write_green_data(green_in);
    m_chunk_buffer.write_blue_data(blue_in);
	
    m_chunk_buffer.read_clk(write_clk);          
    m_chunk_buffer.read_address(write_address);
    m_chunk_buffer.read_enable(read_enable); //1
    m_chunk_buffer.read_red_data_line0(read_red_data_line0);
    m_chunk_buffer.read_red_data_line1(read_red_data_line1);
    m_chunk_buffer.read_red_data_line2(read_red_data_line2);
    m_chunk_buffer.read_green_data_line0(read_green_data_line0);
    m_chunk_buffer.read_green_data_line1(read_green_data_line1);
    m_chunk_buffer.read_green_data_line2(read_green_data_line2);
    m_chunk_buffer.read_blue_data_line0(read_blue_data_line0);
    m_chunk_buffer.read_blue_data_line1(read_blue_data_line1);
    m_chunk_buffer.read_blue_data_line2(read_blue_data_line2);
    
    m_filter_inner.rst(rst);
    m_filter_inner.Ho_444(Ho_444);
    m_filter_inner.write_clk(write_clk);
    m_filter_inner.red_in(red_in);
    m_filter_inner.green_in(green_in);
    m_filter_inner.blue_in(blue_in);
    m_filter_inner.red_out(red_out);
    m_filter_inner.green_out(green_out);
    m_filter_inner.blue_out(blue_out);
    m_filter_inner.read_enable(read_enable);
    m_filter_inner.read_red_data_line0(read_red_data_line0);
    m_filter_inner.read_red_data_line1(read_red_data_line1);
    m_filter_inner.read_red_data_line2(read_red_data_line2);
    m_filter_inner.read_green_data_line0(read_green_data_line0);
    m_filter_inner.read_green_data_line1(read_green_data_line1);
    m_filter_inner.read_green_data_line2(read_green_data_line2);
    m_filter_inner.read_blue_data_line0(read_blue_data_line0);
    m_filter_inner.read_blue_data_line1(read_blue_data_line1);
    m_filter_inner.read_blue_data_line2(read_blue_data_line2);
    
  }

protected:

  chunk_buffer m_chunk_buffer;
  filter_inner m_filter_inner;

};

#endif
