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

#include "Top.hh"

#if !defined(ALTERNATIVE_A) && !defined(ALTERNATIVE_B)
#define ALTERNATIVE_A
#endif

Top::Top(sc_core::sc_module_name name) : SYSTEM_BASE(name) {
    m_Channel1 = new Bus_Ch_t("m_Channel1");
    m_Channel1->clock_port(clk);
    m_Channel1->reset_port(reset);
#ifdef ALTERNATIVE_A
  m_Channel[0] = new P2P_Ch_t("m_Channel2");
  m_Channel[1] = new P2P_Ch_t("m_Channel3");
  for(unsigned int i=0; i<2; ++i) {
    m_Channel[i]->clock_port(clk);
    m_Channel[i]->reset_port(reset);
  }
#endif

  m_Buffer = new Buffer_t("m_Buffer");
  m_Buffer->clock_port(clk);
  m_Buffer->reset_port(reset);
  m_Buffer->bind(*m_Channel1);
#ifdef ALTERNATIVE_A
  m_Buffer->bind(*m_Channel[0]);
  m_Buffer->bind(*m_Channel[1]);
#endif

  m_Producer = new Producer("m_Producer");
  m_Producer->output(*m_Buffer);
    
#ifdef OSSS_INCLUDE_XILINX
  m_Processor = new xilinx_microblaze("m_Processor");
#else
  m_Processor = new osss_processor("m_Processor");
#endif
  m_Processor->clock_port(clk);
  m_Processor->reset_port(reset);
  m_Processor->rmi_client_port(*m_Channel1);
  m_Processor->add_sw_task(m_Producer);
  
  m_Consumer1 = new Consumer<1>("m_Consumer1");
  m_Consumer1->clock_port(clk);
  m_Consumer1->reset_port(reset);
#ifdef ALTERNATIVE_A
  m_Consumer1->input(*m_Channel[0], *m_Buffer);
#else // ALTERNATIVE_B
  m_Consumer1->input(*m_Channel1, *m_Buffer);
#endif

  m_Consumer2 = new Consumer<2>("m_Consumer2");
  m_Consumer2->clock_port(clk);
  m_Consumer2->reset_port(reset);
#ifdef ALTERNATIVE_A
  m_Consumer2->input(*m_Channel[1], *m_Buffer);
#else // ALTERNATIVE_B
  m_Consumer2->input(*m_Channel1, *m_Buffer);
#endif

#ifdef ALTERNATIVE_A
  std::cout << "P2P Channel size: " 
            << static_cast<unsigned int>(P2P_BITSIZE1) << ", "
            << static_cast<unsigned int>(P2P_BITSIZE2) << std::endl
            << std::endl;
#endif

}
