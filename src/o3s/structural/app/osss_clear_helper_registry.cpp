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

#include "osss_clear_helper_registry.h"
#include "o3s/utils/osss_class_proxy_interface.h"
#include "o3s/utils/osss_class_proxy_registry.h"
#include "o3s/datatypes/osss_object.h"

namespace osss{
namespace osssi{

osss_clear_helper_registry::osss_clear_helper_registry()
{
}

osss_clear_helper_registry::~osss_clear_helper_registry()
{
  // Clear all helper sets
  while (!m_registry.empty())
  {
    // destruct the set
    delete m_registry.begin()->second;
    // forget the entry
    m_registry.erase( m_registry.begin() );
  }
}

void
osss_clear_helper_registry::printOn(std::ostream & os) const
{
  os << "Recursive list of all clear helper sets:" << std::endl;
  registry_type::const_iterator it = m_registry.begin();
  if (m_registry.end() == it)
  {
    os << "(empty list of clear sets)" << std::endl;
  }
  else
  {
    unsigned int count = 1;
    while (m_registry.end() != it)
    {
      os << "--- Entry #" << count << " ---" << std::endl;
      osss_clear_helper_set_interface * chsi_ptr = it->second;
      os << *chsi_ptr;
      ++it;
      ++count;
    }
  }
}

void
osss_clear_helper_registry::clear(      osss_object                * object_ptr,
                                  const osss_class_proxy_interface * proxy) const
{
  // fetch type_info for given object
  type_id ti( *object_ptr );
  // if the user did not pass a proxy
  if (NULL == proxy)
  {
    // Find a suitable one
    proxy = osss_class_proxy_registry::instance().findProxy( ti );
    // If not found...
    if (NULL == proxy)
    {
      // This indicates someone forgot OSSS_BASE_CLASS()
      // calls or the compiler has a really weird RTTI...
      std::cerr << "ERROR! No class proxy found for \""
                << ti.name() << "\"" << std::endl
                << "       Maybe you forgot some OSSS_BASE_CLASS() entries?"
                << std::endl;
      OSSS_ABORT();
    }
  }
  // Now "proxy" is valid

  // find helper set
  registry_type::const_iterator it = m_registry.find( proxy->getTypeInfo() );
  // if found, apply it
  if (m_registry.end() != it)
  {
    it->second->clearRecursive(object_ptr, proxy);
  }
}

// Our instance...
osss_clear_helper_registry
osss_clear_helper_registry::instance = osss_clear_helper_registry();

} // end namespace osssi
} // end namespace osss

// $Id: osss_clear_helper_registry.cpp 3021 2009-03-04 22:55:21Z philipph $
// :flavour: (osss,s) (osss_recon,s) (osss_full,s)
