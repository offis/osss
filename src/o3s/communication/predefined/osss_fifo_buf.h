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
#ifndef OSSS_FIFO_BUF_H_INCLUDED_
#define OSSS_FIFO_BUF_H_INCLUDED_

#include "osss_fifo_if.h"

#if defined( OSSS_BLUE )
#  include "o3s/structural/app/osss_hw_macros_blue.h"
#elif defined( OSSS_GREEN )
#  include "o3s/structural/arch/osss_hw_macros_green.h"
#endif

#include "o3s/datatypes/osss_circ_buffer.h"

namespace osss {
OSSS_SYNTHESISABLE_BEGIN_

template< typename T, size_t Size = 1 >
class osss_fifo_buf
  : public osss_fifo_if<T>
{
  CLASS(osss_fifo_buf, NullClass);
public:
  typedef osss_fifo_if<T>                   if_type;
  typedef typename if_type::item_type       item_type;
  typedef osss_circ_buffer<item_type, Size> storage_type;
  typedef typename storage_type::size_type  size_type;

  static const size_type size = storage_type::size;

  // Default constructor -- creates an empty FIFO
  CONSTRUCTOR(public,osss_fifo_buf,()){}

  // Store an item in the FIFO if it is not full
  OSSS_GUARDED_METHOD_VOID( put, OSSS_PARAMS(1, item_type, item),
    ( ! buf_.full() ) )
  {
    buf_.put( item );
  }

  // Get the head item if the FIFO is not empty
  OSSS_GUARDED_METHOD( item_type, get, OSSS_PARAMS(0),
    ( ! buf_.empty() ) )
  {
    return buf_.get();
  }

  // Check whether the FIFO is empty
  OSSS_GUARDED_METHOD( bool, is_empty, OSSS_PARAMS(0), true )
    { return buf_.empty(); }

  // Check whether the FIFO is full
  OSSS_GUARDED_METHOD( bool, is_full,  OSSS_PARAMS(0), true )
    { return buf_.full(); }

private:

  // element storage
  storage_type buf_;

}; // class osss_fifo_buf<T,1>

OSSS_SYNTHESISABLE_END_
OSSS_SYNTHESISABLE_EXPORT_( osss_fifo_buf );


} // namespace osss

#endif // OSSS_FIFO_BUF_H_INCLUDED_
// :flavour: (osss,H) (osss_recon,h) (osss_full,h)
// // Taf!
