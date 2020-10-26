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

#include "Producer.hh"

Producer::Producer(sc_core::sc_module_name name) : 
  output("output") {}

void 
Producer::main() {
  const unsigned char source_addr = 42;
  unsigned char target_addr = 0;
  unsigned int offset = 0;
  Packet p;

  output->reset();
  
  while(true) {
#ifndef PERFORM_ANALYSIS
    OSSS_RET(sc_time(2000.0, SC_NS)) {
      OSSS_EET(sc_time(50.0, SC_NS)) {
#endif
          p.set_source_addr(source_addr);
          p.set_target_addr(target_addr);
          for(unsigned int i = 0; i<p.get_payload_size(); ++i)
            p.set_payload(i, i+offset);
#ifndef PERFORM_ANALYSIS
      }
#endif
      output->put(p);
      if (target_addr >= 10) {
#ifndef PERFORM_ANALYSIS
        OSSS_EET(sc_time(5.0, SC_NS)) {
#endif
            target_addr = 0;
            offset = 0;
#ifndef PERFORM_ANALYSIS
        }
#endif
      }
      else {
#ifndef PERFORM_ANALYSIS
        OSSS_EET(sc_time(20.0, SC_NS)) {
#endif
            target_addr += 1;
            offset += 10;
            p.set_target_addr(target_addr);
#ifndef PERFORM_ANALYSIS
        }
#endif
        output->put(p);
      }
#ifndef PERFORM_ANALYSIS
    }
#endif
  }
}


