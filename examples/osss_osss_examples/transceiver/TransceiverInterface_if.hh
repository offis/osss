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
#ifndef __TRANSCEIVERIF_IF_HH__
#define __TRANSCEIVERIF_IF_HH__

#if defined(OSSS_GREEN)
#include "osss.h"

enum ModeTypeEnum { withParity, withoutParity };
typedef osss_enum<ModeTypeEnum> ModeType;

#elif defined(OSSS_BLUE)
#include "osss.h"

enum ModeType { withParity, withoutParity };
#endif

class TransceiverInterface_if : public sc_interface
{
public: 
  // The same as blockingSend
  virtual void send( sc_biguint< 8 > byte ) = 0;

  // "Alias" for blockingReadReceiveBuffer()
  virtual sc_biguint< 8 > receive() = 0;

  // Returns the content of m_sendBuffer and sets m_sent, 
  // if m_sent == false, else blocks.
  virtual sc_biguint< 8 > getSBuff() = 0;

  // Overwrites m_receiveBuffer and sets m_received, iff m_received == false 
  // and m_receiverEnabled == true, else blocks.
  virtual void putRBuff( sc_biguint< 8 > byte ) = 0;

  // Sets m_parityBit to the passed value.
  virtual void setParityBit( bool parity ) = 0;

  // Returns m_parityBit.
  virtual bool getParityBit() = 0;

  // Sets m_receivedParityBit to the passed value.
  virtual void setReceivedParityBit( bool parity ) = 0;

  // Returns m_receivedParityBit.
  virtual bool getReceivedParityBit() = 0;

  // Clears m_receiverEnabled.
  virtual void disableReceiver() = 0;
 
  // Sets m_receiverEnabled.
  virtual void enableReceiver() = 0;

  // Sets m_baudrate to the passed value. If 0 is passed,
  // m_baudrate is set to 1.
  virtual void setBaudrate( unsigned short baudrate ) = 0;

  // Returns m_baudrate
  virtual unsigned short getBaudrate() = 0;

  // Sets m_mode to the passed value
  virtual void setMode( ModeType mode ) = 0;

  // Returns m_mode
  virtual ModeType getMode() = 0;

  // Returns m_receiverEnabled
  virtual bool receiverEnabled() = 0;

  // Returns m_receiverEnabled, if m_receiverEnabled == true,
  // else blocks
  virtual bool blockUntilReceiverEnabled() = 0;
};

#endif
