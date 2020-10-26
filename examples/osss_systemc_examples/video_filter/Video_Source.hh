#ifndef VIDEO_SOURCE_INCLUDED
#define VIDEO_SOURCE_INCLUDED

#include <systemc.h>

#define cimg_display_type 0
#include "CImg-1-19/CImg.h"
using namespace cimg_library;

SC_MODULE(Video_Source)
{

  //
  // port section
  //
  sc_in<bool> clock;
  sc_in<bool> reset;

  sc_out<bool> Ho_444;
  sc_out<sc_biguint<11> > write_address;

  sc_out<sc_biguint<8> > red_out;
  sc_out<sc_biguint<8> > green_out;
  sc_out<sc_biguint<8> > blue_out;
  
  SC_CTOR(Video_Source)
  {
#ifdef LOCAL_BUILD_FOR_FOSSY_TESTCASE
    CImg<unsigned char> my_img("test_small.bmp");
#else
    CImg<unsigned char> my_img("test_big.bmp");
#endif
    m_dimx = my_img.dimx();
    m_dimy = my_img.dimy();
  
    m_red_layer = my_img.get_channel(0);
    m_green_layer = my_img.get_channel(1);
    m_blue_layer = my_img.get_channel(2);
   
    SC_CTHREAD(write_img, clock.pos());
    // reset_signal_is(reset, true); 
  }

 protected:

  void write_img();
  
  unsigned int m_dimx;
  unsigned int m_dimy;
  CImg<unsigned char> m_red_layer;
  CImg<unsigned char> m_green_layer;
  CImg<unsigned char> m_blue_layer;

};

#endif
