#ifndef CHUNK_BUFFER_INCLUDED
#define CHUNK_BUFFER_INCLUDED

#include <systemc.h>

#ifndef SC_SYNTHESIS
#include "ramb16_sn_sm.hh"
#endif

class chunk_buffer : public sc_module
{
public:
  
  //
  // port section
  //
  sc_in<bool>          rst;
  sc_in<bool>          Ho_444;

  sc_in<bool>          write_clk;
  sc_in< sc_biguint<11> > write_address;
  sc_in< sc_biguint<8> >  write_red_data;
  sc_in< sc_biguint<8> >  write_green_data;
  sc_in< sc_biguint<8> >  write_blue_data;

  sc_in<bool>          read_clk;
  sc_in< sc_biguint<11> > read_address;
  sc_in<bool>          read_enable;
  sc_out< sc_biguint<8> > read_red_data_line0;
  sc_out< sc_biguint<8> > read_red_data_line1;
  sc_out< sc_biguint<8> > read_red_data_line2;
  sc_out< sc_biguint<8> > read_green_data_line0;
  sc_out< sc_biguint<8> > read_green_data_line1;
  sc_out< sc_biguint<8> > read_green_data_line2;
  sc_out< sc_biguint<8> > read_blue_data_line0;
  sc_out< sc_biguint<8> > read_blue_data_line1;
  sc_out< sc_biguint<8> > read_blue_data_line2;

#ifndef SC_SYNTHESIS  
  
protected:

  //
  // signal section
  //
  sc_signal<bool>        write_enable_line0;
  sc_signal<bool>        write_enable_line1;
  sc_signal<bool>        write_enable_line2;

  sc_signal<sc_biguint<8> > write_red_data_line0;
  sc_signal<sc_biguint<8> > write_red_data_line1;
  sc_signal<sc_biguint<8> > write_red_data_line2;
  sc_signal<sc_biguint<8> > write_green_data_line0;
  sc_signal<sc_biguint<8> > write_green_data_line1;
  sc_signal<sc_biguint<8> > write_green_data_line2;
  sc_signal<sc_biguint<8> > write_blue_data_line0;
  sc_signal<sc_biguint<8> > write_blue_data_line1;
  sc_signal<sc_biguint<8> > write_blue_data_line2;

  sc_signal<sc_uint<2> > line_sel_counter;

  // constant or unused bram signals
  sc_signal<bool> gsr_sig; //()

  sc_signal<bool> ssr_a_sig; //0
  sc_signal<bool> ssr_b_sig; //0
  sc_signal<bool> we_a_sig; //1
  sc_signal<bool> we_b_sig; //0
  
  sc_signal<sc_biguint<1> > dpi_a_sig; //0

  sc_signal<sc_biguint<8> > 
    d_a_red0_sig, d_a_red1_sig, d_a_red2_sig, 
    d_a_green0_sig, d_a_green1_sig, d_a_green2_sig, 
    d_a_blue0_sig, d_a_blue1_sig, d_a_blue2_sig;
  // d_a_sig; //()
  
  sc_signal<sc_biguint<1> > 
    dp_a_red0_sig, dp_a_red1_sig, dp_a_red2_sig, 
    dp_a_green0_sig, dp_a_green1_sig, dp_a_green2_sig, 
    dp_a_blue0_sig, dp_a_blue1_sig, dp_a_blue2_sig;
//    dp_a_sig; //()

  sc_signal<sc_biguint<8> > di_b_sig; //0
  sc_signal<sc_biguint<1> > dpi_b_sig; //0
   
  sc_signal<sc_biguint<1> > 
    dp_b_red0_sig, dp_b_red1_sig, dp_b_red2_sig,
    dp_b_green0_sig, dp_b_green1_sig, dp_b_green2_sig,
    dp_b_blue0_sig, dp_b_blue1_sig, dp_b_blue2_sig
    ; //()

public:

  SC_HAS_PROCESS(chunk_buffer);

  chunk_buffer(sc_module_name name) :
    sc_module(name),
    rst("rst"),
    Ho_444("Ho_444"),
    write_clk("write_clk"),
    write_address("write_address"),
    write_red_data("write_red_data"),
    write_green_data("write_green_data"),
    write_blue_data("write_blue_data"),
    read_clk("read_clk"),
    read_address("read_address"),
    read_enable("read_enable"),
    read_red_data_line0("read_red_data_line0"),
    read_red_data_line1("read_red_data_line1"),
    read_red_data_line2("read_red_data_line2"),
    read_green_data_line0("read_green_data_line0"),
    read_green_data_line1("read_green_data_line1"),
    read_green_data_line2("read_green_data_line2"),
    read_blue_data_line0("read_blue_data_line0"),
    read_blue_data_line1("read_blue_data_line1"),
    read_blue_data_line2("read_blue_data_line2"),
    m_red_data_ram_line0("m_red_data_ram_line0"),
    m_red_data_ram_line1("m_red_data_ram_line1"),
    m_red_data_ram_line2("m_red_data_ram_line2"),
    m_green_data_ram_line0("m_green_data_ram_line0"),
    m_green_data_ram_line1("m_green_data_ram_line1"),
    m_green_data_ram_line2("m_green_data_ram_line2"),
    m_blue_data_ram_line0("m_blue_data_ram_line0"),
    m_blue_data_ram_line1("m_blue_data_ram_line1"),
    m_blue_data_ram_line2("m_blue_data_ram_line2")
  {

    /**********************************
     *  BRAM port bindings
     **********************************/

    //
    // red BRAM array (consists out of three lines)
    //
    // line 0

    // global set/reset port
    m_red_data_ram_line0.gsr(gsr_sig);
    // port a
    m_red_data_ram_line0.clk_a(write_clk);
    m_red_data_ram_line0.en_a(write_enable_line0);
    m_red_data_ram_line0.ssr_a(ssr_a_sig);
    m_red_data_ram_line0.we_a(we_a_sig);
    m_red_data_ram_line0.addr_a(write_address);
    m_red_data_ram_line0.di_a(write_red_data_line0); 
    m_red_data_ram_line0.dpi_a(dpi_a_sig); 
    m_red_data_ram_line0.d_a(d_a_red0_sig); 
    m_red_data_ram_line0.dp_a(dp_a_red0_sig); 
    // configuration port a
    m_red_data_ram_line0.WMODE_A = WRITE_FIRST;
    m_red_data_ram_line0.INIT_A = 0;
    m_red_data_ram_line0.SRVAL_A = 0;
    // port b
    m_red_data_ram_line0.clk_b(read_clk);
    m_red_data_ram_line0.en_b(read_enable);
    m_red_data_ram_line0.ssr_b(ssr_b_sig);
    m_red_data_ram_line0.we_b(we_b_sig);
    m_red_data_ram_line0.addr_b(read_address);
    m_red_data_ram_line0.di_b(di_b_sig); 
    m_red_data_ram_line0.dpi_b(dpi_b_sig); 
    m_red_data_ram_line0.d_b(read_red_data_line0); 
    m_red_data_ram_line0.dp_b(dp_b_red0_sig); 
    // configuration port b
    m_red_data_ram_line0.WMODE_B = WRITE_FIRST;
    m_red_data_ram_line0.INIT_B = 0;
    m_red_data_ram_line0.SRVAL_B = 0;
      
    // line 1

    // global set/reset port
    m_red_data_ram_line1.gsr(gsr_sig);
    // port a
    m_red_data_ram_line1.clk_a(write_clk);
    m_red_data_ram_line1.en_a(write_enable_line1);
    m_red_data_ram_line1.ssr_a(ssr_a_sig);
    m_red_data_ram_line1.we_a(we_a_sig);
    m_red_data_ram_line1.addr_a(write_address);
    m_red_data_ram_line1.di_a(write_red_data_line1); 
    m_red_data_ram_line1.dpi_a(dpi_a_sig); 
    m_red_data_ram_line1.d_a(d_a_red1_sig); 
    m_red_data_ram_line1.dp_a(dp_a_red1_sig); 
    // configuration port a
    m_red_data_ram_line1.WMODE_A = WRITE_FIRST;
    m_red_data_ram_line1.INIT_A = 0;
    m_red_data_ram_line1.SRVAL_A = 0;
    // port b
    m_red_data_ram_line1.clk_b(read_clk);
    m_red_data_ram_line1.en_b(read_enable);
    m_red_data_ram_line1.ssr_b(ssr_b_sig);
    m_red_data_ram_line1.we_b(we_b_sig);
    m_red_data_ram_line1.addr_b(read_address);
    m_red_data_ram_line1.di_b(di_b_sig); 
    m_red_data_ram_line1.dpi_b(dpi_b_sig); 
    m_red_data_ram_line1.d_b(read_red_data_line1); 
    m_red_data_ram_line1.dp_b(dp_b_red1_sig); 
    // configuration port b
    m_red_data_ram_line1.WMODE_B = WRITE_FIRST;
    m_red_data_ram_line1.INIT_B = 0;
    m_red_data_ram_line1.SRVAL_B = 0;

    // line 2

    // global set/reset port
    m_red_data_ram_line2.gsr(gsr_sig);
    // port a
    m_red_data_ram_line2.clk_a(write_clk);
    m_red_data_ram_line2.en_a(write_enable_line2);
    m_red_data_ram_line2.ssr_a(ssr_a_sig);
    m_red_data_ram_line2.we_a(we_a_sig);
    m_red_data_ram_line2.addr_a(write_address);
    m_red_data_ram_line2.di_a(write_red_data_line2); 
    m_red_data_ram_line2.dpi_a(dpi_a_sig); 
    m_red_data_ram_line2.d_a(d_a_red2_sig); 
    m_red_data_ram_line2.dp_a(dp_a_red2_sig); 
    // configuration port a
    m_red_data_ram_line2.WMODE_A = WRITE_FIRST;
    m_red_data_ram_line2.INIT_A = 0;
    m_red_data_ram_line2.SRVAL_A = 0;
    // port b
    m_red_data_ram_line2.clk_b(read_clk);
    m_red_data_ram_line2.en_b(read_enable);
    m_red_data_ram_line2.ssr_b(ssr_b_sig);
    m_red_data_ram_line2.we_b(we_b_sig);
    m_red_data_ram_line2.addr_b(read_address);
    m_red_data_ram_line2.di_b(di_b_sig); 
    m_red_data_ram_line2.dpi_b(dpi_b_sig); 
    m_red_data_ram_line2.d_b(read_red_data_line2); 
    m_red_data_ram_line2.dp_b(dp_b_red2_sig); 
    // configuration port b
    m_red_data_ram_line2.WMODE_B = WRITE_FIRST;
    m_red_data_ram_line2.INIT_B = 0;
    m_red_data_ram_line2.SRVAL_B = 0;

    //
    // green BRAM array (consists out of three lines)
    //
    // line 0

    // global set/reset port
    m_green_data_ram_line0.gsr(gsr_sig);
    // port a
    m_green_data_ram_line0.clk_a(write_clk);
    m_green_data_ram_line0.en_a(write_enable_line0);
    m_green_data_ram_line0.ssr_a(ssr_a_sig);
    m_green_data_ram_line0.we_a(we_a_sig);
    m_green_data_ram_line0.addr_a(write_address);
    m_green_data_ram_line0.di_a(write_green_data_line0); 
    m_green_data_ram_line0.dpi_a(dpi_a_sig); 
    m_green_data_ram_line0.d_a(d_a_green0_sig); 
    m_green_data_ram_line0.dp_a(dp_a_green0_sig); 
    // configuration port a
    m_green_data_ram_line0.WMODE_A = WRITE_FIRST;
    m_green_data_ram_line0.INIT_A = 0;
    m_green_data_ram_line0.SRVAL_A = 0;
    // port b
    m_green_data_ram_line0.clk_b(read_clk);
    m_green_data_ram_line0.en_b(read_enable);
    m_green_data_ram_line0.ssr_b(ssr_b_sig);
    m_green_data_ram_line0.we_b(we_b_sig);
    m_green_data_ram_line0.addr_b(read_address);
    m_green_data_ram_line0.di_b(di_b_sig); 
    m_green_data_ram_line0.dpi_b(dpi_b_sig); 
    m_green_data_ram_line0.d_b(read_green_data_line0); 
    m_green_data_ram_line0.dp_b(dp_b_green0_sig); 
    // configuration port b
    m_green_data_ram_line0.WMODE_B = WRITE_FIRST;
    m_green_data_ram_line0.INIT_B = 0;
    m_green_data_ram_line0.SRVAL_B = 0;
      
    // line 1

    // global set/reset port
    m_green_data_ram_line1.gsr(gsr_sig);
    // port a
    m_green_data_ram_line1.clk_a(write_clk);
    m_green_data_ram_line1.en_a(write_enable_line1);
    m_green_data_ram_line1.ssr_a(ssr_a_sig);
    m_green_data_ram_line1.we_a(we_a_sig);
    m_green_data_ram_line1.addr_a(write_address);
    m_green_data_ram_line1.di_a(write_green_data_line1); 
    m_green_data_ram_line1.dpi_a(dpi_a_sig); 
    m_green_data_ram_line1.d_a(d_a_green1_sig); 
    m_green_data_ram_line1.dp_a(dp_a_green1_sig); 
    // configuration port a
    m_green_data_ram_line1.WMODE_A = WRITE_FIRST;
    m_green_data_ram_line1.INIT_A = 0;
    m_green_data_ram_line1.SRVAL_A = 0;
    // port b
    m_green_data_ram_line1.clk_b(read_clk);
    m_green_data_ram_line1.en_b(read_enable);
    m_green_data_ram_line1.ssr_b(ssr_b_sig);
    m_green_data_ram_line1.we_b(we_b_sig);
    m_green_data_ram_line1.addr_b(read_address);
    m_green_data_ram_line1.di_b(di_b_sig); 
    m_green_data_ram_line1.dpi_b(dpi_b_sig); 
    m_green_data_ram_line1.d_b(read_green_data_line1); 
    m_green_data_ram_line1.dp_b(dp_b_green1_sig); 
    // configuration port b
    m_green_data_ram_line1.WMODE_B = WRITE_FIRST;
    m_green_data_ram_line1.INIT_B = 0;
    m_green_data_ram_line1.SRVAL_B = 0;

    // line 2

    // global set/reset port
    m_green_data_ram_line2.gsr(gsr_sig);
    // port a
    m_green_data_ram_line2.clk_a(write_clk);
    m_green_data_ram_line2.en_a(write_enable_line2);
    m_green_data_ram_line2.ssr_a(ssr_a_sig);
    m_green_data_ram_line2.we_a(we_a_sig);
    m_green_data_ram_line2.addr_a(write_address);
    m_green_data_ram_line2.di_a(write_green_data_line2); 
    m_green_data_ram_line2.dpi_a(dpi_a_sig); 
    m_green_data_ram_line2.d_a(d_a_green2_sig); 
    m_green_data_ram_line2.dp_a(dp_a_green2_sig); 
    // configuration port a
    m_green_data_ram_line2.WMODE_A = WRITE_FIRST;
    m_green_data_ram_line2.INIT_A = 0;
    m_green_data_ram_line2.SRVAL_A = 0;
    // port b
    m_green_data_ram_line2.clk_b(read_clk);
    m_green_data_ram_line2.en_b(read_enable);
    m_green_data_ram_line2.ssr_b(ssr_b_sig);
    m_green_data_ram_line2.we_b(we_b_sig);
    m_green_data_ram_line2.addr_b(read_address);
    m_green_data_ram_line2.di_b(di_b_sig); 
    m_green_data_ram_line2.dpi_b(dpi_b_sig); 
    m_green_data_ram_line2.d_b(read_green_data_line2); 
    m_green_data_ram_line2.dp_b(dp_b_green2_sig); 
    // configuration port b
    m_green_data_ram_line2.WMODE_B = WRITE_FIRST;
    m_green_data_ram_line2.INIT_B = 0;
    m_green_data_ram_line2.SRVAL_B = 0;

    //
    // blue BRAM array (consists out of three lines)
    //
    // line 0

    // global set/reset port
    m_blue_data_ram_line0.gsr(gsr_sig);
    // port a
    m_blue_data_ram_line0.clk_a(write_clk);
    m_blue_data_ram_line0.en_a(write_enable_line0);
    m_blue_data_ram_line0.ssr_a(ssr_a_sig);
    m_blue_data_ram_line0.we_a(we_a_sig);
    m_blue_data_ram_line0.addr_a(write_address);
    m_blue_data_ram_line0.di_a(write_blue_data_line0); 
    m_blue_data_ram_line0.dpi_a(dpi_a_sig); 
    m_blue_data_ram_line0.d_a(d_a_blue0_sig); 
    m_blue_data_ram_line0.dp_a(dp_a_blue0_sig); 
    // configuration port a
    m_blue_data_ram_line0.WMODE_A = WRITE_FIRST;
    m_blue_data_ram_line0.INIT_A = 0;
    m_blue_data_ram_line0.SRVAL_A = 0;
    // port b
    m_blue_data_ram_line0.clk_b(read_clk);
    m_blue_data_ram_line0.en_b(read_enable);
    m_blue_data_ram_line0.ssr_b(ssr_b_sig);
    m_blue_data_ram_line0.we_b(we_b_sig);
    m_blue_data_ram_line0.addr_b(read_address);
    m_blue_data_ram_line0.di_b(di_b_sig); 
    m_blue_data_ram_line0.dpi_b(dpi_b_sig); 
    m_blue_data_ram_line0.d_b(read_blue_data_line0); 
    m_blue_data_ram_line0.dp_b(dp_b_blue0_sig); 
    // configuration port b
    m_blue_data_ram_line0.WMODE_B = WRITE_FIRST;
    m_blue_data_ram_line0.INIT_B = 0;
    m_blue_data_ram_line0.SRVAL_B = 0;
      
    // line 1

    // global set/reset port
    m_blue_data_ram_line1.gsr(gsr_sig);
    // port a
    m_blue_data_ram_line1.clk_a(write_clk);
    m_blue_data_ram_line1.en_a(write_enable_line1);
    m_blue_data_ram_line1.ssr_a(ssr_a_sig);
    m_blue_data_ram_line1.we_a(we_a_sig);
    m_blue_data_ram_line1.addr_a(write_address);
    m_blue_data_ram_line1.di_a(write_blue_data_line1); 
    m_blue_data_ram_line1.dpi_a(dpi_a_sig); 
    m_blue_data_ram_line1.d_a(d_a_blue1_sig); 
    m_blue_data_ram_line1.dp_a(dp_a_blue1_sig); 
    // configuration port a
    m_blue_data_ram_line1.WMODE_A = WRITE_FIRST;
    m_blue_data_ram_line1.INIT_A = 0;
    m_blue_data_ram_line1.SRVAL_A = 0;
    // port b
    m_blue_data_ram_line1.clk_b(read_clk);
    m_blue_data_ram_line1.en_b(read_enable);
    m_blue_data_ram_line1.ssr_b(ssr_b_sig);
    m_blue_data_ram_line1.we_b(we_b_sig);
    m_blue_data_ram_line1.addr_b(read_address);
    m_blue_data_ram_line1.di_b(di_b_sig); 
    m_blue_data_ram_line1.dpi_b(dpi_b_sig); 
    m_blue_data_ram_line1.d_b(read_blue_data_line1); 
    m_blue_data_ram_line1.dp_b(dp_b_blue1_sig); 
    // configuration port b
    m_blue_data_ram_line1.WMODE_B = WRITE_FIRST;
    m_blue_data_ram_line1.INIT_B = 0;
    m_blue_data_ram_line1.SRVAL_B = 0;

    // line 2

    // global set/reset port
    m_blue_data_ram_line2.gsr(gsr_sig);
    // port a
    m_blue_data_ram_line2.clk_a(write_clk);
    m_blue_data_ram_line2.en_a(write_enable_line2);
    m_blue_data_ram_line2.ssr_a(ssr_a_sig);
    m_blue_data_ram_line2.we_a(we_a_sig);
    m_blue_data_ram_line2.addr_a(write_address);
    m_blue_data_ram_line2.di_a(write_blue_data_line2); 
    m_blue_data_ram_line2.dpi_a(dpi_a_sig); 
    m_blue_data_ram_line2.d_a(d_a_blue2_sig); 
    m_blue_data_ram_line2.dp_a(dp_a_blue2_sig); 
    // configuration port a
    m_blue_data_ram_line2.WMODE_A = WRITE_FIRST;
    m_blue_data_ram_line2.INIT_A = 0;
    m_blue_data_ram_line2.SRVAL_A = 0;
    // port b
    m_blue_data_ram_line2.clk_b(read_clk);
    m_blue_data_ram_line2.en_b(read_enable);
    m_blue_data_ram_line2.ssr_b(ssr_b_sig);
    m_blue_data_ram_line2.we_b(we_b_sig);
    m_blue_data_ram_line2.addr_b(read_address);
    m_blue_data_ram_line2.di_b(di_b_sig); 
    m_blue_data_ram_line2.dpi_b(dpi_b_sig); 
    m_blue_data_ram_line2.d_b(read_blue_data_line2); 
    m_blue_data_ram_line2.dp_b(dp_b_blue2_sig); 
    // configuration port b
    m_blue_data_ram_line2.WMODE_B = WRITE_FIRST;
    m_blue_data_ram_line2.INIT_B = 0;
    m_blue_data_ram_line2.SRVAL_B = 0;

    //
    // process declaration
    //
    SC_CTHREAD(line_sel_counter_process, Ho_444.pos());
    reset_signal_is(rst, true);

    SC_CTHREAD(mux_process, write_clk.pos());
    reset_signal_is(rst, true);
    
  }

protected:

  void line_sel_counter_process();
  void mux_process();

  ramb16_s9_s9 m_red_data_ram_line0;
  ramb16_s9_s9 m_red_data_ram_line1;
  ramb16_s9_s9 m_red_data_ram_line2;

  ramb16_s9_s9 m_green_data_ram_line0;
  ramb16_s9_s9 m_green_data_ram_line1;
  ramb16_s9_s9 m_green_data_ram_line2;

  ramb16_s9_s9 m_blue_data_ram_line0;
  ramb16_s9_s9 m_blue_data_ram_line1;
  ramb16_s9_s9 m_blue_data_ram_line2;

#else // SC_SYNTHESIS  
  
  SC_CTOR(chunk_buffer)
  {
    
  }

#endif  
};

#endif
