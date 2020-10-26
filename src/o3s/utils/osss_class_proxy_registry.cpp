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

#include "osss_class_proxy_registry.h"
#include <algorithm>

using namespace std;

namespace osss{
namespace osssi{

// static class member pointer
osss_class_proxy_registry*
osss_class_proxy_registry::instance_ = 0;

osss_class_proxy_registry::osss_class_proxy_registry()
{
}

osss_class_proxy_registry::~osss_class_proxy_registry()
{
    // FIXME: Don't we need to clean up our proxies?
    // No, we use a stl container which does that on
    // its own destruction
}

osss_class_proxy_interface *
osss_class_proxy_registry::findProxy( type_id for_type ) const
{
  // find the proxy iterator
  registry_type::const_iterator it = m_registry.find( for_type );

  if (m_registry.end() == it)
  {
    // not found
    return NULL;
  }
  else
  {
    return it->second;
  }
}

} // end namespace osssi
} // end namespace osss
// $Id: osss_class_proxy_registry.cpp 3021 2009-03-04 22:55:21Z philipph $
// :flavour:(osss_recon,s) (osss,s)(osss_icodes,s)(osss_full,s)
