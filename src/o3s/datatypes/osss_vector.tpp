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

#include "o3s/utils/osss_report.h"

namespace osss {

template< class T >
inline
osss_vector<T>::osss_vector( size_type initialSize /* = 0 */ )
  : base_type(initialSize)
{
  // We do this to inhibit creation of vectors of different
  // size during simulation. We want to avoid code like this:
  // ...
  // size = someport.read();
  // osss_vector< size > my_vector;
  // ...
  // We couldn't ever synthesise this :(
  // However, if "size" was constant, it wouldn't be a problem.
  // But then the user should use osss_array...

#if 1
  if( osss_unlikely( sc_core::sc_start_of_simulation_invoked() ) )
  {
    OSSS_REPORT_ERROR( report::called_after_elaboration )
      % __PRETTY_FUNCTION__;
  }
#endif
}

//-----------------------------------------------------------------------//

template< class T >
inline void
osss_vector<T>::resize( size_type newSize )
{
  if( sc_core::sc_start_of_simulation_invoked() )
  {
    OSSS_REPORT_ERROR( report::called_after_elaboration )
      % __PRETTY_FUNCTION__;
  }

  base_type::resize(newSize);
}

//-----------------------------------------------------------------------//

template< class T >
inline
typename osss_vector<T>::const_reference
osss_vector<T>::operator[]( size_type index ) const
{
  return base_type::operator[](index);
}

//-----------------------------------------------------------------------//

template< class T >
inline
typename osss_vector<T>::reference
osss_vector<T>::operator[]( size_type index )
{
  if ( osss_unlikely( index >= base_type::size() ))
  {
    if( sc_core::sc_start_of_simulation_invoked() )
    {
      OSSS_REPORT_ERROR( report::index_out_of_bounds )
        % "osss_vector<>::operator[]"
        % index % base_type::size()
        << "\nAutomatic/implicit extension of an osss_vector is "
           "allowed during elaboration only!";
    }

    // resize vector to hold new element(s)
    base_type::resize( index + 1 );
  }
  return base_type::operator[](index);
}

} // end namespace osss

//-----------------------------------------------------------------------//

namespace sc_core {

// Overloading this method is required by SystemC. But it does
// actually have some reasonable functionality for instances of this class.
template< class T >
void
sc_trace ( sc_core::sc_trace_file *
         , const osss::osss_vector<T> &
         , const std::string & )
{
  OSSS_REPORT_WARNING( osss::report::not_implemented )
    % __PRETTY_FUNCTION__;
}

} // end namespace sc_core

//-----------------------------------------------------------------------//
// $Id: osss_vector.tpp 2737 2008-11-27 09:50:25Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
