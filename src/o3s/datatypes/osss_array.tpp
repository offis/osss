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

#include "osss_array.h"
#include "o3s/utils/osss_report.h"

#include <sstream>

namespace osss {
namespace osssi {

//-----------------------------------------------------------------------//

template< class T>
inline
osss_array_base<T>::osss_array_base(size_type size)
  : m_elements( size )
{}

//-----------------------------------------------------------------------//

template< class T >
inline
typename osss_array_base<T>::size_type
osss_array_base<T>::size() const
{
  return m_elements.size();
}

//-----------------------------------------------------------------------//

template< class T>
inline
typename osss_array_base<T>::reference
osss_array_base<T>::operator[]( size_type index )
{
  if( osss_likely( index < m_elements.size() ) )
    return m_elements[index];

  OSSS_REPORT_ERROR( report::index_out_of_bounds )
    % "osss_array_base<>::operator[]" % index % this->size();
  // really abort here
  OSSS_ABORT();
}

//-----------------------------------------------------------------------//

template< class T>
inline
typename osss_array_base<T>::const_reference
osss_array_base<T>::operator[]( size_type index ) const
{
  if( osss_likely( index < m_elements.size() ) )
    return m_elements[index];

  OSSS_REPORT_ERROR( report::index_out_of_bounds )
    % "osss_array_base<>::operator[]" % index % this->size();
  // really abort here
  OSSS_ABORT();
}

//-----------------------------------------------------------------------//

template< class T>
inline bool
osss_array_base<T>::operator==( const this_type& rhs ) const
{
  return ( m_elements == rhs.m_elements );
}

template< class T >
void
osss_array_base<T>::resize( size_type sz )
{
  m_elements.resize( sz );
}

// Dumps the actual content of the array into an ostream.
// Overloading of this operator is required by SystemC.
template< typename T >
::std::ostream &
operator<<( ::std::ostream & os
          , const osss_array_base<T> & object )
{
  typedef typename osss_array_base<T>::size_type size_type;
  size_type size = object.size();

  for ( size_type i=0; i<size; ++i ) {
    os << object[i] << " ";
  }
  return os;
}

} // end namespace osssi
} // end namespace osss

//-----------------------------------------------------------------------//

namespace sc_core {
// Overloading this method is required by SystemC. But it does
// actually have some reasonable functionality for instances of
// this class.
template< class T, size_t Size >
void
sc_trace( sc_core::sc_trace_file *          tf
        , const osss::osss_array<T, Size> & arr
        , const std::string &               name )
{
  typedef typename osss::osss_array<T,Size>::size_type size_type;
  const size_type size = Size;

  for ( size_type i=0; i<size; ++i ) {
    std::stringstream name_i;
    name_i << name << "(" << i << ")";
    sc_trace( tf, arr[i], name_i.str() );
  }
}

} // end namespace sc_core


//-----------------------------------------------------------------------//
// $Id: osss_array.tpp 2737 2008-11-27 09:50:25Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
