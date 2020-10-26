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

#ifndef PACKET_HH_INCLUDED
#define PACKET_HH_INCLUDED

#include <ostream>

class Packet {
 public:
  
  Packet();
    
  unsigned char get_source_addr() const;
  void          set_source_addr(const unsigned char& addr);
     
  unsigned char get_target_addr() const;
  void          set_target_addr(const unsigned char& addr);
  
  unsigned char get_payload(const unsigned int& index) const;
  void          set_payload(const unsigned int& index, 
                            const unsigned char& data);
  
  unsigned int  get_payload_size() const;
      
 protected:
  unsigned char  m_source_addr;
  unsigned char  m_target_addr;
  unsigned char  m_payload[10];
};

std::ostream & operator << (std::ostream &os, Packet const& p);

#endif
