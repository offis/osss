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
#ifndef __MASTER_H__
#define __MASTER_H__

#include <systemc.h>
#include "Bridge_stub.h"

#ifndef SYNTHESIS
#include <iostream>
#endif //SYNTHESIS

template< int nr>
SC_MODULE( Master )
{
  //PORTS
  sc_in_clk clock;
  sc_in< bool > resetn;
  sc_out< sc_uint< 32 > > output;
  
  osss_port_to_rmi< Bridge_if > bridge_write_port;
  osss_port_to_rmi< Bridge_if > bridge_read_port;
  
  void write();
  
  void read();
  
  SC_CTOR( Master ) :
    clock("clock"),
    resetn("resetn"),
    output("output"),
    bridge_write_port("bridge_write_port"),
    bridge_read_port("bridge_read_port")
  {
    SC_CTHREAD(write, clock.pos() );
    reset_signal_is( resetn, false );

    SC_CTHREAD(read,clock.pos() );
    reset_signal_is( resetn, false );
  }
  
};

#include "Master.icc"

#endif //__MASTER_H__
