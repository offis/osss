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
#include "osss_class_proxy.h"

namespace osss {
namespace osssi {

template< class T >
osss_class_proxy_interface *
osss_class_proxy_registry::addProxy(T*)
{
  // get std::typeinfo for class
  type_id ti = type_id::from<T>();
  // find the proxy for ti
  osss_class_proxy_interface * proxy = findProxy(ti);
  // is there a proxy for that type?
  if (NULL == proxy)
  {
    // no such proxy, insert one
    proxy = new osss_class_proxy<T>;
    m_registry.insert(std::make_pair(ti, proxy));
  }
  return proxy;
}

template< class T >
T*
osss_class_proxy_registry::getClone( const T& source )
{
  this_type& instance = this_type::instance();
  osss_class_proxy_interface* proxy_if;
  if( NULL == ( proxy_if = instance.findProxy( type_id( source )))) {
   std::cerr << "ERROR! Attempt to call getClone() for class \""
             << type_id(source) << "\",\n"
             << "       which is not registered or not derived from osss_object. Ensure, that "
             << "       OSSS_BASE_CLASS(...) is called in it's constructor!"
             << std::endl;
    OSSS_ABORT();
  }
  return dynamic_cast<T*>( proxy_if->getClone( source ) );
}

} // end namespace osssi
} // end namespace osss
// $Id: osss_class_proxy_registry.tpp 3021 2009-03-04 22:55:21Z philipph $
// :flavour:(osss_recon,h) (osss,h) (osss_icodes,h) (osss_full,h)
