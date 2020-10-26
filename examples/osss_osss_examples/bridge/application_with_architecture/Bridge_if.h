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
 ------------------------------------------------------------------------------*/
#ifndef __BRIDGE_IF_HH___
#define __BRIDGE_IF_HH___

#define OSSS_GREEN
#include <osss.h>

enum Action {a_read, a_write, a_idle};
typedef osss_enum<Action> ActionType;

class Bridge_if : public sc_interface
{
 public:

  //Master Interface
  virtual void write(sc_uint< 32 > addr, sc_uint< 32 > data) = 0;
  
  virtual void read(sc_uint< 32 > addr) = 0;
  
  virtual sc_uint< 32 > readd() = 0;
  
  //Slave
  virtual sc_uint< 32 > getAddr() = 0;
 
  virtual void putData(sc_uint< 32 > data) = 0;
  
  virtual sc_uint< 32 > getData() = 0;
  
  virtual ActionType getAction() = 0;

};

#endif
