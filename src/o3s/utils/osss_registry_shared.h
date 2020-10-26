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
#ifndef OSSS_REGISTRY_SHARED_HH
#define OSSS_REGISTRY_SHARED_HH

#include <iostream>
#include <map>

// forward declaration of sc_core::sc_interface
namespace sc_core {
  class sc_interface;
}

// forward declaration of osss::osss_shared_if<...>
namespace osss {
  template<class IF> class osss_shared_if;
}

namespace osss {

namespace osssi {

////////////////////////////////////////////////////////////////////////////
///
/// \brief This class implements a registry for shared objects.
///   
/// Each user defined object which can be plugged into a shared object
/// is associated with its surrounding shared object.
///
/// \note Static class.
///
////////////////////////////////////////////////////////////////////////////

class osss_registry_shared
{
public:

  //! Registers an arbitrary object (po) at a shared object.
  static 
  void 
  add_plugged_object(::osss::osss_shared_if<sc_core::sc_interface>* so, 
		     const void* po);
  
  //! Removes the object from the registry.
  static 
  void 
  delete_plugged_object(const void* po);

  //! Return the shared object associated with an arbitrary object.
  //! If the object is not associated with any shared object NULL
  //! is returned.
  static 
  ::osss::osss_shared_if<sc_core::sc_interface>* 
  get_shared_object(const void* const object);

  static
  const void* 
  get_plugged_object(const ::osss::osss_shared_if<sc_core::sc_interface>* sif);

  //! Prints the registry.
  //!
  //!  \note Used for debugging only.
  static void print();

private:

  //! This map implements the registry.
  //! - first:  embedded object
  //! - second: shared object containing the above object
  static 
  std::map<const void*, 
	   ::osss::osss_shared_if<sc_core::sc_interface>* > osss_shared_map;
};

} // namespace osssi

} // namespace osss

#endif
// $Id: osss_registry_shared.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
