/*------------------------------------------------------------------------------
 Copyright (c) 2005 Kuratorium OFFIS e.V., Oldenburg, Germany
  
 Name of the Library: OSSS (A library for synthesisable system level models 
                            in SystemC) 

 Created for Project: ICODES (http://icodes.offis.de)

 Created by: See AUTHORS (file)

 
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Contact:
 Kuratorium OFFIS e.V. (http://offis.de)
 Escherweg 2
 D-26121 Oldenburg
 PHONE: *49 (441) 9722-0
 FAX: *49 (441) 9722-102
 -----------------------------------------------------------------------------*/

#include "Image.h"

Image::Image() 
{}
    
void Image::clear()
{
  for (unsigned int i=0; i<NUM_BINS; ++i)
  {
    data[i] = 0;
  }
}

void 
Image::fillImage(sc_uint<8> fill_data)
{
  for (unsigned int i=0; i<NUM_BINS; ++i)
  {
    data[i] = fill_data;
  }
}

void Image::setImage()
{
  int value=0;
  
  for (int i=0; i<NUM_BINS; i++)
  {
    data[i] = value;
    
    if (value < 10)
    {
      value = value + 1;
    }
    else value = 0;
  }
}

int Image::numPixels()
{
  return NUM_BINS;
}

void Image::setPixel(int i, sc_uint<8> p)
{
  data[i] = p;
}

sc_uint<8> Image::getPixel(int i) const
{
  return data[i];
}

Image
Image::operator+(Image &j)
{
  Image j2;
  
  for (int i=0; i<NUM_BINS; i++)
  {
    j2.setPixel( i, data[i] +j.getPixel(i) );    
  }  
  return j2;
}

void Image::print()
{
  for (int i = 0; i < NUM_BINS; i++)
    std::cout << data [i] << " ";
}

Image::~Image()
{
}
