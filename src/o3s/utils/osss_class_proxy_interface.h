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

#ifndef OSSS_CLASS_PROXY_INTERFACE_H
#define OSSS_CLASS_PROXY_INTERFACE_H

#include "o3s/utils/type_id.h"
#include <list>

namespace osss {

class osss_object;

namespace osssi {

class osss_class_proxy_interface
{
public:
  /** Type definition for list of our base classes */
  typedef std::list< osss_class_proxy_interface * > base_class_list_type;

  virtual
  ~osss_class_proxy_interface(){}

  /** Get the type_info struct for the C++ class */
  virtual
  type_id getTypeInfo() const = 0;

  /** Get a list of proxies for the base classes */
  virtual
  const base_class_list_type &
  getListOfDirectBaseClasses() const = 0;

  /** Extend the list of base classes. This class does
      not destruct the passed proxy on destruction. */
  virtual
  void
  addBaseClass(osss_class_proxy_interface * base_class_proxy) = 0;

  virtual
  osss_object*
  createInstance() const = 0;

  virtual
  osss_object*
  getClone( const osss_object& ) const = 0;

};

} // end namespace osssi
} // end namespace osss

#endif // OSSS_CLASS_PROXY_INTERFACE_H
// $Id: osss_class_proxy_interface.h 3021 2009-03-04 22:55:21Z philipph $
// :flavour: (osss_full,h) (osss_icodes,h) (osss_recon,h) (osss,h)
