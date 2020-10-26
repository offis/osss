#include "chunk_buffer.hh"

void chunk_buffer::line_sel_counter_process()
{
  //reset
  ssr_a_sig = 0;
  ssr_b_sig = 0;
  we_a_sig  = 1;
  we_b_sig  = 0;
  dpi_a_sig = 0;
  di_b_sig  = 0;
  dpi_b_sig = 0;

  line_sel_counter = 0;
  wait();

  while(true)
  {
    if(line_sel_counter.read() == 2)
    {
      line_sel_counter = 0;
    }
    else
    {
      line_sel_counter = line_sel_counter.read() + 1;
    }

    wait();
  }

}

void chunk_buffer::mux_process()
{
  //reset
  write_enable_line0 = 0;
  write_enable_line1 = 0;
  write_enable_line2 = 0;
  write_red_data_line0 = 0;
  write_red_data_line1 = 0;
  write_red_data_line2 = 0;
  write_green_data_line0 = 0;
  write_green_data_line1 = 0;
  write_green_data_line2 = 0;
  write_blue_data_line0 = 0;
  write_blue_data_line1 = 0;
  write_blue_data_line2 = 0;
  wait();

  while(true)
  {
    if (line_sel_counter.read() == 0)
    {
      write_enable_line0 = 1; // enable line 0
      write_enable_line1 = 0;
      write_enable_line2 = 0;
      write_red_data_line0 = write_red_data.read();
      write_red_data_line1 = 0;
      write_red_data_line2 = 0;
      write_green_data_line0 = write_green_data.read();
      write_green_data_line1 = 0;
      write_green_data_line2 = 0;
      write_blue_data_line0 = write_blue_data.read();
      write_blue_data_line1 = 0;
      write_blue_data_line2 = 0;
    }
    else if (line_sel_counter.read() == 1)
    {
      write_enable_line0 = 0;
      write_enable_line1 = 1; // enable line 1
      write_enable_line2 = 0;
      write_red_data_line0 = 0;
      write_red_data_line1 = write_red_data.read();
      write_red_data_line2 = 0;
      write_green_data_line0 = 0;
      write_green_data_line1 = write_green_data.read();
      write_green_data_line2 = 0;
      write_blue_data_line0 = 0;
      write_blue_data_line1 = write_blue_data.read();
      write_blue_data_line2 = 0;     
    }
    else
    {
      write_enable_line0 = 0;
      write_enable_line1 = 0;
      write_enable_line2 = 1; // enable line 2
      write_red_data_line0 = 0;
      write_red_data_line1 = 0;
      write_red_data_line2 = write_red_data.read();
      write_green_data_line0 = 0;
      write_green_data_line1 = 0;
      write_green_data_line2 = write_green_data.read();
      write_blue_data_line0 = 0;
      write_blue_data_line1 = 0;
      write_blue_data_line2 = write_blue_data.read();
    }

    wait();
  }

}
