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

#include "o3s/utils/type_id.h"

namespace osss{
namespace osssi{

template< class T, class A >
osss_clear_helper< T, A >::osss_clear_helper(A _val)
{
  clear_value = _val;
}


template< class T, class A >
void
osss_clear_helper< T, A >::printOn(std::ostream & os) const
{
  os << "attr_type=\"" << type_id::from<A>()
     << "\" clear_value=\"" << clear_value
     << "\"" << std::endl;
}

template< class T, class A >
void
osss_clear_helper< T, A >::clear(T * object_ptr, offset_type offset) const
{
  // calculate address of attribute from object start address and attribute offset
  address_type attribute_address = reinterpret_cast< address_type >( object_ptr ) + offset;
  // convert it to the suitable ptr and assign the clear value
  *(reinterpret_cast< A* >( attribute_address )) = clear_value;
}

} // end namespace osssi
} // end namespace osss

// $Id: osss_clear_helper.tpp 3021 2009-03-04 22:55:21Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
