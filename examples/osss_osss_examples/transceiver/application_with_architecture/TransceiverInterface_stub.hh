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
#ifndef __TRANSCEIVERIF_STUB_HH__
#define __TRANSCEIVERIF_STUB_HH__

#include "../TransceiverInterface_if.hh"

template<>
class osss_rmi_if<TransceiverInterface_if> : public TransceiverInterface_if
{
public:

  OSSS_OBJECT_STUB_CTOR( TransceiverInterface_if );

  OSSS_METHOD_VOID_STUB(Clear, OSSS_PARAMS(0));

  // The same as blockingSend
  OSSS_METHOD_VOID_STUB(send, OSSS_PARAMS(1, sc_biguint< 8 >, byte));

  // "Alias" for blockingReadReceiveBuffer()
  OSSS_METHOD_STUB(sc_biguint< 8 >, receive, OSSS_PARAMS(0));

  // Returns the content of m_sendBuffer and sets m_sent, 
  // if m_sent == false, else blocks.
  OSSS_METHOD_STUB(sc_biguint< 8 >, getSBuff, OSSS_PARAMS(0));

  // Overwrites m_receiveBuffer and sets m_received, iff m_received == false 
  // and m_receiverEnabled == true, else blocks.
  OSSS_METHOD_VOID_STUB(putRBuff, OSSS_PARAMS(1, sc_biguint< 8 >,  byte));

  // Sets m_parityBit to the passed value.
  OSSS_METHOD_VOID_STUB(setParityBit, OSSS_PARAMS(1, bool, parity));

  // Returns m_parityBit.
  OSSS_METHOD_STUB(bool, getParityBit, OSSS_PARAMS(0));

  // Sets m_receivedParityBit to the passed value.
  OSSS_METHOD_VOID_STUB(setReceivedParityBit, OSSS_PARAMS(1, bool, parity ));

  // Returns m_receivedParityBit.
  OSSS_METHOD_STUB(bool, getReceivedParityBit, OSSS_PARAMS(0));

  // Clears m_receiverEnabled.
  OSSS_METHOD_VOID_STUB(disableReceiver, OSSS_PARAMS(0));

  // Sets m_receiverEnabled.
  OSSS_METHOD_VOID_STUB(enableReceiver, OSSS_PARAMS(0));

  // Sets m_baudrate to the passed value. If 0 is passed,
  // m_baudrate is set to 1.
  OSSS_METHOD_VOID_STUB(setBaudrate, OSSS_PARAMS(1, unsigned short, baudrate));

  // Returns m_baudrate
  OSSS_METHOD_STUB(unsigned short, getBaudrate, OSSS_PARAMS(0));

  // Sets m_mode to the passed value
  OSSS_METHOD_VOID_STUB(setMode, OSSS_PARAMS(1, ModeType, mode));

  // Returns m_mode
  OSSS_METHOD_STUB(ModeType, getMode, OSSS_PARAMS(0));

  // Returns m_receiverEnabled
  OSSS_METHOD_STUB(bool, receiverEnabled, OSSS_PARAMS(0));

  // Returns m_receiverEnabled, if m_receiverEnabled == true,
  // else blocks
  OSSS_METHOD_STUB(bool, blockUntilReceiverEnabled, OSSS_PARAMS(0));

};

#endif
