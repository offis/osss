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

#ifndef OSSS_POOL_H
#define OSSS_POOL_H

#include <vector>
#include <iostream>

namespace osss{

namespace osssi{

/** This class provides a container which fits into
    the special needs of osss_recon's internals.
    It associates key to information structures.
    Additionally, you can access the information
    structures via an index, not just the key! */
template< class key, class info >
class osss_pool
{
  private:
  /// These are pairs, index-wise
  std::vector< key >    m_key_table;
  std::vector< info * > m_info_table;

  public:
  /// Conform to STL requirements and use their size_type
  typedef typename std::vector< key >::size_type index_type;

  /// Default constructor.
  osss_pool();

  /** Adds a key to the pool. Info structure is created with
      default constructor and added, too. */
  index_type add(key k);

  /** In this version of "add()" the caller can give
     the initial values for the info_block.
     These values are copied into the fresh block. */
  index_type add(key k, const info & info_block);

  /** Find the index for a given key and return it.
      This method returns an illegal key, if the key
      is not found. */
  inline
  index_type findIndex(const key& k) const;

  /// Find key for given index and return it
  const key& getKey(index_type i) const;

  /** Find key for a given information structure.
     The structure is compared by address, not
     by value. */
  const key& getKey(info * i) const;

  /// Find the information block by index
  info * getInfoPtrByIndex(index_type i) const;

  /// Find the information block by type
  info * getInfoPtrByKey(key k) const;

  /// Return the current size of the pool
  inline
  index_type size() const;

  /// Returns true, iff at least one element present
  inline
  bool empty() const;

  /** Tests, if the index is legal.
      Use this method to test, if findIndex()
      returned a valid index. */
  inline
  bool isIndexLegal(index_type) const;
};

} // end namespace osssi
} // end namespace osss

#include "osss_pool.tpp"

#endif // OSSS_POOL_H
// $Id: osss_pool.h 2594 2008-09-01 12:59:35Z andreas $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
