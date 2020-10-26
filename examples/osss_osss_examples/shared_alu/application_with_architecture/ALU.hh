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
// File: ALU.hh                                           //
// Defines class ALU. ALU provides implementations for    //
// operatins add(), sub(), mult() and div(). add(), sub() //
// and mult() are performed with full precision. The      //
// decimal precison is peformed with full precision  for  //
// the integer part, and W - IW bits for the integer      //
// part.                                                  //
////////////////////////////////////////////////////////////

#ifndef __ALU__
#define __ALU__

#include "ALU_stub.hh"

// W = wordlength, IW = integer wordlength
// decimal wordlength = W - IW
template< unsigned int W, unsigned int IW >
class ALU : public ALU_if<W, IW>
{
public:
  
  typedef ALU_if<W, IW> base_type;
  
  typedef typename base_type::multReturnType   multReturnType;
  typedef typename base_type::addSubReturnType addSubReturnType;
  typedef typename base_type::divReturnType    divReturnType;

  typedef typename base_type::operandType operandType;

  CLASS(ALU, NullClass);

  // Default constructor
  CONSTRUCTOR(public, ALU, ()) {}
 
  OSSS_GUARDED_METHOD(addSubReturnType,
		      add,
		      OSSS_PARAMS(2, operandType, op1, operandType, op2 ),
		      true)
  {
    return( op1 + op2 );
  }

  OSSS_GUARDED_METHOD(addSubReturnType,
		      sub,
		      OSSS_PARAMS(2, operandType, op1, operandType, op2 ),
		      true)
  {
    return( op1 - op2 );
  }

  OSSS_GUARDED_METHOD(multReturnType,
		      mult,
		      OSSS_PARAMS(2, operandType, multiplicand, 
				     operandType, multiplier),
		      true)
  {
    sc_fixed< 2 * W, W + IW > result = 0;
    for ( int i = W - 1; i >= 0; i-- )
    {
      result = result << 1;
      if( multiplicand[i] != 0 )
      {
        result += multiplier;
      }
      wait();
    }
    return( result >> W - IW );
  }

  OSSS_GUARDED_METHOD(divReturnType,
		      div,
		      OSSS_PARAMS(2, operandType, dividend, 
				  operandType, divisor),
		      true)
  {
    sc_fixed< 2 * W, W + IW > tmp_remainder;
    sc_fixed< 2 * W, W + IW > tmp_divisor;
    sc_fixed< 2 * W - IW, W > result = 0;

    tmp_remainder = dividend;
    tmp_divisor = divisor;
    tmp_divisor = tmp_divisor << W;  
    for( int i = 2 * W - 1 - IW; i >= 0; i-- )
    {
      tmp_remainder = tmp_remainder << 1;
      result = result << 1;
      if ( tmp_remainder >= tmp_divisor )
      {
        tmp_remainder -= tmp_divisor;
        result[0] = 1;
      }
      else
      {
        result[0] = 0;        
      }
      wait();
    }
    return ( result );
  }
};

#endif // __ALU__
