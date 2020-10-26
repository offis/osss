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
#ifdef SP
  #define SCHEDULER osss_static_priority<>
#else
  #ifdef RR
  #define SCHEDULER osss_round_robin
  #else
    #ifdef MR
    #define SCHEDULER osss_modified_round_robin
    #else
      #define SCHEDULER osss_static_priority<>
    #endif
  #endif
#endif

#include "Top.hh"

#define SIMULATED_CYCLES 2000

int
sc_main(int argc, char *argv[])
{
#ifndef SYNTHESIS
  sc_time clockPeriod( 20, SC_NS );
  sc_clock clock( "clock", clockPeriod );

  sc_signal< bool > reset;
  sc_signal< sc_biguint< 8 > > output1;
  sc_signal< sc_biguint< 8 > > output2;

#endif 

  Top< SCHEDULER > tcrvexample( "tcrvexample" );

#ifndef SYNTHESIS

  sc_trace_file* tf = sc_create_vcd_trace_file("trace");
  sc_trace(tf, clock.signal(), "clock");
  sc_trace(tf, reset, "reset");
  sc_trace(tf, output1, "output1" );
  sc_trace(tf, output2, "output2" );
  sc_trace(tf, tcrvexample.line1, "line1" );
  sc_trace(tf, tcrvexample.line2, "line2" );

  tcrvexample.clock( clock );
  tcrvexample.reset( reset );
  tcrvexample.output1( output1 );
  tcrvexample.output2( output2 );

//   // Initializations
  reset = true;

  //////////////////////////////////////////////////////////////////////////////
  // Simulation start                                                         //
  //////////////////////////////////////////////////////////////////////////////
  cout << "Simulation begins ..." << endl;
  osss_start( 2 * clockPeriod );

  // End of reset
  reset = false;
  osss_start( 2 * clockPeriod );

  osss_start( SIMULATED_CYCLES * clockPeriod );

  cout << "Simulation ends ..." << endl;
	
  return(0);
#endif 
}
