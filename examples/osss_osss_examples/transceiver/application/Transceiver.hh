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
#ifndef __CONSUMER_HH__
#define __CONSUMER_HH__

#include "../TransceiverInterface.hh"

template<class IfType>
SC_MODULE( Transceiver )
{
  // Ports
  sc_in_clk clock;
  sc_in< bool > reset;
  sc_in< bool > RxD;
  sc_out< bool > TxD;

  osss_port_to_shared< IfType > tif_send_port;
  osss_port_to_shared< IfType > tif_receive_port;

  // Process prototypes
  void send()
  {
    sc_biguint< 8 > byte;
    ModeType mode;
    bool dummy;
    unsigned short baudrate;
    unsigned char bitCounter = 0;
    unsigned short baudrateCounter = 0;
    bool parityBit;
    dummy = IDLE;
    TxD.write( dummy );
    wait();

    while ( true ) 
    {
      byte = tif_send_port->getSBuff();
      wait();
      mode = tif_send_port->getMode();
      wait();
      baudrate = tif_send_port->getBaudrate();
      wait();
      if ( mode == withParity )
      {
       	parityBit = tif_send_port->getParityBit();
	wait();
        dummy = START_BIT;
        TxD.write( dummy );
        wait( baudrate );
        bitCounter = 0;
        baudrateCounter = 0;
        dummy = byte[7];
        while ( true )
        {
          TxD.write( dummy );
          baudrateCounter = baudrateCounter + 1;
          
          if ( baudrateCounter == baudrate )
          {
            bitCounter = bitCounter + 1;
            baudrateCounter = 0;
            byte <<= 1;
            dummy = byte[7];
          } 
          if ( bitCounter == 8 )
          {
            break;
          }
          wait();
        }

      }
      else
      {
        dummy = START_BIT;
        TxD.write( dummy );
        wait( baudrate );
        bitCounter = 0;
        baudrateCounter = 0;
        dummy = byte[7];
        while ( true )
        {
          TxD.write( dummy );
          baudrateCounter = baudrateCounter + 1;
          
          if ( baudrateCounter == baudrate )
          {
            bitCounter = bitCounter + 1;
            baudrateCounter = 0;
            byte <<= 1;
            dummy = byte[7];
          } 
          if ( bitCounter == 8 )
          {
            break;
          }
          wait();
        }
      }

      if ( mode == withParity )
      {
        TxD.write( parityBit );
        wait( baudrate );
        dummy = STOP_BIT;
        TxD.write( dummy );
        wait( baudrate );
      }
      else
      {
        dummy = STOP_BIT;
        TxD.write( dummy );
        wait( baudrate );
      }
      wait();
    }
  }      

  void receive()
  {
    sc_biguint< 8 > byte;
    bool enabled = false;
    bool parityBit;
    ModeType mode;
    unsigned short baudrate;
    unsigned char bitCounter = 0;
    unsigned short baudrateCounter = 0;
    wait();

    while ( true ) 
    {
      while ( !( enabled && ( RxD.read() == START_BIT ) ) )
      {
       	enabled = tif_receive_port->receiverEnabled();
	wait();
        // The following wait is only necessary, until direct
        // accesses correctly automatically add a wait when
        // this is necessary
        wait();
      }
      // The following wait is only necessary, until direct
      // accesses correctly automatically add a wait when
      // this is necessary
      wait();
      mode = tif_receive_port->getMode();
      wait();
      baudrate = tif_receive_port->getBaudrate();
      wait();
      bitCounter = 0;
      baudrateCounter = 0;
      while ( true )
      {
        baudrateCounter = baudrateCounter + 1;
        
        if ( baudrateCounter == baudrate )
        {
          bitCounter = bitCounter + 1;
          baudrateCounter = 0;
          byte <<= 1;
          byte[0] = RxD.read();
        } 
        if ( bitCounter == 8 )
        {
          break;
        }
        wait();
      }

      if ( mode == withParity )
      {
        wait( baudrate );
        parityBit = RxD.read();
      	tif_receive_port->setReceivedParityBit( parityBit );
	wait();
        wait( baudrate ); // Stop Bit
	tif_receive_port->putRBuff( byte );
	wait();
      }
      else
      {
        wait( baudrate ); // Stop Bit
	tif_receive_port->putRBuff( byte );
	wait();
      }
    }
  }      

  SC_CTOR( Transceiver ) :
    tif_send_port("tif_send_port"),
    tif_receive_port("tif_receive_port")
  {
    SC_CTHREAD( send, clock.pos() );
    reset_signal_is( reset, true );

    SC_CTHREAD( receive, clock.pos() );
    reset_signal_is( reset, true );
  }

};

#endif // __TRANSCEIVER_HH__
