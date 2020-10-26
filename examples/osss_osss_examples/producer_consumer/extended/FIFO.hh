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

#ifndef FIFO_INCLUDED_HH
#define FIFO_INCLUDED_HH

#include "FIFO_if.hh"

template<class ItemType, FIFO_size_t Size>
class FIFO : public FIFO_put_if<ItemType>,
             public FIFO_get_if<ItemType>
{
  public:
    CLASS(FIFO, NullClass);

    // Default constructor -- creates an empty FIFO
    CONSTRUCTOR(public, FIFO, ());

    virtual ~FIFO() {}

    // Store an item in the FIFO if it is not full
    OSSS_GUARDED_METHOD_VOID(put, OSSS_PARAMS(1, ItemType, item), 
			     !OSSS_EXPORTED(is_full()) );

    // Get the head item if the FIFO is not empty
    OSSS_GUARDED_METHOD(ItemType, get, OSSS_PARAMS(0), 
			!OSSS_EXPORTED(is_empty()) );

    // Check whether the FIFO is empty
    OSSS_GUARDED_METHOD(bool, is_empty, OSSS_PARAMS(0), true);

    // Check whether the FIFO is full
    OSSS_GUARDED_METHOD(bool, is_full, OSSS_PARAMS(0), true);

  private:
  
    // Increment index with wrap-around
    static void increment_index(FIFO_size_t &index);

    // Decrement index with wrap-around
    static void decrement_index(FIFO_size_t &index);

    // Buffer containing the items
    #if defined(OSSS_GREEN)
      osss_memory<osss_array<ItemType, Size>, 32, 5>  m_Buffer;
      //xilinx_blockram<osss_array<ItemType, Size>, 32, 9>  m_Buffer;
    #else
      osss_array<ItemType, Size> m_Buffer;
    #endif

    // Index where to store the next item
    FIFO_size_t   m_PutIndex;
    // Index where to get the next item
    FIFO_size_t   m_GetIndex;
    // Number of items in the FIFO
    FIFO_size_t   m_NumItems;
};

#include "FIFO.icc"

#endif
