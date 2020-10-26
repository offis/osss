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

#ifndef _FILTER_INCLUDE_
#define _FILTER_INCLUDE_

#include <systemc.h>

#define OSSS_GREEN

#include "osss.h"       // osss simulation library
#include "Channel.h"

//------------------------------------------------------------------------------
// declaration of modules which are connected to the channel
//------------------------------------------------------------------------------

SC_MODULE(Filter)
{
  sc_in<bool> clock;
  sc_in<bool> reset; 

  // interface to the channel
  //osss_port_to_channel< method_read_if > input;
  osss_port<osss_channel_if<method_read_if> > input;

  Filter(sc_module_name moduleName, unsigned int waitCycles=1);

 private:
  
  SC_HAS_PROCESS(Filter);
    
  void main();
     
  unsigned int m_waitCycles;  		
};

#endif
