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

#include "Packet.hh"
  
Packet::Packet() : 
  m_source_addr(0), 
  m_target_addr(0) {
  for(unsigned int i=0; i<10; ++i)
    m_payload[i] = 0;
}
    
unsigned char 
Packet::get_source_addr() const {
  return m_source_addr;
}
    
void 
Packet::set_source_addr(const unsigned char& addr) {
  m_source_addr = addr;
}
    
unsigned char 
Packet::get_target_addr() const {
  return m_target_addr;
}

void 
Packet::set_target_addr(const unsigned char& addr) {
  m_target_addr = addr;
}
    
unsigned char 
Packet::get_payload(const unsigned int& index) const {
  return m_payload[index];
}
    
void 
Packet::set_payload(const unsigned int& index, 
                    const unsigned char& data) {
  m_payload[index] = data;
}

unsigned int 
Packet::get_payload_size() const {
  return 10;
}

std::ostream & operator << (std::ostream &os, Packet const& p)
{
#ifndef SC_SYNTHESIS
  os << "SourceAddr : " << static_cast<unsigned int>(p.get_source_addr())
     << " TargetAddr : " << static_cast<unsigned int>(p.get_target_addr())
     << " Payload    : ";
  for(unsigned int i=0; i<p.get_payload_size(); ++i) 
  {
    os << static_cast<unsigned int>(p.get_payload(i)) << " ";
  }
#endif // SC_SYNTHESIS
  return os;
}
