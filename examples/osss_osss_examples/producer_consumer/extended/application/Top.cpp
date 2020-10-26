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

Top::Top(sc_core::sc_module_name name) {
  m_Buffer = new Buffer_t("m_Buffer");
  m_Buffer->clock_port(clk);
  m_Buffer->reset_port(reset);

  m_Producer = new Producer("m_Producer");
  m_Producer->clock_port(clk);
  m_Producer->reset_port(reset);
  m_Producer->output(*m_Buffer);
    
  m_Consumer1 = new Consumer<1>("m_Consumer1");
  m_Consumer1->clk(clk);
  m_Consumer1->reset(reset);
  m_Consumer1->input(*m_Buffer);

  m_Consumer2 = new Consumer<2>("m_Consumer2");
  m_Consumer2->clk(clk);
  m_Consumer2->reset(reset);
  m_Consumer2->input(*m_Buffer);
}


