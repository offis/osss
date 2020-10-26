/*----------------------------------------------------------------------------------------
 Copyright (c) 2005 Kuratorium OFFIS e.V., Oldenburg, Germany
  
 Name of the Library: OSSS (A library for synthesisable system level models in SystemC) 

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
 ----------------------------------------------------------------------------------------*/
#ifndef __TRANSCEIVER_IF_HH__
#define __TRANSCEIVER_IF_HH__

#include "TransceiverInterface_if.hh"

const bool IDLE = true;
const bool START_BIT = false;
const bool STOP_BIT = true;

class TransceiverInterface : public TransceiverInterface_if
{
public:
  CLASS(TransceiverInterface, NullClass);

  // Constructor
  CONSTRUCTOR(public, TransceiverInterface, ());

  virtual ~TransceiverInterface() {}

  // The same as blockingSend
  OSSS_GUARDED_METHOD_VOID(send, 
			   OSSS_PARAMS(1, sc_biguint< 8 >, byte), 
			   m_sent);

  // "Alias" for blockingReadReceiveBuffer()
  OSSS_GUARDED_METHOD(sc_biguint< 8 >, 
		      receive, 
		      OSSS_PARAMS(0), 
		      m_received);

  // Returns the content of m_sendBuffer and sets m_sent, 
  // if m_sent == false, else blocks.
  OSSS_GUARDED_METHOD(sc_biguint< 8 >, 
		      getSBuff, 
		      OSSS_PARAMS(0), 
		      !m_sent);

  // Overwrites m_receiveBuffer and sets m_received, iff m_received == false 
  // and m_receiverEnabled == true, else blocks.
  OSSS_GUARDED_METHOD_VOID(putRBuff, 
			   OSSS_PARAMS(1, sc_biguint< 8 >,  byte), 
			   !m_received && m_receiverEnabled);

  // Sets m_parityBit to the passed value.
  OSSS_GUARDED_METHOD_VOID(setParityBit, 
			   OSSS_PARAMS(1, bool, parity), 
			   true);

  // Returns m_parityBit.
  OSSS_GUARDED_METHOD(bool, 
		      getParityBit, 
		      OSSS_PARAMS(0), 
		      true );

  // Sets m_receivedParityBit to the passed value.
  OSSS_GUARDED_METHOD_VOID(setReceivedParityBit, 
			   OSSS_PARAMS(1, bool, parity ), 
			   true);

  // Returns m_receivedParityBit.
  OSSS_GUARDED_METHOD(bool, 
		      getReceivedParityBit, 
		      OSSS_PARAMS(0), 
		      true);

  // Clears m_receiverEnabled.
  OSSS_GUARDED_METHOD_VOID(disableReceiver, 
			   OSSS_PARAMS(0), 
			   true);

  // Sets m_receiverEnabled.
  OSSS_GUARDED_METHOD_VOID(enableReceiver, 
			   OSSS_PARAMS(0), 
			   true);

  // Sets m_baudrate to the passed value. If 0 is passed,
  // m_baudrate is set to 1.
  OSSS_GUARDED_METHOD_VOID(setBaudrate, 
			   OSSS_PARAMS(1, unsigned short, baudrate), 
			   true);

  // Returns m_baudrate
  OSSS_GUARDED_METHOD(unsigned short, 
		      getBaudrate, 
		      OSSS_PARAMS(0), 
		      true);

  // Sets m_mode to the passed value
  OSSS_GUARDED_METHOD_VOID(setMode, 
			   OSSS_PARAMS(1, ModeType, mode), 
			   true);

  // Returns m_mode
  OSSS_GUARDED_METHOD(ModeType, 
		      getMode, 
		      OSSS_PARAMS(0), 
		      true);

  // Returns m_receiverEnabled
  OSSS_GUARDED_METHOD(bool, 
		      receiverEnabled, 
		      OSSS_PARAMS(0), 
		      true);

  // Returns m_receiverEnabled, if m_receiverEnabled == true,
  // else blocks
  OSSS_GUARDED_METHOD(bool, 
		      blockUntilReceiverEnabled, 
		      OSSS_PARAMS(0), 
		      m_receiverEnabled);

protected:
  // Clears m_received
  void
  clearReceived();

  // Sets m_received to m_receiverEnabled.
  void
  setReceived();
  
  // Clears m_sent
  void
  clearSent();

  // Sets m_sent to true
  void
  setSent();

  // Operation mode:
  unsigned short m_baudrate;
  ModeType m_mode;

  sc_biguint< 8 > m_sendBuffer;
  bool m_parityBit;
  sc_biguint< 8 > m_receiveBuffer;
  bool m_receivedParityBit;

  // Flags:
  bool m_receiverEnabled;
  bool m_received;
  bool m_sent;
};

#include "TransceiverInterface.icc"

#endif // __TRANSCEIVER_IF_HH__
