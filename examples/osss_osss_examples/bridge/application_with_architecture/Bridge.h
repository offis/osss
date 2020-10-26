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
#ifndef __BRIDGE_HH___
#define __BRIDGE_HH___

#include "Bridge_if.h"

enum State {s_read, s_write, s_idle};

class Bridge : public Bridge_if
{
 public:

  CLASS(Bridge, NullClass);

  // Default constructor
  CONSTRUCTOR(public, Bridge, ());

  virtual ~Bridge() {}
  
  //Master Interface
  OSSS_GUARDED_METHOD_VOID( write, 
                            OSSS_PARAMS(2, sc_uint< 32 >, addr, 
					   sc_uint< 32 >, data),
                            b_st == s_idle );
  
  OSSS_GUARDED_METHOD_VOID( read, 
                            OSSS_PARAMS(1, sc_uint< 32 >, addr), 
			    b_st == s_idle );
  
  OSSS_GUARDED_METHOD( sc_uint< 32 >, 
	               readd, 
		       OSSS_PARAMS(0),
		       ((b_st == s_read) && b_dataready) );
  
  //Slave Interface
  OSSS_GUARDED_METHOD( sc_uint< 32 >, 
		       getAddr, 
		       OSSS_PARAMS(0),
		       b_st != s_idle );
 
  OSSS_GUARDED_METHOD_VOID( putData,
	                    OSSS_PARAMS(1, sc_uint< 32 >, data), 
			    b_st == s_read );
  
  OSSS_GUARDED_METHOD( sc_uint< 32 >, 
		       getData, 
		       OSSS_PARAMS(0),
		       b_st == s_write );
  
  OSSS_GUARDED_METHOD( ActionType, 
		       getAction,
		       OSSS_PARAMS(0),
		       true );
  
 protected:
  
  bool b_dataready;
  bool m_master_action;
  
  osss_enum<State> b_st;
  
  sc_uint< 32 > b_addr_buffer;
  sc_uint< 32 > b_data_buffer;
  
};

#include "Bridge.icc"

#endif //__BRIDGE_HH___
