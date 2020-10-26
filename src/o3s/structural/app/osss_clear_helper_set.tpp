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

namespace osss{
namespace osssi{

template< class T >
osss_clear_helper_set<T>::~osss_clear_helper_set()
{
  // We have to free all helpers
  while (false == m_helper_map.empty())
  {
    // delete the helper
    delete m_helper_map.begin()->second;
    // and forget the entry
    m_helper_map.erase(m_helper_map.begin());
  }
}

template< class T >
void
osss_clear_helper_set<T>::clearRecursive(      osss_object *                object_ptr,
                                         const osss_class_proxy_interface * proxy) const
{
  // first, test if object_ptr has the desired type (for safety)
  T * obj = dynamic_cast< T* >(object_ptr);
  if (NULL == obj)
  {
    std::cerr << "ERROR! Object type mismatch in " << __func__ << "()" << std::endl;
    OSSS_ABORT();
  }
  // Yes, type is ok. Now apply all helpers to clear the object
  typename helper_map_type::const_iterator hlp_it = m_helper_map.begin();
  while (m_helper_map.end() != hlp_it)
  {
    // use the helper to clear an attribute
    hlp_it->second->clear( dynamic_cast< T *>(obj), hlp_it->first );
    // proceed to next helper
    ++hlp_it;
  }
  // recursively utilise base classes
  const osss_class_proxy_interface::base_class_list_type & base_class_list
          = proxy->getListOfDirectBaseClasses();
  osss_class_proxy_interface::base_class_list_type::const_iterator base_class_it
          = base_class_list.begin();
  while (base_class_list.end() != base_class_it)
  {
    // Now base_class_it points to a class proxy of a base class.
    // Find the helper set...
    osss_clear_helper_registry::instance.clear(object_ptr, (*base_class_it));
    // next base class
    ++base_class_it;
  }
}

template< class T >
template< class A >
void
osss_clear_helper_set<T>::addHelper(      T * object_ptr,
                                          A * attribute_ptr,
                                    const A   clear_value)
{
  // Calculate attribute offset
  offset_type offset =  reinterpret_cast< address_type >( attribute_ptr )
                      - reinterpret_cast< address_type >( object_ptr );
  // Try to find a helper for that offset
  typename helper_map_type::iterator it = m_helper_map.find( offset );
  // If not present
  if (m_helper_map.end() == it)
  {
    // Generate a helper for the offset
    m_helper_map.insert(std::make_pair(offset, new osss_clear_helper< T, A >( clear_value )));
  }
  // otherwise simply do nothing
}

template< class T >
void
osss_clear_helper_set<T>::printOn(std::ostream & os) const
{
  os << "List of clear helpers for class \""
     << type_id::from<T>() << "\":" << std::endl;
  if (m_helper_map.empty())
  {
    os << "(no helpers for this class)" << std::endl;
  }
  else
  {
    typename helper_map_type::const_iterator hlp_it = m_helper_map.begin();
    while (m_helper_map.end() != hlp_it)
    {
      os << "attr_offset=\"" << hlp_it->first << "\" ";
      hlp_it->second->printOn(os);
      ++hlp_it;
    }
  }
}

} // end namespace osssi
} // end namespace osss

// $Id: osss_clear_helper_set.tpp 3021 2009-03-04 22:55:21Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
