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
#ifndef BITMAP_MANAGER_INCLUDED
#define BITMAP_MANAGER_INCLUDED

#include "Bitmap_Manager_if.hh"

#define BITMAP_MANAGER_VERBOSE

template<unsigned int NUM_TRACER>
class Bitmap_Manager : public Bitmap_Manager_if
{
public:
  
  Bitmap_Manager();

  OSSS_GUARDED_METHOD_VOID( setPixel, 
			    OSSS_PARAMS(3, Bitmap_coord_t, x, 
                                           Bitmap_coord_t, y, 
                                           Color, c),
			    m_printing_finished );

  OSSS_GUARDED_METHOD( Color, 
		       getPixel,
		       OSSS_PARAMS(2, Bitmap_coord_t, x, 
                                      Bitmap_coord_t, y),
		       (m_tracing_finished == NUM_TRACER) );

  OSSS_GUARDED_METHOD_VOID( tracing_finished,
			    OSSS_PARAMS(0), 
			    true );

  OSSS_GUARDED_METHOD_VOID( printing_finished,
			    OSSS_PARAMS(0),
			    true );

private:

  Bitmap m_bitmap;
  unsigned int m_tracing_finished;
  bool m_printing_finished;
};

#include "Bitmap_Manager.icc"

#endif
