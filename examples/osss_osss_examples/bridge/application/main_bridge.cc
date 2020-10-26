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
#include "Top.h"

#define SO_SCHEDULER osss_ceiling_priority<5>

int sc_main(int argc, char *argv[])
{
  #ifndef SYNTHESIS
  sc_time clockPeriod( 20, SC_NS);
  sc_clock clock( "clock", clockPeriod);
  sc_signal< bool > resetn;
  #endif //SYNTHESIS
  
  Top< SO_SCHEDULER > top( "top" );
  
  #ifndef SYNTHESIS
  sc_trace_file* tf = sc_create_vcd_trace_file("trace");
  sc_trace(tf, clock.signal(), "clock");
  sc_trace(tf, resetn, "ResetN");
  sc_trace(tf, top.pwrite, "PWrite");
  sc_trace(tf, top.penable, "PEnable");
  sc_trace(tf, top.psel0, "PSel0");
  sc_trace(tf, top.psel1, "PSel1");
  sc_trace(tf, top.psel2, "PSel2");
  sc_trace(tf, top.pwdata, "PWData");
  sc_trace(tf, top.prdata, "PRData");
  
  top.clock( clock );
  top.resetn( resetn );
  
  resetn = false;
  
  cout << "Simulation start... " << endl;
  
  osss_start( 2 * clockPeriod );
  
  resetn = true;
  
  osss_start(1000 * clockPeriod );
  
  cout << "Simulation end... " << endl;
  
  sc_close_vcd_trace_file(tf);
  
  return (0);
  
  #endif //SYNTHESIS
}
  
  
