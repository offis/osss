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
#ifndef __PRODCONS_HH__
#define __PRODCONS_HH__

#include "../TransceiverInterface.hh"

template< class InterfaceType, class OutputType, bool EVEN >
SC_MODULE( ProdCons )
{
  // Ports
  sc_in_clk clock;
  sc_in< bool > reset;
  sc_out< OutputType > output; 

  osss_port_to_shared< InterfaceType > commIF_producer_port; 
  osss_port_to_shared< InterfaceType > commIF_consumer_port;

  // Process prototypes
  void producer()
  {
    OutputType byte;
    if ( EVEN )
    {
      byte = 0;
    }
    else
    {
      byte = 1;
    }
    wait();

    while ( true )
    {
      commIF_producer_port->send( byte );
      wait();
      byte += 2;
    }
  }


  // Process prototypes
  void consumer()
  {
    output.write( 0 );
    wait();
   
    while ( true )
    {
      output = commIF_consumer_port->receive();
      wait();
      wait();
    }
  }

  SC_CTOR( ProdCons ) :
    commIF_producer_port("commIF_producer_port"),
    commIF_consumer_port("commIF_consumer_port")
  {
    SC_CTHREAD( producer, clock.pos() );
    reset_signal_is( reset, true );

    SC_CTHREAD( consumer, clock.pos() );
    reset_signal_is( reset, true );
  }

};

#endif // __PRODCONS_HH__
