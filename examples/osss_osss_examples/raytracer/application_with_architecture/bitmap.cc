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

// 2006-04-11, Thorsten Schubert, OFFIS

#include "bitmap.hh"

#if !defined(__cplusplus)
void initBitmap(Bitmap *b)
{
  b->m_size.x_min=0;
  b->m_size.y_min=0;
  b->m_size.x_max=BITMAP_SIZE_X-1;
  b->m_size.y_max=BITMAP_SIZE_Y-1; 
}

Color getPixel(const Bitmap *b, Bitmap_coord_t x, Bitmap_coord_t y)
{ 
  return b->m_data[y][x]; 
}

void setPixel(Bitmap *b, Bitmap_coord_t x, Bitmap_coord_t y, Color c)
{
  b->m_data[y][x] = c;
}


void  fillTile(Bitmap *b, const Tile *t, Color c)
{
  Bitmap_coord_t x,y;
  
  for (y=t->y_min; y <= t->y_max; ++y)
  {
    for (x=t->x_min; x <= t->x_max; ++x)
    {
      setPixel(b, x, y, c);
    }
  }
}

#else

void  
Bitmap::setPixel(Bitmap_coord_t x, Bitmap_coord_t y, Color c)
{
  m_data[y][x] = c;
}

Color 
Bitmap::getPixel(Bitmap_coord_t x, Bitmap_coord_t y) const
{
  return m_data[y][x];
}

void  
Bitmap::fillTile(const Tile& t, Color c)
{
  Bitmap_coord_t x,y;
  
  for (y=t.y_min; y <= t.y_max; ++y)
  {
    for (x=t.x_min; x <= t.x_max; ++x)
    {
      setPixel(x, y, c);
    }
  }
}

#endif
