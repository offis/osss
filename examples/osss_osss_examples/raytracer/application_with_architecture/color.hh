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

#ifndef COLOR_INCLUDED
#define COLOR_INCLUDED

#define OSSS_GREEN
#include <osss.h>

#include "real.hh"

#define NUM_BITS_RED    8  
#define NUM_BITS_GREEN  8
#define NUM_BITS_BLUE   8

#define COLOR_MAX_RED   ((1 << (NUM_BITS_RED))-1)
#define COLOR_MAX_GREEN ((1 << (NUM_BITS_GREEN))-1)
#define COLOR_MAX_BLUE  ((1 << (NUM_BITS_BLUE))-1)


typedef unsigned char Color_base_t;

class Color : public osss_serialisable_object
{
 public:

  OSSS_IS_SERIALISABLE(Color);
  
  OSSS_SERIALISABLE_CTOR(Color, ()) : r(0), g(0), b(0)
    {}

  OSSS_SERIALISABLE_CTOR(Color, (Color_base_t red,
				 Color_base_t green,
				 Color_base_t blue))
  {
    r = red,
    g = green;
    b = blue;
  }

  OSSS_SERIALISABLE_CTOR(Color, (const Color& c))
  {
    r = c.r;
    g = c.g;
    b = c.b;
  }

  Color& operator=(const Color& c)
  { 
    r = c.r;
    g = c.g;
    b = c.b;
    return *this;
  }

  virtual void serialise()
  {
    osss_serialisable_object::store_element(r);
    osss_serialisable_object::store_element(g);
    osss_serialisable_object::store_element(b);
  }	
						
  virtual void deserialise()
  {
    osss_serialisable_object::restore_element(r);
    osss_serialisable_object::restore_element(g);
    osss_serialisable_object::restore_element(b);
  }

  Color_base_t r;
  Color_base_t g;
  Color_base_t b;

};

Color colorBlack();
Color colorWhite();
Color colorRed();
Color colorGreen();
Color colorBlue();

/* Access the color's rgb-components scaled to/from 8 bit integers */
void  colorSplit24(Color c, Color_base_t *r, Color_base_t *g, Color_base_t *b);
Color colorJoin24(Color_base_t r, Color_base_t g, Color_base_t b);

/* Saturated scaling: each channel will be clipped to 0..max */
Color colorScaleSat(real_t k, Color c);
/* Element-wise sum */
Color colorAddSat(Color a, Color b);
/* Element-wise product */
Color colorMultSat(Color a, Color b);

#endif
