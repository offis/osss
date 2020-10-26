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
#include "osss_report.h"
#include "osss_assert.h"

namespace osss {
namespace osssi {

/**
 * default constructor, currently empty
 */
template< class key, class info >
osss_pool< key, info >::osss_pool()
{
}

template< class key, class info >
inline
typename osss_pool< key, info >::index_type
osss_pool< key, info >::size() const
{
  // return pool size
  return m_key_table.size();
}

template< class key, class info >
inline
bool
osss_pool< key, info >::empty() const
{
  return m_key_table.empty();
}


template< class key, class info >
typename osss_pool< key, info >::index_type
osss_pool< key, info >::add(key k, const info & info_block)
{
  // add key to pool
  index_type new_idx = this->add(k);
  // store info for key/index
  *(m_info_table[new_idx]) = info_block;
  return m_key_table.size()-1;
}

template< class key, class info >
typename osss_pool< key, info >::index_type
osss_pool< key, info >::add(key k)
{
  // sanity check
  OSSS_ASSERT( (key)(NULL) != k && "key is NULL" );

  m_key_table.push_back(k);
  m_info_table.push_back(new info());
  return m_key_table.size()-1;
}


template< class key, class info >
typename osss_pool< key, info >::index_type
osss_pool< key, info >::findIndex(const key& k) const
{
  index_type key_count = m_key_table.size();
  for (index_type index = 0; index < key_count; ++index)
  {
    if (m_key_table[index] == k)
    {
      return index;
    }
  }
  // Not found, return an illegal key
  return size();
}

template< class key, class info >
const key&
osss_pool< key, info >::getKey(index_type i) const
{
  return m_key_table[i];
}

template< class key, class info >
const key&
osss_pool< key, info >::getKey(info * i) const
{
  index_type key_count = m_key_table.size();
  for (index_type index = 0; index < key_count; ++index)
  {
    if (m_info_table[index] == i)
    {
      return m_key_table[index];
    }
  }
  OSSS_REPORT_FATAL( osss::report::internal_error )
    % " Cannot find info ptr by key";
  OSSS_ABORT();
}


template< class key, class info >
info *
osss_pool< key, info >::getInfoPtrByIndex(index_type i) const
{
  return m_info_table[i];
}

template< class key, class info >
info *
osss_pool< key, info >::getInfoPtrByKey(key k) const
{
  // sanity check
  OSSS_ASSERT( (key)(NULL) != k && "key is NULL" );

  index_type idx = findIndex(k);
  if (false == isIndexLegal(idx))
  {
    OSSS_REPORT_FATAL( osss::report::internal_error )
      % " Cannot find index for key";
  }

  return m_info_table[idx];
}


template< class key, class info >
bool
osss_pool< key, info >::isIndexLegal(index_type idx) const
{
  // stl::size_type is unsigned. Illegal keys
  // must be larger or equal to the table size
  return (idx < m_key_table.size());
}
} // end namespace osssi
} // end namespace osss
// $Id: osss_pool.tpp 2815 2008-12-01 17:07:41Z claus $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
