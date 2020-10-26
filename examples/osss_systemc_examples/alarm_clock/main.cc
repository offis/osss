/*----------------------------------------------------------------------------------------
 Copyright (c) 2005 Kuratorium OFFIS e.V., Oldenburg, Germany
  
 Name of the Library: OSSS (A library for synthesisable system level models in SystemC) 

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
 ----------------------------------------------------------------------------------------*/
#include <systemc.h>

// Testbench 
#ifndef SC_SYNTHESIS
  #include <iostream>
  #include "Stimulus.h"
#endif

// Top-level design
#ifdef USE_FOSSY_OUT
// We use the ending .cpp here to prevent that make will build and link
// both (which would result in an error)
// It would be more elegant, however, if fossy would produce separate .h
// and .cc files.
#  include "alarm_clock.fossy_out.cpp"
// After synthesis AlarmClock is not a template anymore
#  define ALARM_CLOCK_TOP AlarmClock
#else
#  include "AlarmClock.h"
#  define ALARM_CLOCK_TOP AlarmClock<10>
#  ifdef SC_SYNTHESIS
// In the synthesis case we must include all relevant cc files in
// order to create a single translation unit
#    include "Counter.cc"
#  endif
#endif

int
sc_main(int argc, char *argv[])
{

  ALARM_CLOCK_TOP alarm_clock( "alarm_clock" );
  
#ifndef SC_SYNTHESIS
  sc_time clockPeriod( 10, SC_NS );
  const int SIMULATED_CYCLES = 100;
  sc_clock clock( "clock", clockPeriod );
  
  sc_signal< bool > reset;
  sc_signal< bool > alarm;

//   sc_trace_file* tf = sc_create_vcd_trace_file( "trace" );
//   sc_trace( tf, clock.signal(), "clock" );
//   sc_trace( tf, reset, "reset" );
//   sc_trace( tf, alarm, "alarm" );


  alarm_clock.clock(clock);
  alarm_clock.reset(reset);
  alarm_clock.alarm(alarm);

  Stimulus  stimulus("stimulus");
  stimulus.clock(clock);
  stimulus.reset(reset);
  stimulus.alarm(alarm);
  
  sc_start( clockPeriod * SIMULATED_CYCLES );
  
  cerr << "Simulation ends ..." << endl;

#endif    
  return(0);
}
