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

#ifndef __TOP_INCLUDED__
#define __TOP_INCLUDED__

#include "Newton.hh"

SC_MODULE(Top)
{
  sc_in< bool > clock;
  sc_in< bool > reset;

  sc_out< sc_biguint< IW - 1 > > operandEven; 
  sc_out< sc_biguint< IW - 1 > > operandOdd; 
  sc_out< sc_fixed< W, IW > >    outputEven; 
  sc_out< sc_fixed< W, IW > >    outputOdd; 

  SC_CTOR(Top) :
    sharedALU("sharedALU"),
    newton( "newton" )
  {
    sharedALU.clock_port( clock );
    sharedALU.reset_port( reset );

    newton.clock( clock );
    newton.reset( reset );
    newton.operandEven( operandEven );  
    newton.outputEven( outputEven );
    newton.operandOdd( operandOdd );  
    newton.outputOdd( outputOdd );
    newton.alu_port1( sharedALU );
    newton.alu_port2( sharedALU );
  }

 protected: 
  osss_shared< ALU< 2 * W, 2 * IW >, osss_round_robin > sharedALU;
  Newton< W, IW > newton;

};

#endif
