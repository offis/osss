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
#ifndef __APBMASTER_HH__
#define __APBMASTER_HH__

#include "Bridge_stub.h"

SC_MODULE(APBMaster)
{
  //PORTS
  sc_in_clk pclk;
  sc_in <bool> presetn;
  sc_out <bool> pwrite;
  sc_out <bool> penable;
  sc_out <bool> psel0;
  sc_out <bool> psel1;
  sc_out <bool> psel2;
  sc_out <sc_uint<32> > pwdata;
  sc_out <sc_uint<32> > paddr;
  sc_in <sc_uint< 32 > > prdata;
  
  osss_port<osss_rmi_if< Bridge_if > > bridge_port;
  
  void work();
  
  SC_CTOR(APBMaster) : 
    pclk("pclk"),
    presetn("presetn"),
    pwrite("pwrite"),
    penable("penable"),
    psel0("psel0"),
    psel1("psel1"),
    psel2("psel2"),
    pwdata("pwdata"),
    paddr("paddr"),
    prdata("prdata"),
    bridge_port("bridge_port")
  {
    SC_CTHREAD( work, pclk.pos() );
    reset_signal_is( presetn, false );
  }
  
};

#include "APBMaster.icc"

#endif // __APBMASTER_HH__


