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
#ifndef TOP_INCLUDED
#define TOP_INCLUDED

#include <systemc.h>

#define OSSS_BLUE

#include "Packet.hh"
#include "Producer.hh"
#include "Consumer.hh"


SC_MODULE(Top)
{
  sc_in<bool>   pi_bClk;
  sc_in<bool>   pi_bReset;
  
  sc_out<sc_uint<8> > po_SourceAddr;
  sc_out<sc_uint<8> > po_TargetAddr;
  sc_out<sc_uint<8> > po_Payload;
  
  Producer                                         *m_Producer;

  // osss_shared<osss_fifo_buf<Packet, 10>, osss_no_scheduler > m_Buffer;
  osss_shared<osss_fifo_buf<Packet, 10> /* , osss_no_scheduler */ > m_Buffer;
  //osss_shared_fifo< Packet, 10 >                    m_Buffer;

  Consumer                                         *m_Consumer;

  sc_signal<Packet>               ms_Packet;

  void splitPacket();
  
  SC_CTOR(Top) :
    m_Buffer("m_Buffer")
  {
    m_Buffer.clock_port(pi_bClk);
    m_Buffer.reset_port(pi_bReset);
    m_Buffer.reset_sensitivity(OSSS_POS);

    m_Producer = new Producer("m_Producer");
    m_Producer->pi_bClk(pi_bClk);
    m_Producer->pi_bReset(pi_bReset);
    m_Producer->output(m_Buffer);
    
    m_Consumer = new Consumer("m_Consumer");
    m_Consumer->pi_bClk(pi_bClk);
    m_Consumer->pi_bReset(pi_bReset);
    m_Consumer->po_Packet(ms_Packet);
    m_Consumer->input(m_Buffer);
    
    SC_METHOD(splitPacket);
    sensitive << ms_Packet;
  }
  
};

#include "Top.icc"

#endif
