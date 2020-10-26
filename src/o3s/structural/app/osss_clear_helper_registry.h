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

#ifndef OSSS_CLEAR_HELPER_REGISTRY_H
#define OSSS_CLEAR_HELPER_REGISTRY_H

#include "o3s/utils/type_id.h"
#include "osss_clear_helper_set_interface.h"

#include <map>
#include <list>
#include <iostream>

namespace osss{
namespace osssi{

/** This class maintains a registry of clear helper sets
    associated to C++ classes. We use type_info* as
    class identifications in order to have a key for
    indexing. We assume here, that the C++ compiler does
    NOT use a fresh type_info structure for each
    occurence of a class, sonce the type_info structure
    is obained as const only. Additionally, the ISO standard
    says that the type_info structure persists until the
    program ends. However, the standard does not state
    that these structures are unique. It may well happen
    that some few instances of a class have different type_info's.
    (I could imagine this happening when linking several
    libraries that use common base classes provided by source.)
    In case of multiple type_infos we treat these as different
    classes. They just have ("by accident") clear helpers doing
    the same job the same way.

    Not trying to use the same helper sets whenever two
    type_info structures are equivalent may serve us in another
    case. Imagine multiple inheritance cases where one parent
    class is not the first one (appears somewhere right in the
    inheritance list). C++ compilers may choose to treat these
    classes differently when it comes to memory layout than
    ordinary classes of the same type.
    If this is encoded in type_info (and it is likely to be this
    way) we can handle that situation well.
*/

class osss_clear_helper_registry
{
private:
  /** This is our registry. For each key we maintain a set
      of clear helpers */
  typedef std::map< type_id, osss_clear_helper_set_interface * > registry_type;
  /** This is our internal registry instance. */
  registry_type m_registry;

public:
  osss_clear_helper_registry();
  ~osss_clear_helper_registry();

  /** This method allows to add a helper. The appropriate set
      is determined internally by this method by looking at
      the runtime type of *object_ptr. An example object
      in object_ptr, the address of the attribute to be cleared
      in attribute_ptr and the clear value in clear_value.
      The example object is not altered by this call. */
  template< class T, class A >
  void
  add(      T * object_ptr,
            A * attribute_ptr,
      const A   clear_value);

  /** Apply clear helpers to the passed object. If you
      want to start while interpreting the object to
      be of a certain type, you may pass a proxy here.
      Usually you don't. This feature is used to
      implement ascending the class tree while clearing.
      Passing NULL forces determining the start class
      using the RTTI (that is what you probably want as
      a user) */
  void
  clear(      osss_object                * object_ptr,
        const osss_class_proxy_interface * proxy = NULL) const;

  void
  printOn(std::ostream & os) const;

  /** This is the static instance of this class. Start
      here if you want to use the services of our registry. */
  static osss_clear_helper_registry instance;
};

} // end namespace osssi
} // end namespace osss

#include "osss_clear_helper_registry.tpp"

#endif // OSSS_CLEAR_HELPER_REGISTRY_H

// $Id: osss_clear_helper_registry.h 3021 2009-03-04 22:55:21Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
