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

#ifndef __ALU_if__
#define __ALU_if__

template< unsigned int W, unsigned int IW>
class ALU_if : public sc_interface
{
public:
  typedef sc_fixed< 2 * W, 2 * IW > multReturnType;
  typedef sc_fixed< W + 1, IW + 1 > addSubReturnType;
  typedef sc_fixed< 2 * W - IW, W > divReturnType;

  typedef sc_fixed< W, IW > operandType;

  virtual
  addSubReturnType 
  add( operandType op1, operandType op2 ) = 0;

  virtual
  addSubReturnType 
  sub( operandType op1, operandType op2 ) = 0;

  virtual
  multReturnType 
  mult( operandType multiplicand, operandType multiplier ) = 0;

  virtual
  divReturnType
  div( operandType dividend, operandType divisor ) = 0;

};

#endif // __ALU_if__
