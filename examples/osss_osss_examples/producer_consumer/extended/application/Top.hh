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

#ifndef TOP_INCLUDED_HH
#define TOP_INCLUDED_HH

#include <osss.h>
#include "Packet.hh"
#include "../FIFO.hh"
#include "Producer.hh"
#include "Consumer.hh"

SC_MODULE(Top) {
  sc_in<bool> clk, reset;
   
  SC_CTOR(Top); 

 protected:
  typedef osss_shared<FIFO<Packet, 10>, osss_least_recently_used<3> > Buffer_t;

  Producer    *m_Producer;
  Buffer_t    *m_Buffer;
  Consumer<1> *m_Consumer1;
  Consumer<2> *m_Consumer2;
};

#endif
