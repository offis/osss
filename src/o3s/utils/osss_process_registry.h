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
#ifndef OSSS_PROCESS_REGISTRY_HH
#define OSSS_PROCESS_REGISTRY_HH

#include "osss_process_handle.h"

#include <iostream>
#include <map>

//forward declaration of sc_core::sc_object
namespace sc_core {
class sc_object;
}

namespace osss {
namespace osssi {

////////////////////////////////////////////////////////////////////////////
///
/// \brief This class implements a registry for systemc processes.
///
/// Each process gets a unique ID that is counted from 1-N
/// The ID 0 (zero) is the "invalid" ID
///
/// \note Static class.
///
////////////////////////////////////////////////////////////////////////////

class osss_process_registry
{
public:
  typedef osss_process_handle::id_type id_type;

  //! Returns the unique ID of the requested osss_process_handle.
  //! If the osss_process_handle is invalid 0 (zero) is returned
  static
  id_type
  get_proc_id(const osss_process_handle& proc_handle);

  static
  const char *
  get_proc_name(const id_type id);

  //! Prints the registry.
  //!
  //!  \note Used for debugging only.
  static void print();

private:

  //! This map implements the registry.
  //! - first:  pointer to the base class of the process
  //! - second: unique ID
  static
  std::map<const sc_core::sc_object*, id_type> osss_process_id_map;

  //! The counter that is used for generating the unique IDs
  static id_type m_id_counter;
};

} // namespace osssi

} // namespace osss

#endif // OSSS_PROCESS_REGISTRY_HH
// $Id: osss_process_registry.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
