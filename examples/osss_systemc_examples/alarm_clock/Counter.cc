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
#include "Counter.h"
#ifndef SC_SYNTHESIS
#include <iostream>
#endif

using namespace std;

Counter::Counter( const unsigned int lowerBound,
		  const unsigned int upperBound,
		  const unsigned int initialValue )
{
#ifndef SC_SYNTHESIS
  if ( lowerBound >= upperBound )
  {
    cerr << "lowerBound greater of equal to upperBound" << endl;
  }
  if ( ! ( ( lowerBound <= initialValue ) && ( initialValue <= upperBound ) ) )
  {
    cerr << "initialValue out of bounds" << endl;
  }
#endif 
  m_counter = initialValue;
  m_lowerBound = lowerBound;
  m_upperBound = upperBound;
  m_overflow = false;
}
 
bool 
Counter::overflow() const
{
  return( m_overflow );
}

void 
Counter::clearOverflow()
{
  m_overflow = false;  
}

void 
Counter::setLowerBound( const unsigned int lowerBound )
{
  m_lowerBound = lowerBound;  
}

void 
Counter::setUpperBound( const unsigned int upperBound )
{
  m_upperBound = upperBound;
}

void 
Counter::reset()
{
  m_counter = m_lowerBound;
  m_overflow = false;
}

unsigned int 
Counter::getValue() const
{
  return( m_counter );
}

void 
Counter::count( const unsigned int stepSize )
{
  unsigned int diff = m_upperBound - m_counter;
  if ( diff < stepSize ) // counter overflow
  {
    m_counter = m_lowerBound + ( stepSize - diff ) - 1;
    m_overflow = true;
  }
  else 
  {
    m_counter += stepSize;
  }
}

