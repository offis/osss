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
#ifndef PACKET_INCLUDED
#define PACKET_INCLUDED

#define OSSS_GREEN
#include "osss.h"

#include <iosfwd>

class Packet : public osss_serialisable_object
{
public:

  OSSS_IS_SERIALISABLE( Packet );
 
  // default constructor:
  // Packet();
  OSSS_SERIALISABLE_CTOR(Packet, ());

  // special constructor:
  // Packet(sc_uint<8> sourceAddr, 
  //	    sc_uint<8> targetAddr,
  //	    sc_uint<8> payload);
  OSSS_SERIALISABLE_CTOR(Packet, (sc_uint<8> sourceAddr,
		                  sc_uint<8> targetAddr,
			          sc_uint<8> payload));

  // copy constructor:
  // Packet(const Packet &pkt);
  OSSS_SERIALISABLE_CTOR(Packet, (const Packet &pkt));
 
  virtual void serialise();

  virtual void deserialise();

  //---------------------------------------------------------------//
  
  sc_uint<8>  GetSourceAddr() const;
  void        SetSourceAddr(sc_uint<8> sourceAddr);
    
  sc_uint<8>  GetTargetAddr() const;
  void        SetTargetAddr(sc_uint<8> targetAddr);

  sc_uint<8>  GetPayload() const;
  void        SetPayload(sc_uint<8> payload);
    
  bool        operator == (Packet const& p) const;
  
private:
  
  sc_uint<8>  m_SourceAddr;
  sc_uint<8>  m_TargetAddr;
    
  sc_uint<8>  m_Payload;
};

std::ostream & operator << (std::ostream &os, Packet const& p);

namespace sc_core {

#if SYSTEMC_VERSION <= 20020405
void sc_trace(sc_trace_file *traceFile, 
	      Packet const& p, 
	      sc_string const& name);
#else
void sc_trace(sc_trace_file *traceFile, 
	      Packet const& p, 
	      std::string const& name);
#endif

} // end namespace sc_core

#include "Packet.icc"

#endif
