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
#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/osss_report.h"

namespace osss{
namespace osssi{

template< class T >
osss_object *
osss_class_proxy< T >::createInstance() const
{
  T * t = new T();
  if (NULL == dynamic_cast< osss_object* >(t))
  {
    delete t;
    std::cerr << "ERROR! Attempt to call getOSSSObjectInstance() for class \"" << typeid(t).name() << "\"." << std::endl
              << "       Apparently this class is not derived from osss_object and/or has no virtual destructor." << std::endl;
    OSSS_ABORT();
  }
  return dynamic_cast< osss_object* >(t);
}


template< class T >
osss_object *
osss_class_proxy< T >::getClone(const osss_object & original) const
{
  /* FIXME: Think of sanity checks */
  T * clone = new T();
  const T* source = dynamic_cast<const T*>(&original);
  *clone = *source;
  return clone;
}


template< class T >
type_id
osss_class_proxy< T >::getTypeInfo() const
{
  return type_id::from<T>();
}

template< class T >
const osss_class_proxy_interface::base_class_list_type &
osss_class_proxy< T >::getListOfDirectBaseClasses() const
{
  return m_direct_base_class_list;
}

template< class T >
void
osss_class_proxy< T >::addBaseClass(osss_class_proxy_interface * base_class_proxy)
{
  // first see, if there is an suitable entry already
  base_class_list_type::iterator it = m_direct_base_class_list.begin();
  bool found = false;
  while (!found && (m_direct_base_class_list.end() != it))
  {
    found = (*it == base_class_proxy);
    ++it;
  }
  if (!found)
  {
    m_direct_base_class_list.push_back(base_class_proxy);
  }
}

} // end namespace osssi
} // end namespace osss
// $Id: osss_class_proxy.tpp 3021 2009-03-04 22:55:21Z philipph $
// :flavour:(osss_recon,h) (osss,h) (osss_icodes,h) (osss_full,h)
