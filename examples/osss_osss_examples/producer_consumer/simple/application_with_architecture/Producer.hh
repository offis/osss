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
#ifndef PRODUCER_INCLUDED
#define PRODUCER_INCLUDED

#define OSSS_GREEN
#include <osss.h>

#include "Packet.hh"

OSSS_MODULE(Producer)
{
  // port to the remote Shared Object
  // osss_port<osss_rmi_if< osss_fifo_put_if<Packet> > > output;
  // or (short form):
  //osss_port_to_rmi<osss_fifo_if<Packet> > output;
  osss_fifo_out<Packet>::rmi_type output;
  
  void main();

  OSSS_CTOR(Producer) : 
    output("output")
  {
    OSSS_CTHREAD(main, true);
  }
};

#include "Producer.icc"

#endif
