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

#ifndef BITMAP_INCLUDED
#define BITMAP_INCLUDED

#if defined(__cplusplus)
#include <iostream>
#include <vector>
#endif

#include "color.hh"

typedef unsigned short Bitmap_coord_t;

typedef struct
{
  Bitmap_coord_t  x_min;
  Bitmap_coord_t  y_min;
  Bitmap_coord_t  x_max;
  Bitmap_coord_t  y_max;
} Tile;


#define BITMAP_SIZE_X 200
#define BITMAP_SIZE_Y 200

#if defined(__cplusplus)
struct Bitmap
#else
typedef struct
#endif
{
  Tile  m_size;

#if defined(__cplusplus)
  std::vector<std::vector<Color> > m_data;
#else
  Color m_data[BITMAP_SIZE_Y][BITMAP_SIZE_X];
#endif

#if defined(__cplusplus)
  Bitmap()
  {
    m_data = std::vector<std::vector<Color> >(BITMAP_SIZE_Y);
    for(unsigned int y=0; y<BITMAP_SIZE_Y; y++)
    {
      m_data[y] = std::vector<Color>(BITMAP_SIZE_X);
    }
    
    m_size.x_min=0;
    m_size.y_min=0;
    m_size.x_max=BITMAP_SIZE_X-1;
    m_size.y_max=BITMAP_SIZE_Y-1; 
  }

  Bitmap(const Tile& t)
  {
    m_data = std::vector<std::vector<Color> >(t.y_max+1);
    for(Bitmap_coord_t y=0; y<(t.y_max+1); y++)
    {
      m_data[y] = std::vector<Color>(t.x_max+1);
    }

    m_size = t;
  }

  Bitmap(const Bitmap& b)
  {
    m_data = std::vector<std::vector<Color> >(b.m_size.y_max+1);
    for(Bitmap_coord_t y=0; y<(b.m_size.y_max+1); y++)
    {
      m_data[y] = std::vector<Color>(b.m_size.x_max+1);
    }

    for(Bitmap_coord_t y=0; y<(b.m_size.y_max+1); y++)
    {
      for(Bitmap_coord_t x=0; x<(b.m_size.x_max+1); x++)
      {
        m_data[y][x] = b.m_data[y][x];
      }
    }

    m_size = b.m_size;
  }

  Bitmap& operator = (const Bitmap &b)
  {
    for(Bitmap_coord_t y=0; y<(b.m_size.y_max+1); y++)
    {
      for(Bitmap_coord_t x=0; x<(b.m_size.x_max+1); x++)
      {
        m_data[y][x] = b.m_data[y][x];
      }
    }

    m_size = b.m_size;
    return *this;
  }

  void  setPixel(Bitmap_coord_t x, Bitmap_coord_t y, Color c);
  Color getPixel(Bitmap_coord_t x, Bitmap_coord_t y) const;

  void  fillTile(const Tile& t, Color c);

#endif

#if defined(__cplusplus)
};
#else
} Bitmap;
#endif

#if !defined(__cplusplus)
/* Set m_size */
void initBitmap(Bitmap *b);

void  setPixel(Bitmap *b, Bitmap_coord_t x, Bitmap_coord_t y, Color c);
Color getPixel(const Bitmap *b, Bitmap_coord_t x, Bitmap_coord_t y);

void  fillTile(Bitmap *b, const Tile *t, Color c);
#endif

#endif
