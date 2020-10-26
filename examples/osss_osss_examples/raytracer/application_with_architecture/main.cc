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
#include <systemc.h>
#include "Top.hh"

#ifndef SYNTHESIS
#include "Testbench.hh"
#endif // SYNTHESIS

int sc_main(int, char**)
{
  Top top("top");

#ifndef SYNTHESIS
  Testbench testbench("testbench");
  sc_signal<bool> s_clock;
  sc_signal<bool> s_reset;

  top.pi_bClk(s_clock);
  top.pi_bReset(s_reset);
  
  testbench.po_bClk(s_clock);
  testbench.po_bReset(s_reset);

  /*
  // tracing this application for the calculation results in a > 2GB trace file!
  sc_trace_file* tf = sc_create_vcd_trace_file( "trace" );
  sc_trace( tf, s_clock, "clock" );
  sc_trace( tf, s_reset, "reset" );
  top.m_processor_bus->trace_all(tf);
  */
  
  osss_start();
#endif

  return 0;
}
