#ifndef VIDEO_SINK_INCLUDED
#define VIDEO_SINK_INCLUDED

#include <systemc.h>

#define cimg_display_type 0
#include "CImg-1-19/CImg.h"
using namespace cimg_library;

#ifndef OUTPUT_TAG
#define OUTPUT_TAG ""
#endif

#ifndef FILTER_INNER_CC_FILE
#define FILTER_INNER_CC_FILE ""
#endif

SC_MODULE(Video_Sink)
{

  //
  // port section
  //
  sc_in<bool> Ho_444;
  sc_in<bool> rst;
  sc_in<bool> write_clk;

  sc_in<sc_biguint<8> > red_in;
  sc_in<sc_biguint<8> > green_in;
  sc_in<sc_biguint<8> > blue_in;
  
  SC_CTOR(Video_Sink) :
    // Define a 720x480 color image with 8 bits per color component.
    m_image(720,480,1,3)
  {
    SC_CTHREAD(read_img, write_clk.pos());
    reset_signal_is(rst, true);
  }

  ~Video_Sink()
  {
    m_image.save("test_out_" FILTER_INNER_CC_FILE "_" OUTPUT_TAG ".bmp");
  }

 protected:

  void read_img();
  
  CImg<unsigned char> m_image;

};

#endif
