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
#include <systemc.h>
#include "Top.hh"

#ifndef SC_SYNTHESIS
#include "Testbench.hh"
#endif // SC_SYNTHESIS

int sc_main(int, char**)
{
  Top top("top");

#ifndef SC_SYNTHESIS
  Testbench testbench("testbench");
  sc_signal<bool> s_clock;
  sc_signal<bool> s_reset;
  sc_signal<sc_uint<8> > s_SourceAddr;
  sc_signal<sc_uint<8> > s_TargetAddr;
  sc_signal<sc_uint<8> > s_Payload;

  top.pi_bClk(s_clock);
  top.pi_bReset(s_reset);
  top.po_SourceAddr(s_SourceAddr);
  top.po_TargetAddr(s_TargetAddr);
  top.po_Payload(s_Payload);
  
  testbench.po_bClk(s_clock);
  testbench.po_bReset(s_reset);
  testbench.pi_SourceAddr(s_SourceAddr);
  testbench.pi_TargetAddr(s_TargetAddr);
  testbench.pi_Payload(s_Payload);

  osss_start(1000*testbench.GetClockCyclePeriod());
#endif

  return 0;
}
