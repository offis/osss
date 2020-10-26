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

#ifndef TESTBENCH_INCLUDED
#define TESTBENCH_INCLUDED

#include <systemc.h>

SC_MODULE(Testbench)
{
  sc_out<bool>    po_bClk;  //po_b -> port out bool
  sc_out<bool>    po_bReset;
  
  Testbench(sc_module_name moduleName,
            sc_time clockPeriod=sc_time(10.0, SC_NS),
            unsigned int numResetCylces=10);

  sc_time  GetClockCyclePeriod() const;

 private:

  SC_HAS_PROCESS(Testbench);

  void generateClockAndReset();  

  sc_time       m_ClockPeriodHalf;
  unsigned int  m_NumResetCycles;

};

#endif
