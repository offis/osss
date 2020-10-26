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
#ifndef __SLAVE_H__
#define __SLAVE_H__

#include <systemc.h>

#ifndef SYNTHESIS
  #include <iostream>
#endif //SYNTHESIS

SC_MODULE( Slave )
{
  //PORTS
  sc_in_clk pclk;
  sc_in <bool> presetn;
  sc_in <bool> pwrite;
  sc_in <bool> penable;
  sc_in <bool> psel;
  sc_in <sc_uint<32> > pwdata;
  sc_in <sc_uint<32> > paddr;
  sc_out <sc_uint<32> > prdata;
  
  //Register
  sc_uint<32> reg;
  
  void doit();
  
  //Constructor
  SC_CTOR( Slave ) :
    pclk("pclk"),
    presetn("presetn"),
    pwrite("pwrite"),
    penable("penable"),
    psel("psel"),
    pwdata("pwdata"),
    paddr("paddr"),
    prdata("prdata")
  {
    SC_CTHREAD( doit, pclk.pos() );
    reset_signal_is( presetn, false );
  }
  
};

#include "Slave.icc"

#endif //__SLAVE_H__
