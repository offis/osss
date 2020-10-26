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

namespace osss {

template< class T, unsigned int Size >
inline
osss_serialisable_array<T, Size>::osss_serialisable_array()
  : base_type()
  , m_used_size(Size)
{}

//-----------------------------------------------------------------------//

template< class T, unsigned int Size >
inline
osss_serialisable_array<T, Size>::
osss_serialisable_array(const osss_serialisable_array<T, Size> &sourceArray) :
  base_type(sourceArray),
  m_used_size(sourceArray.m_used_size)
{}

//----------------------------------------------------------------------------//

template< class T, unsigned int Size >
inline
osss_serialisable_array<T, Size>::~osss_serialisable_array()
{}

//----------------------------------------------------------------------------//

template< class T, unsigned int Size >
inline
osss_serialisable_array<T, Size> &
osss_serialisable_array<T, Size>::
operator=( const osss_serialisable_array<T, Size> & sourceArray )
{
  if ( &sourceArray != this )
  {
    base_type::operator=(sourceArray);
    m_used_size = sourceArray.m_used_size;
  }
  return(*this);
}

template< class T, unsigned int Size >
inline
void 
osss_serialisable_array<T, Size>::used_size(size_type s)
{
  if (s <= Size)
    m_used_size = s;
  else
    m_used_size = Size;
}

template< class T, unsigned int Size >
inline
typename osss_serialisable_array<T, Size>::size_type
osss_serialisable_array<T, Size>::used_size() const
{
  return m_used_size;
}

//-----------------------------------------------------------------------//

template< class T, unsigned int Size >
void
osss_serialisable_array<T, Size>::serialise()
{
  osss_serialisable_object::store_element(m_used_size);
  // only store the elemets from 0 to used_size
  for(size_type i=0; i<m_used_size; i++)
    osss_serialisable_object::store_element(base_type::operator[](i));
}

template< class T, unsigned int Size >
void
osss_serialisable_array<T, Size>::deserialise()
{
  osss_serialisable_object::restore_element(m_used_size);
  // only restore the elements from 0 to used_size
  for(size_type i=0; i<m_used_size; i++)
    osss_serialisable_object::restore_element(base_type::operator[](i));
  // for other elements call default constructor
  for(size_type i = m_used_size; i<Size; i++)
    base_type::operator[](i) = T();
}

} // end namespace osss

//-----------------------------------------------------------------------//
// $Id: osss_serialisable_array.tpp 2737 2008-11-27 09:50:25Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
