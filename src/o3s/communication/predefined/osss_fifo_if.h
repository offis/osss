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
#ifndef OSSS_FIFO_IF_H_INCLUDED_
#define OSSS_FIFO_IF_H_INCLUDED_

#include "o3s/utils/osss_systemc.h"

namespace osss {

OSSS_SYNTHESISABLE_BEGIN_

template< typename T >
struct osss_fifo_put_if
  : public virtual sc_core::sc_interface
{
  typedef T                   item_type;
  typedef osss_fifo_put_if<T> if_type;

  /// Store an item in the FIFO if it is not full
  virtual void put(item_type item) = 0;

  /// Check whether the FIFO is full
  virtual bool is_full() = 0;
};

template< typename T >
struct osss_fifo_get_if
  : public virtual sc_core::sc_interface
{
  typedef T                   item_type;
  typedef osss_fifo_get_if<T> if_type;

  /// Get the head item if the FIFO is not empty
  virtual item_type get()  = 0;

  /// Check whether the FIFO is empty
  virtual bool is_empty() = 0;

};

template< typename T >
struct osss_fifo_if
  : osss_fifo_put_if< T >
  , osss_fifo_get_if< T >
{
  // disambiguate typedefs
  typedef T               item_type;
  typedef osss_fifo_if<T> if_type;
};

OSSS_SYNTHESISABLE_END_

OSSS_SYNTHESISABLE_EXPORT_(osss_fifo_get_if);
OSSS_SYNTHESISABLE_EXPORT_(osss_fifo_put_if);
OSSS_SYNTHESISABLE_EXPORT_(osss_fifo_if);

} // namespace osss

#endif // OSSS_FIFO_IF_H_INCLUDED_
// :flavour: (osss,H) (osss_recon,h) (osss_full,h)
// // Taf!
