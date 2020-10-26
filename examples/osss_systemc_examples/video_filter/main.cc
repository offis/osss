#include "Top.hh"

#ifndef SYNTHESIS
#include "Testbench.hh"
#endif // SYNTHESIS

int printu8(sc_biguint<8> i) { return i.to_int();}
int print8(sc_bigint<8> i) { return i.to_int();}

int sc_main(int, char**)
{
  Top m_top("m_top");

#ifndef SYNTHESIS
  Testbench testbench("testbench");
  sc_signal<bool> s_clock;
  sc_signal<bool> s_reset;
   
  testbench.po_bClk(s_clock);
  testbench.po_bReset(s_reset);

  m_top.clock(s_clock);
  m_top.reset(s_reset);

  sc_trace_file* Tf;
  Tf = sc_create_vcd_trace_file("trace_" FILTER_INNER_CC_FILE);
  ((vcd_trace_file*)Tf)->sc_set_vcd_time_unit(-9);

  sc_trace(Tf, m_top.m_filter.rst  , "rst" );
  sc_trace(Tf, m_top.m_filter.Ho_444  , "Ho_444" );
  sc_trace(Tf, m_top.m_filter.write_clk  , "write_clk" );
//   sc_trace(Tf, m_top.m_filter.write_address  , "write_address" );
//   sc_trace(Tf, m_top.m_filter.red_in  , "red_in" );
//   sc_trace(Tf, m_top.m_filter.green_in  , "green_in" );
//   sc_trace(Tf, m_top.m_filter.blue_in  , "blue_in" );
//   sc_trace(Tf, m_top.m_filter.red_out  , "red_out" );
//   sc_trace(Tf, m_top.m_filter.green_out  , "green_out" );
//   sc_trace(Tf, m_top.m_filter.blue_out  , "blue_out" );
//   sc_trace(Tf, m_top.m_filter.read_red_data_line0 , "read_red_data_line0" );
//   sc_trace(Tf, m_top.m_filter.read_red_data_line1  , "read_red_data_line1" );
//   sc_trace(Tf, m_top.m_filter.read_red_data_line2  , "read_red_data_line2" );
//   sc_trace(Tf, m_top.m_filter.read_green_data_line0 , "read_green_data_line0" );
//   sc_trace(Tf, m_top.m_filter.read_green_data_line1  , "read_green_data_line1" );
//   sc_trace(Tf, m_top.m_filter.read_green_data_line2  , "read_green_data_line2" );
//   sc_trace(Tf, m_top.m_filter.read_blue_data_line0 , "read_blue_data_line0" );
//   sc_trace(Tf, m_top.m_filter.read_blue_data_line1  , "read_blue_data_line1" );
//   sc_trace(Tf, m_top.m_filter.read_blue_data_line2  , "read_blue_data_line2" );
//   sc_trace(Tf, m_top.m_filter.read_enable  , "read_enable");
//   sc_trace(Tf, m_top.m_filter.line_sel_counter  , "line_sel_counter");
// 
//   sc_trace(Tf, m_top.m_filter.red_filter_matrix_sig  , "red_filter_matrix_sig");
//   sc_trace(Tf, m_top.m_filter.green_filter_matrix_sig  , "green_filter_matrix_sig");
//   sc_trace(Tf, m_top.m_filter.blue_filter_matrix_sig  , "blue_filter_matrix_sig");


  //sc_start(400*testbench.GetClockCyclePeriod());
  sc_start();

  sc_close_vcd_trace_file(Tf);

#endif

  return 0;
}
