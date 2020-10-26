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

#define OSSS_GREEN

#include "Producer.hh"
#include "Consumer.hh"
#include "Channel.hh"

class Top : public osss_system
{
public:

  sc_in<bool>   pi_bClk;
  sc_in<bool>   pi_bReset;

  sc_out<sc_uint<8> > po_SourceAddr;
  sc_out<sc_uint<8> > po_TargetAddr;
  sc_out<sc_uint<8> > po_Payload;
  sc_out<unsigned int> po_PacketNr;

  Producer*                                            m_Producer;
  ChannelType*                                         m_Channel1;
  Producer*                                            m_Producer1;
  ChannelType*                                         m_Channel11;
  osss_object_socket<osss_shared<osss_fifo_buf<Packet, 10> > >* m_Buffer;
  ChannelType*                                         m_Channel2;
  Consumer*                                            m_Consumer;

  sc_signal<Packet> ms_Packet;
  sc_signal<unsigned int> ms_PacketNr;

  void splitPacket();

  SC_HAS_PROCESS(Top);

  Top(sc_core::sc_module_name name) :
    osss_system(name),
    pi_bClk("pi_bClk"),
    pi_bReset("pi_bReset")
  {
    // register clock and reset port and make them global
    osss_global_port_registry::register_clock_port(pi_bClk);
    osss_global_port_registry::register_reset_port(pi_bReset);

    m_Channel1 = new ChannelType("m_Channel1");
    m_Channel11 = new ChannelType("m_Channel11");
    m_Channel2 = new ChannelType("m_Channel2");

    m_Buffer = 
      new osss_object_socket<osss_shared<osss_fifo_buf<Packet, 10> > >("m_Buffer");

    m_Buffer->bind(*m_Channel1);
    m_Buffer->bind(*m_Channel11);
    m_Buffer->bind(*m_Channel2);

    m_Producer = new Producer("m_Producer");
    m_Producer->output(*m_Channel1, *m_Buffer);

    m_Producer1 = new Producer("m_Producer1");
    m_Producer1->output(*m_Channel11, *m_Buffer);

    m_Consumer = new Consumer("m_Consumer");
    m_Consumer->po_Packet(ms_Packet);
    m_Consumer->po_PacketNr(ms_PacketNr);
    m_Consumer->input(*m_Channel2, *m_Buffer);

    SC_METHOD(splitPacket);
    sensitive << ms_PacketNr;
    dont_initialize();
  }

};

#include "Top.icc"

#endif
