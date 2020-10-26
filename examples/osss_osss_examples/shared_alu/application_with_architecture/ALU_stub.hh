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

#ifndef __ALU_stub__
#define __ALU_stub__

#include "ALU_if.hh"

template< unsigned int W, unsigned int IW>
class osss_rmi_if<ALU_if<W, IW > > : public ALU_if<W, IW>
{
public:

  typedef ALU_if<W, IW> base_type;

  typedef typename base_type::multReturnType   multReturnType;
  typedef typename base_type::addSubReturnType addSubReturnType;
  typedef typename base_type::divReturnType divReturnType;

  typedef typename base_type::operandType operandType;

  OSSS_OBJECT_STUB_CTOR(base_type);

  OSSS_METHOD_STUB( addSubReturnType,
                    add, 
                    OSSS_PARAMS(2, operandType, op1, 
                                   operandType, op2 ) );

  OSSS_METHOD_STUB( addSubReturnType,
                    sub,
                    OSSS_PARAMS(2, operandType, op1, 
                                   operandType, op2 ) );

  OSSS_METHOD_STUB( multReturnType,
                    mult,
                    OSSS_PARAMS(2, operandType, multiplicand, 
                                   operandType, multiplier) );

  OSSS_METHOD_STUB( divReturnType,
                    div,
                    OSSS_PARAMS(2, operandType, dividend, 
                                   operandType, divisor ) );

};

#endif // __ALU_stub__
