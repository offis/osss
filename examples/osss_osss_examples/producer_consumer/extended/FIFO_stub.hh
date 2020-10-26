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

#ifndef FIFO_STUB_INCLUDED_HH
#define FIFO_STUB_INCLUDED_HH

#include <osss.h>

#include "FIFO_if.hh"

template<class ItemType>
class osss_rmi_if<FIFO_put_if<ItemType> > : public FIFO_put_if<ItemType>
{
public:
  typedef FIFO_put_if<ItemType> base_type;
  OSSS_OBJECT_STUB_CTOR(base_type);

  // Store an item in the FIFO if it is not full
  OSSS_METHOD_VOID_STUB(put, OSSS_PARAMS(1, ItemType, item));

  // Check whether the FIFO is empty
  OSSS_METHOD_STUB(bool, is_empty, OSSS_PARAMS(0));

  // Check whether the FIFO is full
  OSSS_METHOD_STUB(bool, is_full, OSSS_PARAMS(0));
};

template<class ItemType>
class osss_rmi_if<FIFO_get_if<ItemType> > : public FIFO_get_if<ItemType>
{
public:
  typedef FIFO_get_if<ItemType> base_type;
  OSSS_OBJECT_STUB_CTOR(base_type);

  // Get the head item if the FIFO is not empty
  OSSS_METHOD_STUB(ItemType, get, OSSS_PARAMS(0));

  // Check whether the FIFO is empty
  OSSS_METHOD_STUB(bool, is_empty, OSSS_PARAMS(0));

  // Check whether the FIFO is full
  OSSS_METHOD_STUB(bool, is_full, OSSS_PARAMS(0));
};

#endif
