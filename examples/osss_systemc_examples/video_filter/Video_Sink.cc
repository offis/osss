#include "Video_Sink.hh"

//#define DEBUG_SINK

void
Video_Sink::read_img()
{
  //reset
  unsigned int x_count = 0;
  unsigned int y_count = 0;
  bool first_run = true;
  wait();

  while(true)
  {

    while(first_run && Ho_444 == 0)
      wait();

    first_run = false;

#ifdef DEBUG_SINK
    std::cout << "@ " << sc_time_stamp() 
              << " r: " << red_in 
              << " g: " << green_in 
              << " b: " << blue_in << std::endl;
#endif
    
    m_image.at(x_count, y_count, 0, 0) = red_in.read().to_int();
    m_image.at(x_count, y_count, 0, 1) = green_in.read().to_int();
    m_image.at(x_count, y_count, 0, 2) = blue_in.read().to_int();

    wait();

    if (Ho_444 == 1)
    {
      x_count = 0;
      y_count++;
    }
    else
    {
      x_count++;
    }
  }
}
