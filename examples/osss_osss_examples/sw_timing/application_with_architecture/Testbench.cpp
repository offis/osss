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

#include "Testbench.h"

Testbench::Testbench(sc_module_name module_name,
                     sc_time clockPeriod,
                     unsigned int numResetCylces) 
                     
                     : sc_module(module_name),
                       m_ClockPeriodHalf(clockPeriod/2.0),
                       m_NumResetCycles(numResetCylces)
{           
  SC_THREAD(generateClockAndReset);
}

sc_time
Testbench::GetClockCyclePeriod() const
{
  return 2.0*m_ClockPeriodHalf;
}

void
Testbench::generateClockAndReset()
{
  po_bReset.write(true);
  po_bClk.write(true);

  for (unsigned int i=0; i<m_NumResetCycles; ++i)
  {
    wait(m_ClockPeriodHalf);
    po_bClk.write(false);
    wait(m_ClockPeriodHalf);
    po_bClk.write(true);
  }

  wait(m_ClockPeriodHalf);
  po_bReset.write(false);
  po_bClk.write(false);
  
  while(true)
  {
    wait(m_ClockPeriodHalf);
    po_bClk.write(true);
    wait(m_ClockPeriodHalf);
    po_bClk.write(false);
  }
}
