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
#include "osss_clear_helper_set.h"
#include "o3s/utils/osss_class_proxy_registry.h"

namespace osss{
namespace osssi{

template< class T, class A >
void
osss_clear_helper_registry::add(      T * object_ptr,
                                      A * attribute_ptr,
                                const A   clear_value)
{
  type_id ti( *object_ptr );
  // find the helper set for the passed type
  registry_type::iterator reg_it = m_registry.find(ti);
  if (m_registry.end() == reg_it)
  {
    // If we get here, there ist noch clear helper set
    // for the desired type, yet. We create it.
    m_registry.insert(std::make_pair(ti, new osss_clear_helper_set<T>()));
    reg_it = m_registry.find(ti);
  }
  // Now reg_it should be valid...
  dynamic_cast< osss_clear_helper_set<T> * >(reg_it->second)->addHelper(object_ptr, attribute_ptr, clear_value);
}

} // end namespace osssi
} // end namespace osss

// $Id: osss_clear_helper_registry.tpp 3021 2009-03-04 22:55:21Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
