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
// File: Newton.hh                                        //
// Declares module 'Newton'. Newton contains the two      //
// processes 'odd' and 'even' which calculate the square  //
// roots according to Newton's iteration. The necessary   //
// arithmetic operations are provided by a shared object  //
// which instantiates class ALU.                          //
////////////////////////////////////////////////////////////

#ifndef __NEWTON__
#define __NEWTON__

#include "ALU.hh"

template< unsigned int W, unsigned int IW >
SC_MODULE( Newton )
{
  // Ports
  sc_in< bool > clock;
  sc_in< bool > reset;
  sc_out< sc_biguint< IW - 1 > > operandEven; 
  sc_out< sc_biguint< IW - 1 > > operandOdd; 
  sc_out< sc_fixed< W, IW > > outputEven; 
  sc_out< sc_fixed< W, IW > > outputOdd; 

  // Shared object ports
  osss_port_to_rmi< ALU_if< 2 * W, 2 * IW > > alu_port1; 
  osss_port_to_rmi< ALU_if< 2 * W, 2 * IW > > alu_port2;

  // Processes
  void even()
  {
    sc_biguint< IW - 1 > operand = 2;
    sc_fixed< 2 * W + 1, 2 * IW + 1 > diff = 0;
    sc_fixed< 2 * W, 2 * IW > div = 0;
    sc_fixed< 2 * W - IW, W > x0 = 0;
    sc_fixed< 2 * W - IW, W > x1 = 0;
    sc_fixed< 2 * W, 2 * IW > tmp1 = 0;
    sc_fixed< 2 * W + 1, 2 * IW + 1 > tmp2 = 0;
    sc_fixed< W, IW > tmp3 = 0;
  
    operandEven.write( operand );
    outputEven.write( x0 );

    alu_port1->reset();

    wait();  // end of reset
    
    while( true )
    {
      operandEven.write( operand );
      diff = 0;
      x0 = operand;
      x1 = operand;
      tmp3 = operand;
      
      do 
      {
        x0 = x1;
     	div = alu_port1->mult( 2, x0 );
	wait();
        if ( div == 0 )
        {
          x0 = 0;
          break;
        }
      	tmp1 = alu_port1->mult( x0, x0 );
	wait();
     	tmp2 = alu_port1->add( tmp1, tmp3 );
	wait();
     	x1 = alu_port1->div( tmp2, div );
	wait();
        diff = alu_port1->sub( x0, x1 );
	wait();
        if ( diff < 0 ) 
        {
          diff = alu_port1->mult( -1, diff );
	  wait();
        }
        wait();
      } while( diff > 0.00001 );
#ifndef SYNTHESIS
      cout << "sqrt(" << operand << ") = " << x0 << endl;
#endif
      outputEven.write( x0 );
      operand += 2;
      wait();
    }
  }
  
  void odd()
  {
    sc_biguint< IW - 1 > operand = 1;
    sc_fixed< 2 * W + 1, 2 * IW + 1 > diff = 0;
    sc_fixed< 2 * W, 2 * IW > div = 0;
    sc_fixed< 2 * W - IW, W > x0 = 0;
    sc_fixed< 2 * W - IW, W > x1 = 0;
    sc_fixed< 2 * W, 2 * IW > tmp1 = 0;
    sc_fixed< 2 * W + 1, 2 * IW + 1 > tmp2 = 0;
    sc_fixed< W, IW > tmp3 = 0;
  
    operandOdd.write( operand );
    outputOdd.write( x0 );

    alu_port2->reset();

    wait();  // end of reset
    
    while( true )
    {
      operandOdd.write( operand );
      diff = 0;
      x0 = operand;
      x1 = operand;
      tmp3 = operand;
      
      do 
      {
        x0 = x1;
      	div = alu_port2->mult( 2, x0 );
	wait();
        if ( div == 0 )
        {
          x0 = 0;
          break;
        }
      	tmp1 = alu_port2->mult( x0, x0 );
	wait();
      	tmp2 = alu_port2->add( tmp1, tmp3 );
	wait();
       	x1 = alu_port2->div( tmp2, div );
	wait();
	diff = alu_port2->sub( x0, x1 );
	wait();
        if ( diff < 0 ) 
        {
       	  diff = alu_port2->mult( -1, diff );
	  wait();
        }
        wait();
      } while( diff > 0.00001 );
#ifndef SYNTHESIS
      cout << "sqrt(" << operand << ") = " << x0 << endl;
#endif
      outputOdd.write( x0 );
      operand += 2;
      wait();
    }
  }
  
  SC_CTOR( Newton ) : 
    alu_port1("alu_port1"), 
    alu_port2("alu_port2")
  {
    SC_CTHREAD( even, clock.pos() );
    reset_signal_is( reset, true );

    SC_CTHREAD( odd, clock.pos() );
    reset_signal_is( reset, true );
  }
};

#endif
