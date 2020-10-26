#include "Video_Source.hh"

void
Video_Source::write_img()
{

  //reset
  Ho_444 = 0;
  write_address = 0;
  red_out = 0;
  green_out = 0;
  blue_out = 0;
  Ho_444 = 0;
  wait();

  Ho_444 = 1;
  wait();
  Ho_444 = 0;
  wait();
  
  while(true)
  {
    for(unsigned int y=0; y<m_dimy; y++)
    {
      for(unsigned int x=0; x<m_dimx; x++)
      {
        red_out = m_red_layer.at(x, y);
        green_out = m_green_layer.at(x, y);
        blue_out = m_blue_layer.at(x, y);
        write_address = x;
        if(x == (m_dimx - 1))
          Ho_444 = 1;
        else
          Ho_444 = 0;
        
        wait();
      }
    }

    sc_stop();
  }

}
