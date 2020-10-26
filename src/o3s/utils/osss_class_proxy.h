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

#ifndef OSSS_CLASS_PROXY_H
#define OSSS_CLASS_PROXY_H

#include "osss_class_proxy_interface.h"

namespace osss{
namespace osssi{

/** This class stores information on a C++ class and
    also provides some utility methods [getInstance()
    and getClone()]. The template parameter T
    is meant to be the C++ class this proxy does repesent.
*/

template< class T >
class osss_class_proxy
  : public osss_class_proxy_interface
{
private:
  /** Store a list of class proxy references for the
      direct base classes of the C++ class we represent. */
  base_class_list_type m_direct_base_class_list;

public:
  osss_class_proxy()
    : m_direct_base_class_list()
  {}
  virtual ~osss_class_proxy(){}

  /** Get an instance of T constructed using the default constructor */
  osss_object*
  createInstance() const;

  /** Get a clone of T& */
  osss_object*
  getClone( const osss_object& original ) const;

  /** Get the type_info struct for the C++ class */
  virtual
  type_id getTypeInfo() const;

  /** Get a list of proxies for the base classes */
  virtual
  const base_class_list_type &
  getListOfDirectBaseClasses() const;

  /** Extend the list of base classes. This class does
      not destruct the passed proxy on destruction. */
  virtual
  void
  addBaseClass(osss_class_proxy_interface * base_class_proxy);
};

} // end namespace osssi
} // end namespace osss

#include "osss_class_proxy.tpp"

#endif // OSSS_CLASS_PROXY_H
// $Id: osss_class_proxy.h 3021 2009-03-04 22:55:21Z philipph $
// :flavour:(osss_recon,h) (osss,h) (osss_icodes,h) (osss_full,h)
