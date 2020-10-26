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
#ifndef BRIDGE_STUB_INCLUDED
#define BRIDGE_STUB_INCLUDED

#include "Bridge_if.h"

template<>
class osss_rmi_if< Bridge_if > : public Bridge_if
{
 public:
    
  OSSS_OBJECT_STUB_CTOR(Bridge_if);

  // Master Interface
  OSSS_METHOD_VOID_STUB(write, OSSS_PARAMS(2, sc_uint<32>, addr, sc_uint<32>, data));

  OSSS_METHOD_VOID_STUB(read, OSSS_PARAMS(1, sc_uint<32>, addr));

  OSSS_METHOD_STUB(sc_uint<32>, readd, OSSS_PARAMS(0));

  // Slave Interface
  OSSS_METHOD_STUB(sc_uint<32>, getAddr, OSSS_PARAMS(0));
 
  OSSS_METHOD_VOID_STUB(putData, OSSS_PARAMS(1, sc_uint< 32 >, data));
    
  OSSS_METHOD_STUB(sc_uint< 32 >, getData, OSSS_PARAMS(0));
  
  OSSS_METHOD_STUB(ActionType, getAction, OSSS_PARAMS(0));

};


#endif
