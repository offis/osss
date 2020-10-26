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

#include "Filter.h"
#include "Image.h"

Filter::Filter(sc_module_name moduleName, unsigned int waitCycles) 
  : sc_module(moduleName),
    m_waitCycles(waitCycles)
{
  SC_CTHREAD(main, clock.pos());
  reset_signal_is(reset, true);
}

void 
Filter::main()
{
  Image i;

  // call reset on Channel transactor
  input->reset();
  
  wait(); // for resetting the system
  while(true)
  {
    cout << sc_time_stamp() << " " << name() << " filter unread ";
    i.print();
    cout << endl;
    
    // reading from the channel (calls the read method of the method_read_if
    // which is implemented in the Channel::Transactor_read class)
    input->read(i);
  
    cout << sc_time_stamp() << " " << name() << " filter read ";
    i.print();
    cout << endl;
    
    i.clear();
    wait(m_waitCycles);
  }
}
