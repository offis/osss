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
#ifndef CONSUMER_INCLUDED
#define CONSUMER_INCLUDED

#define OSSS_GREEN
#include <osss.h>

#include "Packet.hh"

OSSS_MODULE(Consumer)
{
  /* this kind of port has been used on the Application Layer (OSSS_BLUE)
    osss_port<osss_shared_if<osss_fifo_if<Packet> > > input;
  or (short form):
    osss_port_to_shared<osss_fifo_if<Packet> > input;
  */
  
  // interface to the remote object
  //osss_port<osss_rmi_if<osss_fifo_get_if<Packet> > > input;
  //osss_port_to_rmi<osss_fifo_if<Packet> > input;

  osss_fifo_in<Packet>::rmi_type  input;
  
  sc_out<Packet>  po_Packet;
  sc_out<unsigned int> po_PacketNr;
  
  void main();
  
  OSSS_CTOR(Consumer) : 
    input("input")
  {
    OSSS_CTHREAD(main, true);
  }  

};

#include "Consumer.icc"

#endif
