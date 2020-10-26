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

#define OSSS_BLUE

#include "../TransceiverInterface.hh"
#include "ProdCons.hh"
#include "Transceiver.hh"

template< class Scheduler >
SC_MODULE( Top )
{
  // Ports
  sc_in< bool > clock;
  sc_in< bool > reset;
  sc_out< sc_biguint< 8 > > output1; 
  sc_out< sc_biguint< 8 > > output2; 

  sc_signal< bool > line1;
  sc_signal< bool > line2;

  Transceiver< TransceiverInterface_if > *transceiver1;
  Transceiver< TransceiverInterface_if > *transceiver2;
  
  ProdCons< TransceiverInterface_if, sc_biguint< 8 >, true > *prodCons1;
  ProdCons< TransceiverInterface_if, sc_biguint< 8 >, false > *prodCons2;

  osss_shared< TransceiverInterface, Scheduler > tif1; 
  osss_shared< TransceiverInterface, Scheduler > tif2; 

  SC_CTOR( Top )
  {
    tif1.clock_port( clock );
    tif1.reset_port( reset );

    tif2.clock_port( clock );
    tif2.reset_port( reset );

    prodCons1 = new 
      ProdCons< TransceiverInterface_if, sc_biguint< 8 >, true >( "prodcons1" );
    prodCons1->clock( clock );
    prodCons1->reset( reset );
    prodCons1->output( output1 );
    prodCons1->commIF_producer_port( tif1 );
    prodCons1->commIF_consumer_port( tif1 );

    prodCons2 = new 
      ProdCons< TransceiverInterface_if, sc_biguint< 8 >, false >( "prodcons2" );
    prodCons2->clock( clock );
    prodCons2->reset( reset );
    prodCons2->output( output2 );
    prodCons2->commIF_producer_port( tif2 );
    prodCons2->commIF_consumer_port( tif2 );

    transceiver1 = new Transceiver< TransceiverInterface_if >( "transceiver1" );
    transceiver1->clock( clock );
    transceiver1->reset( reset );
    transceiver1->TxD( line1 );
    transceiver1->RxD( line2 );
    transceiver1->tif_send_port( tif1 );
    transceiver1->tif_send_port.setPriority(10);
    transceiver1->tif_receive_port( tif1 );
    transceiver1->tif_receive_port.setPriority(100);

    transceiver2 = new Transceiver< TransceiverInterface_if >( "transceiver2" );
    transceiver2->clock( clock );
    transceiver2->reset( reset );
    transceiver2->TxD( line2 );
    transceiver2->RxD( line1 );
    transceiver2->tif_send_port( tif2 );
    transceiver2->tif_send_port.setPriority(10);
    transceiver2->tif_receive_port( tif2 );
    transceiver2->tif_receive_port.setPriority(100);
  }

};

#endif // __TOP_HH__
