/* -----------------------------------------------------------------------
 * Copyright (c) 2005-2008  OFFIS Institute for Information Technology,
 *                          Carl von Ossietzky University,
 *                          Oldenburg, Germany
 * All rights reserved.
 *
 * This file is directly or indirectly part of the OSSS simulation
 * library, a library for synthesisable system level models in
 * SystemC.
 *
 * Created for the projects:
 *   - ICODES  (1)
 *   - PolyDyn (2)
 *   - ANDRES  (3)
 *
 * 1) http://icodes.offis.de/
 * 2) http://ehs.informatik.uni-oldenburg.de/en/research/projects/polydyn/
 * 3) http://andres.offis.de/
 *
 * A list of authors and contributors can be found in the accompanying
 * AUTHORS file.  For detailed copyright information, please refer
 * to the COPYING file.
 *
 * -----------------------------------------------------------------------
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301,
 * USA.
 *
 * Contact information:
 *  OFFIS
 *  Institute for Information Technology
 *    Escherweg 2
 *    D-26121 Oldenburg
 *    Germany
 *  www  : http://offis.de/
 *  phone: +49 (441) 9722-0
 *  fax  : +49 (441) 9722-102
 * -----------------------------------------------------------------------
 */

#ifndef OSSS_CIRC_BUFFER_H_INCLUDED
#define OSSS_CIRC_BUFFER_H_INCLUDED

#include "osss_array.h"
#include <cstddef> // size_t

namespace osss {
OSSS_SYNTHESISABLE_BEGIN_

template< typename T, size_t Size = 1 >
class osss_circ_buffer;

template< typename T, size_t Size >
class osss_circ_buffer
{
  typedef osss_circ_buffer<T,Size> this_type;
public:

  typedef T      item_type;
  typedef size_t index_type;
  typedef size_t size_type;

  static const size_type size = Size;

  osss_circ_buffer()
    : buf_()
    , put_idx_(0)
    , get_idx_(0)
    , num_items_(0)
  {}

  void put( item_type item )
  {
    buf_[put_idx_] = item;
    if( num_items_ < size ) {
      inc_index( put_idx_ );
      ++num_items_;
    }
  }

  item_type get() {
    item_type result = buf_[ get_idx_ ];
    if( num_items_ > 0 ) {
      inc_index( get_idx_ );
      --num_items_;
    }
    return result;
  }

 bool full()  const { return num_items_ == size; }
 bool empty() const { return num_items_ ==    0; }

private:
  static void inc_index( index_type& idx ) {
    idx = ( idx == size - 1 ) ? 0 : idx + 1;
  }

  /// element storage
//  osss_array<item_type, size> buf_;
  item_type buf_[size];

  /// Index where to store the next item
  index_type put_idx_;
  /// Index where to get the next item
  index_type get_idx_;
  /// Number of items in the buffer
  size_type  num_items_;

}; // class osss_fifo_buf<T,Size>


// --------------- partial specialization for size == 1 ------------------

template< typename T >
class osss_circ_buffer<T,1>
{
  typedef osss_circ_buffer<T,1> this_type;
public:

  typedef T      item_type;
  typedef bool   size_type;

  static const size_type size = 1;

  osss_circ_buffer()
    : buf_()
    , num_items_(false)
  {}

  void put( item_type item ) {
    buf_       = item;
    num_items_ = true;
  }

  item_type get() {
    num_items_ = false;
    return buf_;
  }

  bool full()  const { return num_items_; }
  bool empty() const { return !num_items_; }

private:
  /// element storage
  item_type buf_;

  /// Number of items in the buffer
  size_type num_items_;

}; // class osss_fifo_buf<T,1>

// --------------- partial specialization for size == 0 ------------------

/// invalid size '0' --  deliberately not defined !
template< typename T >
class osss_circ_buffer<T,0>;

OSSS_SYNTHESISABLE_END_

OSSS_SYNTHESISABLE_EXPORT_(osss_circ_buffer);

} // end namespace osss

#endif // OSSS_CIRC_BUFFER_H_INCLUDED
// :flavour: (osss,H) (osss_recon,h) (osss_full,h)
// Taf!
