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

////////////////////////////////////////////////////////////
// File: tb_sharedALU.cpp                                 //
// Testbench for shared ALU. Simulation run is stopped,   //
// as soon as the square roots from 0,...,20 have been    //
// calcualted.                                            //
////////////////////////////////////////////////////////////

// wordlength = 16 bit, integer wordlength = 6 bit
// decimal wordlength = 16 bit - 6 bit = 10 bit
const unsigned int  W = 16;
const unsigned int  IW = 6;

#define SC_INCLUDE_FX
#include <systemc.h>

#define OSSS_GREEN // Virtual Target Architecture Layer Model
#include <osss.h>

#include "Top.hh"

int
sc_main(int argc, char *argv[])
{
#ifndef SYNTHESIS

  sc_time clockPeriod( 30, SC_NS );
  sc_clock clock( "clock", clockPeriod );

  sc_signal< bool > reset;
  sc_signal< sc_biguint< IW - 1 > > operandEven; 
  sc_signal< sc_biguint< IW - 1 > > operandOdd; 
  sc_signal< sc_fixed< W, IW > > outputEven; 
  sc_signal< sc_fixed< W, IW > > outputOdd; 

#endif //SYNTHESIS

 Top top( "top" );

#ifndef SYNTHESIS

 sc_trace_file* tf = sc_create_vcd_trace_file("trace");

 sc_trace(tf, clock.signal(), "clock");
 sc_trace(tf, reset, "reset");
 sc_trace(tf, operandEven, "operandEven" );
 sc_trace(tf, outputEven, "outputEven" );
 sc_trace(tf, operandOdd, "operandOdd" );
 sc_trace(tf, outputOdd, "outputOdd" );

 top.clock( clock );
 top.reset( reset );
 top.operandEven( operandEven );  
 top.outputEven( outputEven );
 top.operandOdd( operandOdd );  
 top.outputOdd( outputOdd );

 ///////////////////////////////////////////////////////
 // Simulation start                                  //
 ///////////////////////////////////////////////////////
 cout << "Simulation begins ..." << endl;
 reset = true;
 osss_start( 2 * clockPeriod );

 // End of reset
 reset = false;
 osss_start( 2 * clockPeriod );

 // When operand 22 appears that means that the
 // calculation of the squared root for operand 
 // 20 was just finished.
 while ( operandEven.read() != 22 )
 {
   osss_start( clockPeriod );
 }

 cout << "Time: " << sc_time_stamp() << endl;
 cout << "Simulation ends ..." << endl;
    
 return(0);

#endif //SYNTHESIS
}
