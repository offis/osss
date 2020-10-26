#include "filter_inner.hh"

void filter_inner::sel_counter_proc()
{
  // reset
  line_sel_counter = 0;

  while(true)
  {
    wait();
    
    if (line_sel_counter.read() >= 2) 
      line_sel_counter = 0;
    else
      line_sel_counter = line_sel_counter.read() + 1;
  }
}
 
void filter_inner::shift_filter_proc()
{
  // reset
  
  read_enable = 1;

  // call user-defined ctor of the matrix specialisation
  pixel_matrix_t 
    zero_matrix (static_cast<pixel_t>(0));
    
  pixel_matrix_t 
    red_filter_matrix   = zero_matrix,
    green_filter_matrix = zero_matrix,
    blue_filter_matrix  = zero_matrix;

  
  red_filter_matrix_sig   = red_filter_matrix;
  green_filter_matrix_sig = green_filter_matrix;
  blue_filter_matrix_sig  = blue_filter_matrix;

  while(true)
  {
    wait();
    
    red_filter_matrix   <<= 1;
    green_filter_matrix <<= 1;
    blue_filter_matrix  <<= 1;

    if (line_sel_counter.read() == sc_uint<2>(0u)) 
    {
      red_filter_matrix.x0.x0   = read_red_data_line0.read(); 
      red_filter_matrix.x1.x0   = read_red_data_line1.read(); 
      red_filter_matrix.x2.x0   = read_red_data_line2.read(); 
      green_filter_matrix.x0.x0 = read_green_data_line0.read();
      green_filter_matrix.x1.x0 = read_green_data_line1.read();
      green_filter_matrix.x2.x0 = read_green_data_line2.read();
      blue_filter_matrix.x0.x0  = read_blue_data_line0.read();
      blue_filter_matrix.x1.x0  = read_blue_data_line1.read();
      blue_filter_matrix.x2.x0  = read_blue_data_line2.read();
    }
    else if (line_sel_counter.read() == sc_uint<2>(1u))
    {
      red_filter_matrix.x0.x0   = read_red_data_line1.read(); 
      red_filter_matrix.x1.x0   = read_red_data_line2.read(); 
      red_filter_matrix.x2.x0   = read_red_data_line0.read(); 
      green_filter_matrix.x0.x0 = read_green_data_line1.read();
      green_filter_matrix.x1.x0 = read_green_data_line2.read();
      green_filter_matrix.x2.x0 = read_green_data_line0.read();
      blue_filter_matrix.x0.x0  = read_blue_data_line1.read();
      blue_filter_matrix.x1.x0  = read_blue_data_line2.read();
      blue_filter_matrix.x2.x0  = read_blue_data_line0.read();
    }
    else if (line_sel_counter.read() == sc_uint<2>(2u))
    {
      red_filter_matrix.x0.x0   = read_red_data_line2.read(); 
      red_filter_matrix.x1.x0   = read_red_data_line0.read(); 
      red_filter_matrix.x2.x0   = read_red_data_line1.read(); 
      green_filter_matrix.x0.x0 = read_green_data_line2.read();
      green_filter_matrix.x1.x0 = read_green_data_line0.read();
      green_filter_matrix.x2.x0 = read_green_data_line1.read();
      blue_filter_matrix.x0.x0  = read_blue_data_line2.read();
      blue_filter_matrix.x1.x0  = read_blue_data_line0.read();
      blue_filter_matrix.x2.x0  = read_blue_data_line1.read();
    }
    else
    {
      red_filter_matrix   = zero_matrix;
      green_filter_matrix = zero_matrix;
      blue_filter_matrix  = zero_matrix;
    }
    
    red_filter_matrix_sig   = red_filter_matrix;
    green_filter_matrix_sig = green_filter_matrix;
    blue_filter_matrix_sig  = blue_filter_matrix; 
  }
}
 
void filter_inner::apply_coeffs_proc()
{
  // reset
  coeff_matrix_t 
    coeff_table ( 0, -1, 0,
                 -1,  5,-1,
                  0, -1, 0 );

  while(true)
  {
    wait();
    
    red_out   = coeff_table * red_filter_matrix_sig;
    green_out = coeff_table * green_filter_matrix_sig;
    blue_out  = coeff_table * blue_filter_matrix_sig;
  }
}
