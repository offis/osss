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
#ifndef __TOP_HH__
#define __TOP_HH__

#define OSSS_GREEN

#define TRANSCEIVER_DIRECT_CONNECTION_MODE

#include "../TransceiverInterface.hh"
#include "ProdCons.hh"
#include "Transceiver.hh"
#include "Channel.hh"

template< class Scheduler >
class Top : public osss_system
{
public:

  // Ports
  sc_in< bool > clock;
  sc_in< bool > reset;
  sc_out< sc_biguint< 8 > > output1; 
  sc_out< sc_biguint< 8 > > output2; 

  sc_signal< bool > line1;
  sc_signal< bool > line2;

  ChannelType* prod1_prodPortCh;
  ChannelType* prod1_consPortCh;

  ChannelType* prod2_prodPortCh;
  ChannelType* prod2_consPortCh;

#ifndef TRANSCEIVER_DIRECT_CONNECTION_MODE
  ChannelType* trans1_sendPortCh;
  ChannelType* trans1_recvPortCh;

  ChannelType* trans2_sendPortCh;
  ChannelType* trans2_recvPortCh;
#endif
 
  Transceiver *transceiver1;
  Transceiver *transceiver2;

  ProdCons< TransceiverInterface_if, sc_biguint< 8 >, true > *prodCons1;
  ProdCons< TransceiverInterface_if, sc_biguint< 8 >, false > *prodCons2;

  osss_object_socket<osss_shared< TransceiverInterface, Scheduler > >* tif1; 
  osss_object_socket<osss_shared< TransceiverInterface, Scheduler > >* tif2; 

  Top(sc_core::sc_module_name name) :
    osss_system(name)
  {
    prod1_prodPortCh = new ChannelType("prod1_prodPortCh");
    prod1_prodPortCh->clock_port(clock);
    prod1_prodPortCh->reset_port(reset);

    prod1_consPortCh = new ChannelType("prod1_consPortCh");
    prod1_consPortCh->clock_port(clock);
    prod1_consPortCh->reset_port(reset);

#ifndef TRANSCEIVER_DIRECT_CONNECTION_MODE
    trans1_sendPortCh = new ChannelType("trans1_sendPortCh");
    trans1_sendPortCh->clock_port(clock);
    trans1_sendPortCh->reset_port(reset);

    trans1_recvPortCh = new ChannelType("trans1_recvPortCh");
    trans1_recvPortCh->clock_port(clock);
    trans1_recvPortCh->reset_port(reset);
#endif

    tif1 = new osss_object_socket<
      osss_shared< TransceiverInterface, Scheduler > >("tif1"); 
    tif1->clock_port(clock);
    tif1->reset_port(reset);
    tif1->bind(*prod1_prodPortCh);
    tif1->bind(*prod1_consPortCh);
#ifndef TRANSCEIVER_DIRECT_CONNECTION_MODE
    tif1->bind(*trans1_sendPortCh);
    tif1->bind(*trans1_recvPortCh);
#endif
      
    //--------------------------------------------------------------------------

    prod2_prodPortCh = new ChannelType("prod2_prodPortCh");
    prod2_prodPortCh->clock_port(clock);
    prod2_prodPortCh->reset_port(reset);

    prod2_consPortCh = new ChannelType("prod2_consPortCh");
    prod2_consPortCh->clock_port(clock);
    prod2_consPortCh->reset_port(reset);

#ifndef TRANSCEIVER_DIRECT_CONNECTION_MODE
    trans2_sendPortCh = new ChannelType("trans2_sendPortCh");
    trans2_sendPortCh->clock_port(clock);
    trans2_sendPortCh->reset_port(reset);

    trans2_recvPortCh = new ChannelType("trans2_recvPortCh");
    trans2_recvPortCh->clock_port(clock);
    trans2_recvPortCh->reset_port(reset);
#endif

    tif2 = new osss_object_socket<
      osss_shared< TransceiverInterface, Scheduler > >("tif2");
    tif2->clock_port(clock);
    tif2->reset_port(reset);
    tif2->bind(*prod2_prodPortCh);
    tif2->bind(*prod2_consPortCh);
#ifndef TRANSCEIVER_DIRECT_CONNECTION_MODE
    tif2->bind(*trans2_sendPortCh);
    tif2->bind(*trans2_recvPortCh); 
#endif
    
    //--------------------------------------------------------------------------

    prodCons1 = new ProdCons< 
      TransceiverInterface_if, sc_biguint< 8 >, true >( "prodCons1" );
    prodCons1->clock( clock );
    prodCons1->reset( reset );
    prodCons1->output( output1 );
    prodCons1->commIF_producer_port( *prod1_prodPortCh, *tif1 );
    prodCons1->commIF_consumer_port( *prod1_consPortCh, *tif1 );

    prodCons2 = new ProdCons< 
      TransceiverInterface_if, sc_biguint< 8 >, false >( "prodCons2" );
    prodCons2->clock( clock );
    prodCons2->reset( reset );
    prodCons2->output( output2 );
    prodCons2->commIF_producer_port( *prod2_prodPortCh, *tif2 );
    prodCons2->commIF_consumer_port( *prod2_consPortCh, *tif2 );

    transceiver1 = new Transceiver( "transceiver1" );
    transceiver1->clock( clock );
    transceiver1->reset( reset );
    transceiver1->TxD( line1 );
    transceiver1->RxD( line2 );
#ifdef TRANSCEIVER_DIRECT_CONNECTION_MODE
    transceiver1->tif_send_port( tif1->plugged_object() );
#else
    transceiver1->tif_send_port( *trans1_sendPortCh, *tif1 );
#endif
    transceiver1->tif_send_port.setPriority(10);
#ifdef TRANSCEIVER_DIRECT_CONNECTION_MODE
    transceiver1->tif_receive_port( tif1->plugged_object() );
#else
    transceiver1->tif_receive_port( *trans1_recvPortCh, *tif1 );
#endif
    transceiver1->tif_receive_port.setPriority(100);

    transceiver2 = new Transceiver( "transceiver2" );
    transceiver2->clock( clock );
    transceiver2->reset( reset );
    transceiver2->TxD( line2 );
    transceiver2->RxD( line1 );
#ifdef TRANSCEIVER_DIRECT_CONNECTION_MODE
    transceiver2->tif_send_port( tif2->plugged_object() );
#else
    transceiver2->tif_send_port( *trans2_sendPortCh, *tif2 );
#endif
    transceiver2->tif_send_port.setPriority(10);
#ifdef TRANSCEIVER_DIRECT_CONNECTION_MODE
    transceiver2->tif_receive_port( tif2->plugged_object() );
#else
    transceiver2->tif_receive_port( *trans2_recvPortCh, *tif2 );
#endif
    transceiver2->tif_receive_port.setPriority(100);
  }

};

#endif // __TOP_HH__
